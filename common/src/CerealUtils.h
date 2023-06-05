#pragma once

#include <spdlog/spdlog.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <sstream>
#include <optional>

namespace damn {

template <typename Object>
inline static std::optional<std::string> pack(const Object& obj)
{
	try {
		std::stringstream ss;
		cereal::BinaryOutputArchive oarchive(ss);

		oarchive(obj);

        return std::string{ ss.str() };

	} catch (const std::exception& e) {
		spdlog::error( "Impossible to pack object:", e.what() );
	}

	return {};
}

template <typename Object>
inline static std::optional<Object> unpack(const std::string& str)
{
	try {

		std::stringstream ss{ str };
		cereal::BinaryInputArchive iarchive(ss);

		Object obj;
		iarchive(obj);

        return std::make_optional( obj );

	} catch (const std::exception& e) {
		spdlog::error("Impossible to unpack object:", e.what());
	}

	return {};
}

} // namespace damn
