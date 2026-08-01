#include "DummyCore.hpp"
#include "../Engine/Simulator.hpp"
#include <iostream>
#include <string>

using namespace std;

DummyCore::DummyCore(const string &name, uint64_t instruction)
	: SimObject(name), instruction(instruction) {}

void DummyCore::init() {
	value = 2;
	simout("Initialized value as 2");
}

void DummyCore::startup() {
	simout("Starting core work loop...");
	pushEvent(instruction, [this]() { doSomeWork(); });
}

void DummyCore::doSomeWork() {
	value = value * 2;
	simout("doing dummy core stuff, value = " + to_string(value));

	if (Simulator::getCurrentCycle() < 100) {
		pushEvent(instruction, [this]() { doSomeWork(); });
	}
}
