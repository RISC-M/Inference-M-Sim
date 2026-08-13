#include "wormhole_test.hpp"
#include "../../Engine/Simulator.hpp"
#include "../src/Router.hpp"
#include "../src/Flit.hpp"
#include <iostream>
#include <string>

// A custom TrafficGen that injects a 3-flit worm (HEAD, BODY, TAIL)
class WormholeTrafficGen : public SimObject {
private:
    Router* target;
public:
    WormholeTrafficGen(const std::string &name, Router* target) : SimObject(name), target(target) {}

    void init() override {}

    void startup() override {
        simout("WormholeTrafficGen starting up...");

        // Inject HEAD at cycle 2
        pushEvent(2, [this]() {
            Flit head;
            head.id = 99;
            head.src_x = 0; head.src_y = 0;
            head.dst_x = 1; head.dst_y = 0;
            head.type = flitType::UNICAST;
            head.state = flitState::HEAD;
            simout("Injecting HEAD flit " + std::to_string(head.id));
            target->receiveFlit(head); 
        });

        // Inject BODY at cycle 3
        pushEvent(3, [this]() {
            Flit body;
            body.id = 99;
            body.src_x = 0; body.src_y = 0;
            body.dst_x = 1; body.dst_y = 0;
            body.type = flitType::UNICAST;
            body.state = flitState::BODY;
            simout("Injecting BODY flit " + std::to_string(body.id));
            target->receiveFlit(body); 
        });

        // Inject TAIL at cycle 4
        pushEvent(4, [this]() {
            Flit tail;
            tail.id = 99;
            tail.src_x = 0; tail.src_y = 0;
            tail.dst_x = 1; tail.dst_y = 0;
            tail.type = flitType::UNICAST;
            tail.state = flitState::TAIL;
            simout("Injecting TAIL flit " + std::to_string(tail.id));
            target->receiveFlit(tail); 
        });
    }
};

void run_wormhole_test() {
    std::cout << "\n--- Running Wormhole Test ---\n";
    Simulator::reset(); // Clear old events and start from cycle 0

    Router r00("Router_0_0", 0, 0);
    Router r10("Router_1_0", 1, 0);

    r00.setNeighbors(nullptr, nullptr, &r10, nullptr);
    r10.setNeighbors(nullptr, nullptr, nullptr, &r00);

    WormholeTrafficGen gen("Worm_Injector", &r00);

    Simulator::run();
}
