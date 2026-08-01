#include "SimObject.hpp"
#include "Simulator.hpp"
#include <iostream>

using namespace std;

SimObject::SimObject(const string &name) : name(name) {}

const string &SimObject::getName() const { return name; }

void SimObject::pushEvent(uint64_t delay_cycles, function<void()> cb) {
	Simulator::pushEvent(delay_cycles, cb, name);
}

void SimObject::scheduleEvent(uint64_t target_cycle, function<void()> cb) {
	Simulator::scheduleEvent(target_cycle, cb, name);
}

void SimObject::debug(const string &message) const {
	cout << "[Cycle " << Simulator::getCurrentCycle() << "] [" << name << "] " << message << endl;
}


