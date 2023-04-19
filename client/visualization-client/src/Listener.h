#pragma once
#include <qobjectdefs.h>
#include <string>

namespace damn {

	enum class damn_packet_type : uint8_t
	{
		undefined = 0,
		registration,
		heartbeat
	};

	struct damn_packet_t { virtual ~damn_packet_t() = default; };

	struct damn_registration_packet_t : public damn_packet_t
	{
		std::string source;
		std::string userID;
		std::string details;
		std::time_t timestamp;
		const auto packetType{ damn_packet_type::registration };
	};

class DAMNListener
{
public:
	explicit DAMNListener() = default;

Q_SIGNALS:
	void notifyRegistrationPacket(damn_registration_packet_t);
	
	
private:
};

} // namespace damn