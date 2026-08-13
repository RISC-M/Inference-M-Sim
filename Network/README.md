# Inference-M Networking Simulator

## Overview
This directory contains the Network-on-Chip (NoC) simulator tailored for Direct Compute Access (DCA) workloads. It is designed to model custom hardware running RV32I + Zve32x vector extensions, tracing the cycle-by-cycle network behavior as payloads travel between tiles.

---

## Current Components
These components have been implemented to form the baseline routing mesh:

* **Flit Payload Structure**: The fundamental data packet traversing the network, carrying routing coordinates, traffic type information, and arbitrary data payloads.
* **Traffic Injector**: A workload generation module responsible for introducing point-to-point traffic into the network to simulate real-world traffic patterns.
* **Mesh Node (Router)**: The core switching element of the network, containing internal queuing and four directional pathways to neighboring tiles.
* **Deterministic XY Routing**: The baseline routing algorithm that resolves Unicast traffic paths by navigating X-coordinates before Y-coordinates, ensuring deadlock-free travel.

---

## Expected Components
These components will be implemented to support the advanced features required for Machine Learning workloads and Direct Compute Access:

* **Multicast Splitting (Stream Fork)**: A module designed to duplicate network traffic at branch points, allowing a single payload to reach multiple destinations efficiently.
* **Contention Arbitration**: Resolves port contention when multiple messages request the same output direction simultaneously.
* **Reduction Synchronization**: A stalling mechanism that waits for multiple corresponding partial sums to arrive from different directions before merging them.
* **LZC Hardware Arbitration**: A specialized arbiter used to prioritize and sort multiple ready reduction results attempting to leave the router.
* **Wide Reduction Port**: A dedicated high-bandwidth port specifically handling the merging of wide vector datapath elements.
* **DCA Offload Port**: The final hardware interface that redirects arriving network payloads directly into the local processor's execution unit, bypassing deep pipeline stalls.

---

## Change Log
- **v0.1.0** - MVP milestone achieved. Added baseline data packet structures, node routing logic, traffic injection, and a functional 2x2 mesh topology.

## Implementation Roadmap
### Phase 1: Foundation & Flow Control
- [ ] Add Backpressure and Flow Control (Wormhole Routing)
  - [ ] Implement buffer depth constraints
  - [ ] Add stall signals between routers
- [ ] Add support for separate `req`, `rsp`, and `wide` router ports

### Phase 2: The Edges (Network Interface)
- [ ] Add Network Interface (NI)
  - [ ] Implement address mask to X/Y mask translation

### Phase 3: Multicast Operations (One-to-Many)
- [ ] Add support for Multicast operations
  - [ ] Implement Stream Fork (duplicating packets)
  - [ ] Implement basic Reduction Arbiter on the `rsp` network to merge multicast responses

### Phase 4: Reduction Operations (Many-to-One)
- [ ] Add Support for Narrow Reduction Operations
  - [ ] Implement Sync Module (wait for all operands)
  - [ ] Add lightweight in-network ALUs (CollectB, LsbAnd)
  - [ ] Add Arbitration components for different reduction packets (LZC)

### Phase 5: Direct Compute Access (DCA)
- [ ] Differentiate Centralized vs Parallel Reduction
  - [ ] Replicate reduction logic per output for narrow networks
  - [ ] Centralize reduction logic for the wide network
- [ ] Add DCA Offload Port (Handing wide reductions to the core's FPU)
  