#pragma once

#include <cstdint>
#include <functional>
#include <string>

class SimObject {
	protected:
		std::string name;

	public:
		explicit SimObject(const std::string &name);
		virtual ~SimObject() = default;

		const std::string &getName() const;

		// Hardware lifcycle functions
		virtual void reset() {}
		virtual void startup() {}

		// Event scheduling helpers
		void pushEvent(uint64_t delay_cycles, std::function<void()> cb);
		void scheduleEvent(uint64_t target_cycle, std::function<void()> cb);

		// Logging helper
		void simout(const std::string &message) const;

};