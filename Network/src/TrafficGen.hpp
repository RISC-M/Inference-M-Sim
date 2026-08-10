#pragma once

#include "../../Engine/SimObject.hpp"
#include "Router.hpp"

class TrafficGen : SimObject {
	private:
		Router *target;

	public:
		TrafficGen(const std::string &name, Router *target);

		void reset() override;
		void startup() override;
};