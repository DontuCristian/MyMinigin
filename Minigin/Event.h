#pragma once
#include <cstdint>
#include <any>
#include <string>
#include "Hash.h"


namespace dae
{
	using EventId = uint32_t;
	using EventArgs = std::any;

	struct Event
	{
		const EventId id;
		static const uint8_t MAX_ARGS = 8;
		uint8_t nbArgs = 0;
		EventArgs args[MAX_ARGS];

		Event(EventId _id) :id{ _id } {}

	};
}