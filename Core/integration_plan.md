# Implementation Plan: Coral NPU Core Co-Simulation Integration & Verification

This document outlines the detailed architecture, file changes, and verification plan for integrating the **Coral NPU Core** into **Inference-M-Sim** using an in-process co-simulation model (Option 1).

---

## Goal Description
 
Integrate a `CoralCore` execution module into [`Inference-M-Sim`](file:///home/sjaguri/Inference-M-Sim/README.md) that:
1. Receives tasks/instructions from the Network module via a clean `fetchAndExecute(task, callback)` interface.
2. Invokes a behavioral simulator engine (wrapping Google's **MPACT-CoralNPU**) to perform functional data operations and register state updates.
3. Calculates hardware cycle latencies (based on opcodes, vector lengths, or MAC counts).
4. Schedules discrete timing events using `Inference-M-Sim`'s event engine (`pushEvent(cycles, callback)`).
5. Triggers a completion callback to the Network once the scheduled cycles elapse.

---

## User Review Required

> [!IMPORTANT]
> **Dependency Strategy for MPACT:**  
> To ensure `Inference-M-Sim` remains compilable out-of-the-box on any Linux machine without requiring complex Bazel/MPACT build toolchains pre-installed, we will implement a **pluggable behavioral engine abstraction** (`CoralBehavioralEngine` interface):
> - **`StubBehavioralEngine` (Default):** A clean C++ fallback engine that simulates tensor/vector operations functionally (MAC, Add, ReLU) in pure C++.
> - **`MpactBehavioralEngine` (Production):** Links against Google's `coralnpu-mpact` static library when `USE_MPACT=1` is passed to `make`.

> [!NOTE]
> **Network/Core Contract:**  
> The `CoralCore` will expose a non-blocking `fetchAndExecute(InstructionTask, callback)` method. If the core is `BUSY`, it returns `false` to indicate backpressure to the Network.

---

## Open Questions

> [!NOTE]
> None at present. The proposed architecture directly matches your specification.

---

## Proposed Changes

```
Inference-M-Sim/
├── Core/
│   ├── InstructionTask.hpp        [NEW]  -- Data packet structure (PC, opcodes, data buffers)
│   ├── CoralBehavioralEngine.hpp  [NEW]  -- Abstract interface & Stub/MPACT behavioral wrapper
│   ├── CoralCore.hpp              [NEW]  -- Core SimObject wrapping behavioral engine & timing
│   └── CoralCore.cpp              [NEW]  -- Implementation of fetchAndExecute and event callbacks
├── tests/
│   └── test_coral_core.cpp        [NEW]  -- Automated unit/integration test suite
└── Makefile                       [MODIFY]-- Add build targets for new Core files and test suite
```

---

### 1. `Core/InstructionTask.hpp` [NEW]

Defines the instruction packet passed between Network and Core:

```cpp
#pragma once
#include <cstdint>
#include <vector>
#include <string>

enum class OpcodeType {
    SCALAR_ADD,
    SCALAR_MUL,
    VECTOR_MAC,
    VECTOR_RELU,
    MEM_LOAD,
    MEM_STORE
};

struct InstructionTask {
    uint64_t task_id;
    uint64_t pc;
    OpcodeType opcode;
    uint32_t vector_length; // Number of elements for vector ops
    std::vector<int8_t> input_data;
    std::vector<int8_t> output_data;
};
```

---

### 2. `Core/CoralBehavioralEngine.hpp` [NEW]

Provides the functional execution layer. Defaults to a pure C++ functional stub and allows linking with `MPACT`:

```cpp
#pragma once
#include "InstructionTask.hpp"
#include <iostream>

class CoralBehavioralEngine {
public:
    virtual ~CoralBehavioralEngine() = default;
    virtual void execute(InstructionTask &task) = 0;
};

// Default functional engine (Pure C++)
class StubBehavioralEngine : public CoralBehavioralEngine {
public:
    void execute(InstructionTask &task) override {
        // Perform bit-exact functional simulation for basic ops
        if (task.opcode == OpcodeType::VECTOR_RELU) {
            task.output_data.resize(task.input_data.size());
            for (size_t i = 0; i < task.input_data.size(); ++i) {
                task.output_data[i] = std::max<int8_t>(0, task.input_data[i]);
            }
        } else if (task.opcode == OpcodeType::VECTOR_MAC) {
            task.output_data.resize(task.input_data.size());
            for (size_t i = 0; i < task.input_data.size(); ++i) {
                task.output_data[i] = task.input_data[i] * 2; // Example MAC transformation
            }
        }
    }
};
```

---

### 3. `Core/CoralCore.hpp` & `Core/CoralCore.cpp` [NEW]

Implements the `CoralCore` as a `SimObject`:

```cpp
#pragma once
#include "../Engine/SimObject.hpp"
#include "InstructionTask.hpp"
#include "CoralBehavioralEngine.hpp"
#include <memory>
#include <functional>

class CoralCore : public SimObject {
private:
    bool is_busy = false;
    std::unique_ptr<CoralBehavioralEngine> behavioral_engine;
    uint64_t completed_tasks = 0;

public:
    explicit CoralCore(const std::string &name = "CoralCore_0", 
                      std::unique_ptr<CoralBehavioralEngine> engine = std::make_unique<StubBehavioralEngine>());

    void init() override;
    void startup() override;

    // Interface called by Network
    bool fetchAndExecute(InstructionTask task, std::function<void(const InstructionTask&)> on_complete);

    bool isBusy() const { return is_busy; }
    uint64_t getCompletedTaskCount() const { return completed_tasks; }

private:
    uint64_t calculateCycles(const InstructionTask &task) const;
};
```

```cpp
#include "CoralCore.hpp"
#include "../Engine/Simulator.hpp"

CoralCore::CoralCore(const std::string &name, std::unique_ptr<CoralBehavioralEngine> engine)
    : SimObject(name), behavioral_engine(std::move(engine)) {}

void CoralCore::init() {
    simout("Coral NPU Core initialized.");
}

void CoralCore::startup() {
    simout("Coral NPU Core ready for network tasks.");
}

bool CoralCore::fetchAndExecute(InstructionTask task, std::function<void(const InstructionTask&)> on_complete) {
    if (is_busy) {
        simout("REJECTED Task #" + std::to_string(task.task_id) + " - Core BUSY");
        return false;
    }

    is_busy = true;
    simout("ACCEPTED Task #" + std::to_string(task.task_id) + " - Starting behavioral execution");

    // 1. Behavioral Execution (Functional math)
    behavioral_engine->execute(task);

    // 2. Compute timing cycles
    uint64_t cycles = calculateCycles(task);

    // 3. Schedule event in Inference-M-Sim
    pushEvent(cycles, [this, task, on_complete]() mutable {
        is_busy = false;
        completed_tasks++;
        simout("COMPLETED Task #" + std::to_string(task.task_id) + " at cycle " + std::to_string(Simulator::getCurrentCycle()));
        if (on_complete) {
            on_complete(task);
        }
    });

    return true;
}

uint64_t CoralCore::calculateCycles(const InstructionTask &task) const {
    switch (task.opcode) {
        case OpcodeType::SCALAR_ADD: return 1;
        case OpcodeType::VECTOR_MAC: return 4 + (task.vector_length / 16);
        case OpcodeType::VECTOR_RELU: return 2 + (task.vector_length / 32);
        default: return 1;
    }
}
```

---

### 4. `Makefile` [MODIFY]

Update [`Makefile`](file:///home/sjaguri/Inference-M-Sim/Makefile) to compile `CoralCore` and test binaries:

```diff
 SRCS := example.cpp \
         Core/DummyCore.cpp \
+        Core/CoralCore.cpp \
         Engine/SimObject.cpp \
         Engine/Simulator.cpp

+test: $(BUILD_DIR)/test_coral_core
+	./$(BUILD_DIR)/test_coral_core
```

---

## Verification Plan

### Automated Tests
We will build a dedicated test executable `tests/test_coral_core.cpp` and run it via `make test`.

1. **Test Core State Transitions (`IDLE` $\rightarrow$ `BUSY` $\rightarrow$ `IDLE`):**
   - Verify `fetchAndExecute()` accepts tasks when `IDLE` (`isBusy() == false`).
   - Verify `fetchAndExecute()` rejects subsequent tasks while `isBusy() == true`.
   - Verify `isBusy()` returns to `false` exactly when the scheduled event cycle elapses.

2. **Test Cycle Accuracy:**
   - Schedule a `VECTOR_MAC` task with `vector_length = 32` (expected cycles: 6).
   - Verify that the network callback fires at `Simulator::getCurrentCycle() == 6`.

3. **Test Functional Correctness:**
   - Send input data `{-5, 0, 10, -20, 15}` with opcode `VECTOR_RELU`.
   - Verify output data produced by behavioral engine is `{0, 0, 10, 0, 15}` upon completion callback.

4. **Test Back-to-Back Network Pipelining:**
   - Send Task 1 at cycle 0 (duration: 6 cycles).
   - In Task 1's completion callback at cycle 6, submit Task 2 (duration: 4 cycles).
   - Verify Task 2 completes at cycle 10 and total completed task count is 2.

### Manual Verification
- Run `./build/sim` or `./build/test_coral_core` and inspect simulation log outputs (`simout`) to verify cycle timestamps and state transitions.
