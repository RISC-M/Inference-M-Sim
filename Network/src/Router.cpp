#include "Router.hpp"
#include "../../Engine/Simulator.hpp"
#include "Flit.hpp"
#include <cstddef>
#include <iterator>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

Router::Router(const std::string& name, int x, int y) : SimObject(name), x(x), y(y) {}

void Router::setNeighbors(Router* n, Router* s, Router* e, Router* w) {
    north = n;
    south = s;
    east = e;
    west = w;
}

// for state initialization
void Router::init() { 
    
}

// Push the initial buffer processing event
void Router::startup() { 
    pushEvent(1, [this]() { processBuffer(); });
}

// Push the flit to the queue and log it
void Router::receiveFlit(const Flit& flit) { 
    flitQueue.push(flit);

    simout("Router " + name + " received flit " + to_string(flit.id));
}

// Process the flit queue and schedule the next buffer processing event
void Router::processBuffer() { 
    if(!flitQueue.empty()) {
        Flit f = flitQueue.front();
        flitQueue.pop();
        
        if(f.dst_x == x && f.dst_y == y) {
            simout("Flit " + to_string(f.id) + " reached its destination at (" + to_string(f.dst_x) + ", " + to_string(f.dst_y) + ")");
        } else {
            xy_route_fork(f);
        }
    }

    if (Simulator::getCurrentCycle() < 100) {
        pushEvent(1, [this]() { processBuffer(); });
    }
}

void Router::xy_route_fork(const Flit& f) {
    Router* nextRouter = nullptr;
    string dir = "";
    
    if(f.dst_x > x) {
        nextRouter = east;
        dir = "east";
    } else if(f.dst_x < x) {
        nextRouter = west;
        dir = "west";
    } else if(f.dst_y > y) {
        nextRouter = north;
        dir = "north";
    } else if(f.dst_y < y) {
        nextRouter = south;
        dir = "south";
    }

    if(nextRouter) {
        simout("Routing flit " + to_string(f.id) + " to " + dir);

        pushEvent(1, [this, nextRouter, f]() { nextRouter->receiveFlit(f); });
    } else {
        simout("ERROR: No neighbor exists in direction needed for flit " + to_string(f.id));
    }   
}