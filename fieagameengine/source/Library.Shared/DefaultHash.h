#pragma once

#include <string>

namespace Library
{
	std::size_t AdditiveHash(const std::uint8_t* key, std::size_t length);

	template <typename T>
	struct DefaultHash final
	{
		std::size_t operator()(const T& key) const;

	};

	template <>
	struct DefaultHash<char*> final
	{
		std::size_t operator()(const char* const key) const;
	};
	
	template <>
	struct DefaultHash<const char*> final
	{
		std::size_t operator()(const char* const key) const;
	};

	template <>
	struct DefaultHash<const char* const> final
	{
		std::size_t operator()(const char* const key) const;
	};

	template <>
	struct DefaultHash<char* const> final
	{
		std::size_t operator()(const char* const key) const;
	};

	template <>
	struct DefaultHash<std::string> final
	{
		std::size_t operator()(const std::string& key) const;
	};

	template <>
	struct DefaultHash<const std::string> final
	{
		std::size_t operator()(const std::string& key) const;
	};
}

#include "DefaultHash.inl"