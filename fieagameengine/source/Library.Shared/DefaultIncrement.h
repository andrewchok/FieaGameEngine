#pragma once

#include <cstddef>

namespace Library
{
	template <typename T>
	struct DefaultIncrement final
	{
		std::size_t operator()(const std::size_t size, const std::size_t capacity) const;
	};

}

#include "DefaultIncrement.inl"