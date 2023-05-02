#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <ctime>
#include <optional>

#include "IPacket.h"
#include "PacketType.h"

namespace damn {

	class Packet : public IPacket
	{
	public:

	private:
		std::string m_source{ "" };
		std::time_t m_timestamp;
		PacketType m_packetType{ PacketType::undefined };

		std::optional<std::string> m_userID{ std::nullopt };
		std::optional<std::string> m_details{ std::nullopt };
	};

} // namespace damn

#endif // PACKET_H
