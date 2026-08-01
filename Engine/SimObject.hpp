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
		virtual void init() {}
		virtual void startup() {}

		// Event scheduling helpers
		void pushEvent(uint64_t delay_cycles, std::function<void()> cb,
					   const std::string &evt_name = "");
		void scheduleEvent(uint64_t target_cycle, std::function<void()> cb,
						   const std::string &evt_name = "");

		// Debug logging hselper
		void debug(const std::string &message) const;
};