
#include "Simulator.hpp"
#include "SimObject.hpp"
#include <iostream>

using namespace std;

// Static memory for simulation
uint64_t Simulator::current_cycle = 0;
uint64_t Simulator::next_seq_id = 0;
EventQueue Simulator::event_queue;
vector<SimObject *> Simulator::sim_objects;
bool Simulator::running = false;

uint64_t Simulator::getCurrentCycle() {
	return current_cycle;
}

void Simulator::registerObject(SimObject *obj) {
	if (obj) {
		sim_objects.push_back(obj);
	}
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
	cout << "=== Starting Simulation ===\n"
		 << endl;

	// Phase 1: Automatically initialize all registered hardware components
	for (SimObject *obj : sim_objects) {
		obj->init();
	}

	// Phase 2: Automatically startup all registered hardware components
	for (SimObject *obj : sim_objects) {
		obj->startup();
	}

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
	cout << "\n=== Simulation Finished at Cycle " << current_cycle << " ===" << endl;
}

void Simulator::stop() {
	running = false;
}
