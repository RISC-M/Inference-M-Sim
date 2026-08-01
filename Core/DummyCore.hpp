#pragma once

#include "../Engine/SimObject.hpp"
#include <cstdint>
#include <string>

class DummyCore : public SimObject {
	private:
		int value = 0;
		uint64_t instruction;

	public:
		explicit DummyCore(const std::string &name = "DummyCore", uint64_t instruction = 10);

		// SimObject setup
		void init() override;
		void startup() override;

		// Modeling the core
		void doSomeWork();
};