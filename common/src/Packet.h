#pragma once

#include <string>
#include <ctime>
#include "EnumConverter.h"

namespace damn {
	
enum class PacketType : uint8_t {
	Undefined = 0,
	Registration,
	Heartbeat,
	Disconnection,
};

struct Packet 
{
	int64_t timestamp;
	std::string sourceID;
	std::string userID;
	std::string details;
	PacketType type { PacketType::Undefined };

	std::string toString()
	{
		std::stringstream ss;

		ss << "\n";
		ss << "[Timestamp] " << timestamp << "\n";
		ss << "[Source ID] " << sourceID << "\n";
		ss << "[User ID] " << userID << "\n";
		ss << "[Details] " << details << "\n";
		ss << "[Packet type] " << +timestamp << "\n";

		return ss.str();
	}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(timestamp);
		ar(sourceID);
		ar(userID);
		ar(details);
		ar(type);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(timestamp);
		ar(sourceID);
		ar(userID);
		ar(details);
		ar(type);
	}
};

} // namespace damn

