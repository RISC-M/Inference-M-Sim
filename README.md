# Inference-M-Sim 

Work in progress LLM inference accelerator chip, taking advantage of DCA and NoC inspired by https://mlsys.org/virtual/2026/oral/3804

---

## Build & Execution Instructions

### Commands

* **Build the simulation binary**:
  ```bash
  make
  ```
  Compiles all source files into object files under `build/` and outputs the executable binary `build/sim`.

* **Run the simulation**:
  ```bash
  make run
  ```
  Builds the project (if needed) and executes the simulation binary `./build/sim`.

* **Clean build artifacts**:
  ```bash
  make clean
  ```
  Removes the `build/` directory and all generated object/binary files.