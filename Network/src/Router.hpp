#pragma once

#include "../../Engine/SimObject.hpp"
#include "Flit.hpp"
#include <queue>
#include <vector>

class Router : public SimObject {
	private:
		int x;
		int y;

		std::queue<Flit> flitQueue;

		Router *north = nullptr;
		Router *south = nullptr;
		Router *east = nullptr;
		Router *west = nullptr;

	public:
		Router(const std::string &name, int x, int y);

		void setNeighbors(Router *n, Router *s, Router *e, Router *w);

		void reset() override;
		void startup() override;

		// Puts the flit into the queue and signals that it arrived
		void receiveFlit(const Flit &flit);

		// The function which moves flits every cycle
		void processBuffer();

		void xy_route_fork(const Flit &f);
		void stream_fork(const Flit &f);
		void reduction_sync(const Flit &f);
};