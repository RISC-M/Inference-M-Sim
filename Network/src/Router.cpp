#include "Router.hpp"
#include "Flit.hpp"
#include <cstddef>
#include <iterator>
#include <mutex>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

Router::Router(sc_module_name name, int x, int y) : sc_module(name), x(x), y(y) {
    SC_METHOD(processBuffer);
    sensitive << clk.pos();
}

void Router::setNeighbors(Router* n, Router* s, Router* e, Router* w) {
    north = n;
    south = s;
    east = e;
    west = w;
}

void Router::simout(const std::string& msg) const {
    std::cout << "[Cycle " << (sc_time_stamp().value() / 1000) << "] : " << this->name() << " : " << msg << std::endl;
}

// Push the flit to the queue and log it
void Router::receiveFlit(const Flit& flit) { 
    simout("Router " + std::string(this->name()) + " received flit " + to_string(flit.id));
    flitQueue.push(flit);
}

// Process the flit queue and schedule the next buffer processing event
void Router::processBuffer() { 
    if(!flitQueue.empty()) {
        Flit f = flitQueue.front();

        // 1. Check if it's at its destination FIRST
        if(f.dst_x == x && f.dst_y == y) {
            simout("Flit " + to_string(f.id) + " reached its destination at (" + to_string(f.dst_x) + ", " + to_string(f.dst_y) + ")");
            flitQueue.pop(); 
        } 
        // 2. If it's not at its destination, try to route it
        else {
            if(xy_route_fork(f)) {
                flitQueue.pop(); // Pop if routing was successful
            }
        }
    }
}

bool Router::xy_route_fork(const Flit& f) {
    Router* nextRouter = nullptr;
    int* targetLock = nullptr;

    if(f.dst_x > x) {
        nextRouter = east;
        targetLock = &east_locked_by_id;
    } else if (f.dst_x < x) {
        nextRouter = west;
        targetLock = &west_locked_by_id;
    } else if (f.dst_y > y) {
        nextRouter = north;
        targetLock = &north_locked_by_id;
    } else if (f.dst_y < y) {
        nextRouter = south;
        targetLock = &south_locked_by_id;
    }

    if(nextRouter && targetLock) {
        if(f.state == flitState::HEAD || f.state == flitState::SINGLE) {
            if(*targetLock == -1) *targetLock = f.id;
            else return false;
        } else if(f.state == flitState::BODY || f.state == flitState::TAIL) {
            if(*targetLock != f.id) {
                return false;
            }
        }

        if(!nextRouter->canAcceptFlit()) return false;
        
        // In SystemC, we can just push to their queue. In a purely cycle-accurate model, 
        // we might delay this, but for now we call their receiveFlit directly.
        nextRouter->receiveFlit(f);
    
        if (f.state == flitState::TAIL || f.state == flitState::SINGLE) {
            *targetLock = -1;
        }
        return true;
    } 

    return false;
}