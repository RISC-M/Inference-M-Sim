#include "Core/DummyCore.hpp"
#include "Engine/Simulator.hpp"

int main() {
	DummyCore core0("Core 0", 10);
	DummyCore core1("Core 1", 20);

	Simulator::run();
	return 0;
}
