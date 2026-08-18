#include "Network/src/Router.hpp"
#include "Network/src/TrafficGen.hpp"
#include "Network/tests/wormhole_test.hpp"
#include "Network/tests/ni_test.hpp"
#include <systemc.h>
#include <iostream>

int sc_main(int argc, char* argv[]) {
    std::cout << "--- Inference-M Networking Sim MVP (SystemC) ---\n";
    std::cout << "=== Starting Simulation ===\n\n";

    run_ni_tests();

    sc_clock clk("clk", 1, SC_NS);

    Router r00("Router_0_0", 0, 0);
    Router r10("Router_1_0", 1, 0);
    Router r01("Router_0_1", 0, 1);
    Router r11("Router_1_1", 1, 1);

    r00.clk(clk);
    r10.clk(clk);
    r01.clk(clk);
    r11.clk(clk);

    r00.setNeighbors(&r01, nullptr, &r10, nullptr);
    r10.setNeighbors(&r11, nullptr, nullptr, &r00);
    r01.setNeighbors(nullptr, &r00, &r11, nullptr);
    r11.setNeighbors(nullptr, &r10, nullptr, &r01);

    TrafficGen gen("Injector", &r00);
    gen.clk(clk);

    // Run the main simulation
    sc_start(100, SC_NS);
    
    std::cout << "\n=== Simulation Finished at Cycle 100 ===\n\n";

    // Run wormhole test
    // run_wormhole_test(); // Commented out to prevent double execution in same simulation context

    return 0;
}