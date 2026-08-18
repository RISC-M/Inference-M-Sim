#include "NI.hpp"

tlm::tlm_extension_base* AWUserExtension::clone() const {
    AWUserExtension* ext = new AWUserExtension;
    ext->addr_mask = this->addr_mask;
    ext->collective_opcode = this->collective_opcode;
    return ext;
}

void AWUserExtension::copy_from(tlm::tlm_extension_base const &ext) {
    const AWUserExtension& custom_ext = static_cast<const AWUserExtension&>(ext);
    this->addr_mask = custom_ext.addr_mask;
    this->collective_opcode = custom_ext.collective_opcode;
}

// b_transport callback for the AXI target socket
NI::NI(sc_module_name name, Router* router, int x, int y) : sc_module(name), local_router(router), src_x(x), src_y(y) {
    axi_target_socket.register_b_transport(this, &NI::b_transport);
}

void NI::extract_unicast_addr(uint64_t addr, int mask, int *x_dst, int *y_dst, int *x_mask, int *y_mask) {
    const int OFFSET_BITS = 20; // this is assumed from the 1 MB per core

    *y_dst = (addr >> OFFSET_BITS) & 0x1;       // selects bit 20 for Y
    *x_dst = (addr >> (OFFSET_BITS + 1)) & 0x1; // selects bit 21 for X

    *y_mask = (mask >> OFFSET_BITS) & 0x1;      // selects bit 20 of the mask
    *x_mask = (mask >> (OFFSET_BITS + 1)) & 0x1;// selects bit 21 of the mask
}

// define the transaction between the core and the NI
void NI::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    uint64_t addr = trans.get_address();
    unsigned char* data = trans.get_data_ptr();

    AWUserExtension* awuser_ext = nullptr;
    trans.get_extension(awuser_ext);

    if(awuser_ext == nullptr) {
        return;
    }

    // target address extractions:
    // This is heavily dependent on:
    // 1. the spm sizing (core side decision)
    // 2. the mesh sizing (ie. # of cores)
    // For the time being, we assume 1 MB per core and 2x2 mesh
    // We will also assume Unicast for now

    int target_x;
    int target_y;
    int target_x_mask;
    int target_y_mask;
    extract_unicast_addr(addr, awuser_ext->addr_mask, &target_x, &target_y, &target_x_mask, &target_y_mask);

    Flit f;
    f.dst_x = target_x;
    f.dst_y = target_y;
    
    f.id = packet_id_ctr++;
    f.payload = *data;
    
    f.src_x = this->src_x;
    f.src_y = this->src_y;

    f.type = flitType::UNICAST; //hardcoded for now
    f.state = flitState::SINGLE;

    local_router->receiveFlit(f);

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}