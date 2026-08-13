#pragma once

#include "../../Engine/SimObject.hpp"
#include "Flit.hpp"
#include <queue>
#include <vector>

class Router : public SimObject {
    private:
        int x;
        int y;

        size_t bufferSize;
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
        Router(const std::string& name, int x, int y);

        void setNeighbors(Router* n, Router* s, Router* e, Router* w);

        void init() override;
        void startup() override;

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