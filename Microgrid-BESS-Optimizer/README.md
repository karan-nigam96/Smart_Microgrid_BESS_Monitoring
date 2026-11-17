# Smart Microgrid Power Flow & BESS Operational Monitor

A high-performance C++ simulation and monitoring framework engineered for hybrid renewable energy microgrids and **Battery Energy Storage Systems (BESS)**. This project models multi-node power network topologies using graph data structures and optimizes BESS charging/discharging schedules.

## Features
- **Asset Performance & Energy Management:** Models 24-hour operational cycles for Battery Energy Storage Systems (BESS), tracking state-of-charge (SoC), charging/discharging efficiency, and grid-import reduction.
- **System Monitoring & Fault Diagnostics:** Implements graph-based network topologies ($O(V + E)$ complexity) to simulate multi-node power flow, monitor transmission capacities, and isolate potential line overloads.
- **High-Performance C++:** Utilizes contiguous memory structures (`std::vector`) and optimized control loops for low-latency state estimation.

## Tech Stack
- **Language:** C++17
- **Build System:** CMake (Version 3.12+)

## Build Instructions
```bash
mkdir build && cd build
cmake ..
cmake --build .
./microgrid_sim
```
