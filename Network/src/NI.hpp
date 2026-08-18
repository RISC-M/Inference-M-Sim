/* Translation Schema:
    1. Outgoing (AXI -> NoC/Flit)
    input args:
    - Address
    - AWUSER Sideband
        - addr mask
        - collective opcode
    - Payload

    output args:
    - dst
    - x mask
    - y mask
    - opcode
    - Payload

    2. Incoming (NoC/Flit -> AXI)
    input args:
    - src
    - x mask
    - y mask
    - Payload

    output args:
    - Address
    - AWUSER Sideband
        - addr mask
        - collective opcode
    - Payload
 */

#pragma once
#include <cstdint>
#include <sys/types.h>
#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include "Router.hpp"

struct AWUserExtension : public tlm::tlm_extension<AWUserExtension> {
    int addr_mask;
    int collective_opcode;

    virtual tlm_extension_base* clone() const override;
    virtual void copy_from(tlm_extension_base const &ext) override;
};

class NI : public sc_module {
public:
    int src_x, src_y;
    int packet_id_ctr = 0;
    
    // setup clock for the NI
    sc_in<bool> clk;
    
    // Setup the core->NI interface socket
    tlm_utils::simple_target_socket<NI> axi_target_socket;

    Router* local_router;

    // b_transport callback for the AXI target socket
    NI(sc_module_name name, Router* router, int x, int y);

    void extract_unicast_addr(uint64_t addr, int mask, int *x_dst, int *y_dst, int *x_mask, int *y_mask);

    // define the transaction between the core and the NI
    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay);
};
