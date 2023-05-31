#pragma once

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

		return std::make_optional( ss.str() );

	} catch (const std::exception&) {

	}

	return std::nullopt;
}

template <typename Object>
inline static std::optional<Object> unpack(const std::string& str)
{
	try {

		std::stringstream ss{ str };
		cereal::BinaryInputArchive iarchive(ss);

		Object obj;
		iarchive(obj);

		return std::make_optional(obj);

	} catch (const std::exception&) {

	}

	return std::nullopt;
}

} // namespace damn