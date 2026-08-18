#pragma once
#include <systemc.h>
#include <string>

class DummyCore : public sc_module {
	private:
		int value = 0;
		uint64_t instruction;

	public:
		sc_in<bool> clk;

		DummyCore(sc_module_name name, uint64_t instruction = 10);

		void simout(const std::string& msg) const;

		// Modeling the core
		void doSomeWork();
};