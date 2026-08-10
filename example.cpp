#include "Engine/Simulator.hpp"
#include "Network/src/Router.hpp"
#include "Network/src/TrafficGen.hpp"
#include <iostream>

int main() {
    std::cout << "--- Inference-M Networking Sim MVP ---\n";

    Router r00("Router_0_0", 0, 0);
    Router r10("Router_1_0", 1, 0);
    Router r01("Router_0_1", 0, 1);
    Router r11("Router_1_1", 1, 1);

    r00.setNeighbors(&r01, nullptr, &r10, nullptr);
    r10.setNeighbors(&r11, nullptr, nullptr, &r00);
    r01.setNeighbors(nullptr, &r00, &r11, nullptr);
    r11.setNeighbors(nullptr, &r10, nullptr, &r01);

    TrafficGen gen("Injector", &r00);

    Simulator::run();
    
    return 0;
}
