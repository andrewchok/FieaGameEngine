#include "DefaultEquality.h"

namespace Library
{
	template<typename T>
	inline const bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}

	inline std::size_t DefaultEquality<char*>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline std::size_t DefaultEquality<const char*>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline std::size_t DefaultEquality<char* const>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline std::size_t DefaultEquality<const char* const>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
}