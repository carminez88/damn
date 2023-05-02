#ifndef PACKETTYPE_H
#define PACKETTYPE_H

#include <cstdint>

namespace damn {

	enum class PacketType : uint8_t
	{
		undefined = 0,
		registration,
		heartbeat
	};

} // namespace damn

#endif // PACKETTYPE_H
