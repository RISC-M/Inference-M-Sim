
#include "Simulator.hpp"
#include <iostream>

using namespace std;

// Static memory for simulation
uint64_t Simulator::current_cycle = 0;
uint64_t Simulator::next_seq_id = 0;
EventQueue Simulator::event_queue;
bool Simulator::running = false;

uint64_t Simulator::getCurrentCycle() {
	return current_cycle;
}

void Simulator::scheduleEvent(uint64_t target_cycle, function<void()> cb, const string &obj_name) {

	if (target_cycle < current_cycle) {
		cerr << "[Warning] Attempted to schedule event in the past! Target: "
			 << target_cycle << ", Current: " << current_cycle << endl;
		return;
	}
	event_queue.push(Event{target_cycle, next_seq_id++, cb, false, obj_name});
}

void Simulator::pushEvent(uint64_t delay_cycles, function<void()> cb, const string &obj_name) {
	scheduleEvent(current_cycle + delay_cycles, cb, obj_name);
}

void Simulator::run(uint64_t max_cycles) {
	running = true;
	while (running && !event_queue.empty()) {
		Event event = event_queue.top();
		event_queue.pop();

		if (event.scheduled_cycle > max_cycles) {
			break;
		}

		current_cycle = event.scheduled_cycle;

		if (!event.is_cancelled && event.callback) {
			event.callback();
		}
	}
	running = false;
}

void Simulator::stop() {
	running = false;
}
