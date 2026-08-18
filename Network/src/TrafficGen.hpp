#pragma once

#include <systemc.h>
#include "Router.hpp"

class TrafficGen : public sc_module {
	private:
		Router *target;

	public:
		sc_in<bool> clk;

		TrafficGen(sc_module_name name, Router *target);

		void simout(const std::string& msg) const;
		void run_traffic();
};