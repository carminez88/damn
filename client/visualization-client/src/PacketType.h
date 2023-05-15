#ifndef PACKETTYPE_H
#define PACKETTYPE_H

#include <cstdint>

namespace damn {

	enum class PacketType : uint8_t
	{
		Undefined = 0,
		Registration,
		Heartbeat
	};

} // namespace damn

#endif // PACKETTYPE_H
