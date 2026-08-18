#pragma once

#include <systemc.h>
#include "Flit.hpp"
#include <queue>
#include <vector>

class Router : public sc_module {
    private:
        int x;
        int y;

        size_t bufferSize = 4;
        std::queue<Flit> flitQueue;

        int east_locked_by_id = - 1;
        int west_locked_by_id = - 1;
        int north_locked_by_id = - 1;
        int south_locked_by_id = - 1;

        Router* north = nullptr;
        Router* south = nullptr;
        Router* east = nullptr;
        Router* west = nullptr;

    public:
        sc_in<bool> clk;

        Router(sc_module_name name, int x, int y);

        void setNeighbors(Router* n, Router* s, Router* e, Router* w);

        void simout(const std::string& msg) const;

        // Puts the flit into the queue and signals that it arrived
        void receiveFlit(const Flit& flit);

        // The function which moves flits every cycle
        void processBuffer();

        // checks if a given router can accept a flit to its queue
        bool canAcceptFlit() const {
            return flitQueue.size() < bufferSize;
        }

        bool xy_route_fork(const Flit& f);
        void stream_fork(const Flit& f);
        void reduction_sync(const Flit& f);
};