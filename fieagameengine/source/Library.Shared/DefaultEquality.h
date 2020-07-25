#pragma once

#include <string>

namespace Library
{
	template <typename T>
	struct DefaultEquality final
	{
		const bool operator()(const T& lhs, const T& rhs) const;
	};

	template <>
	struct DefaultEquality<char*> final
	{
		std::size_t operator()(const char* const lhs, const char* const rhs) const;
	};

	template <>
	struct DefaultEquality<const char*> final
	{
		std::size_t operator()(const char* const lhs, const char* const rhs) const;
	};

	template <>
	struct DefaultEquality<char* const> final
	{
		std::size_t operator()(const char* const lhs, const char* const rhs) const;
	};

	template <>
	struct DefaultEquality<const char* const> final
	{
		std::size_t operator()(const char* const lhs, const char* const rhs) const;
	};

}

#include "DefaultEquality.inl"