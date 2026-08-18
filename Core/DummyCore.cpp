#include "DummyCore.hpp"
#include <iostream>

DummyCore::DummyCore(sc_module_name name, uint64_t instruction) 
    : sc_module(name), instruction(instruction) {
    value = 2; // Initialized value
    SC_THREAD(doSomeWork);
}

void DummyCore::simout(const std::string& msg) const {
    std::cout << "[Cycle " << (sc_time_stamp().value() / 1000) << "] : " << this->name() << " : " << msg << std::endl;
}

void DummyCore::doSomeWork() {
    simout("Starting core work loop...");
    simout("Initialized value as 2");
    
    while(true) {
        wait(instruction, SC_NS);
        value = value * 2;
        simout("doing dummy core stuff, value = " + std::to_string(value));
    }
}
