#pragma once

#include <cstdint>
#include <functional>
#include <string>

struct Event {
		uint64_t scheduled_cycle;
		uint64_t seq_id;
		std::function<void()> callback;
		bool is_cancelled = false;
		std::string sim_object_name;

		// comparison operator for min-heap tie-breaking
		bool operator>(const Event &other) const {
			if (scheduled_cycle != other.scheduled_cycle) {
				return scheduled_cycle > other.scheduled_cycle;
			}
			return seq_id > other.seq_id;
		}
};

