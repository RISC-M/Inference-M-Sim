#define private public  // C++ trick to allow our test to inspect the Router's internal flitQueue!
#include "../src/Router.hpp"
#undef private

#include "../src/NI.hpp"
#include "ni_test.hpp"
#include <iostream>
#include <cassert>

void test_unicast_address_extraction() {
    std::cout << "[TEST] Running NI Unicast Address Extraction Math..." << std::endl;
    
    NI test_ni("TestNI", nullptr, 0, 0); // No router needed for this pure math test
    
    int x_dst, y_dst, x_mask, y_mask;
    
    // Test 1: Address 0x100000 -> Should map to Node (0,1)
    // 0x100000 >> 20 = 1. binary: 01. Y=1 (bit 20), X=0 (bit 21).
    test_ni.extract_unicast_addr(0x100000, 0x000000, &x_dst, &y_dst, &x_mask, &y_mask);
    assert(x_dst == 0 && y_dst == 1);
    
    // Test 2: Address 0x350000 -> Should map to Node (1,1)
    // 0x300000 >> 20 = 3. binary: 11. X=1, Y=1.
    test_ni.extract_unicast_addr(0x350000, 0x000000, &x_dst, &y_dst, &x_mask, &y_mask);
    assert(x_dst == 1 && y_dst == 1);
    
    std::cout << "[TEST]   -> extract_unicast_addr passed!" << std::endl;
}

void test_axi_to_flit_translation() {
    std::cout << "[TEST] Running NI AXI-to-Flit Translation Pipeline..." << std::endl;
    
    // Setup a dummy router and NI attached to Tile (2, 3)
    Router dummy_router("DummyRouter", 2, 3);
    NI test_ni("TestNI", &dummy_router, 2, 3);
    
    // Create a dummy AXI transaction targeting Tile (0,1) which is base address 0x100000
    tlm::tlm_generic_payload trans;
    trans.set_address(0x100000);
    unsigned char dummy_data = 42;
    trans.set_data_ptr(&dummy_data);
    
    // Attach the custom sideband
    AWUserExtension* ext = new AWUserExtension;
    ext->addr_mask = 0;
    ext->collective_opcode = 0;
    trans.set_extension(ext);
    
    // Execute the transport (Simulating the Core sending a request)
    sc_time delay = SC_ZERO_TIME;
    test_ni.b_transport(trans, delay);
    
    // Verify the NI set the TLM success response
    assert(trans.get_response_status() == tlm::TLM_OK_RESPONSE);
    
    // Verify the Flit was injected into the Router
    assert(dummy_router.flitQueue.size() == 1);
    
    // Extract the Flit and verify all fields were mapped correctly
    Flit f = dummy_router.flitQueue.front();
    assert(f.src_x == 2);
    assert(f.src_y == 3);
    assert(f.dst_x == 0);
    assert(f.dst_y == 1);
    assert(f.payload == 42);
    assert(f.type == flitType::UNICAST);
    assert(f.state == flitState::SINGLE);
    
    // Cleanup
    trans.clear_extension(ext);
    delete ext;
    
    std::cout << "[TEST]   -> b_transport AXI to Flit translation passed!" << std::endl;
}

void run_ni_tests() {
    std::cout << "\n=== Starting NI Unit Tests ===\n";
    test_unicast_address_extraction();
    test_axi_to_flit_translation();
    std::cout << "=== All NI Unit Tests Passed ===\n\n";
}
