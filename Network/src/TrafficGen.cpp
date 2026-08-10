#include "TrafficGen.hpp"
#include "Flit.hpp"

TrafficGen::TrafficGen(const std::string &name, Router *target)
	: SimObject(name), target(target) {}

void TrafficGen::reset() {
	// Nothing to init
}

void TrafficGen::startup() {
	simout("TrafficGen starting and sending first flit");

	Flit f;
	f.id = 0;
	f.src_x = 0;
	f.src_y = 0;
	f.dst_x = 1;
	f.dst_y = 1;
	f.type = flitType::UNICAST;

	f.payload = 46;

	pushEvent(2, [this, f]() {
		simout("Injecting flit " + std::to_string(f.id) + " into network.");
		target->receiveFlit(f);
	});
}
