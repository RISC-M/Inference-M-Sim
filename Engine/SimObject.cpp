#include "SimObject.hpp"
#include <iostream>

SimObject::SimObject(const std::string &name) : name(name) {}

const std::string &SimObject::getName() const { return name; }

void SimObject::debug(const std::string &message) const {
	std::cout << "[" << name << "]" << message << std::endl;
}
