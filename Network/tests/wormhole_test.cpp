#include "wormhole_test.hpp"
#include "../src/Router.hpp"
#include "../src/Flit.hpp"
#include <systemc.h>
#include <iostream>
#include <string>

// A custom TrafficGen that injects a 3-flit worm (HEAD, BODY, TAIL)
class WormholeTrafficGen : public sc_module {
private:
    Router* target;
public:
    sc_in<bool> clk;

    WormholeTrafficGen(sc_module_name name, Router* target) : sc_module(name), target(target) {
        SC_THREAD(run_traffic);
    }

    void simout(const std::string& msg) const {
        std::cout << "[Cycle " << (sc_time_stamp().value() / 1000) << "] : " << this->name() << " : " << msg << std::endl;
    }

    void run_traffic() {
        simout("WormholeTrafficGen starting up...");

        Flit head;
        head.id = 99; head.src_x = 0; head.src_y = 0; head.dst_x = 1; head.dst_y = 0;
        head.type = flitType::UNICAST; head.state = flitState::HEAD;
        
        Flit body;
        body.id = 99; body.src_x = 0; body.src_y = 0; body.dst_x = 1; body.dst_y = 0;
        body.type = flitType::UNICAST; body.state = flitState::BODY;

        Flit tail;
        tail.id = 99; tail.src_x = 0; tail.src_y = 0; tail.dst_x = 1; tail.dst_y = 0;
        tail.type = flitType::UNICAST; tail.state = flitState::TAIL;

        wait(2, SC_NS);
        simout("Injecting HEAD flit " + std::to_string(head.id));
        target->receiveFlit(head); 

        wait(1, SC_NS);
        simout("Injecting BODY flit " + std::to_string(body.id));
        target->receiveFlit(body); 

        wait(1, SC_NS);
        simout("Injecting TAIL flit " + std::to_string(tail.id));
        target->receiveFlit(tail); 
    }
};

// We will not run this test automatically in sc_main because SystemC does not support multiple sc_start runs gracefully 
// without complex reset logic. To run this, replace the objects in sc_main with this test.
