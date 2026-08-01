#pragma once

#include "Event.hpp"
#include <cstdint>
#include <functional>
#include <queue>
#include <string>
#include <vector>

using EventQueue = std::priority_queue<
	Event,				// 1. Element Type
	std::vector<Event>, // 2. Internal Memory Container
	std::greater<Event> // 3. Min-Heap Comparator
	>;

class SimObject;

class Simulator {
	private:
		static uint64_t current_cycle;
		static uint64_t next_seq_id;
		static EventQueue event_queue;
		static std::vector<SimObject *> sim_objects;
		static bool running;

	public:
		static uint64_t getCurrentCycle();
		static void registerObject(SimObject *obj);


		static void scheduleEvent(uint64_t target_cycle, std::function<void()> cb, const std::string &obj_name = "");
		static void pushEvent(uint64_t delay_cycles, std::function<void()> cb, const std::string &obj_name = "");

		static void run(uint64_t max_cycles = UINT64_MAX);
		static void stop();
};