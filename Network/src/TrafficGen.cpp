#include "TrafficGen.hpp"
#include "Flit.hpp"

TrafficGen::TrafficGen(sc_module_name name, Router* target)
    : sc_module(name), target(target) {
    SC_THREAD(run_traffic);
}

void TrafficGen::simout(const std::string& msg) const {
    std::cout << "[Cycle " << (sc_time_stamp().value() / 1000) << "] : " << this->name() << " : " << msg << std::endl;
}

void TrafficGen::run_traffic() {
    simout("TrafficGen starting and sending first flit");

    Flit f;
    f.id = 0;
    f.src_x = 0;
    f.src_y = 0;
    f.dst_x = 1;
    f.dst_y = 1;
    f.type = flitType::UNICAST;
    f.state = flitState::SINGLE;

    f.payload = 46;
    
    // Wait for 2 clock cycles before injecting
    wait(2, SC_NS);

    simout("Injecting flit " + std::to_string(f.id) + " into network.");
    target->receiveFlit(f); 
}

