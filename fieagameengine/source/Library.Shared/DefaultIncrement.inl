#include "DefaultIncrement.h"

namespace Library
{
	template<typename T>
	inline std::size_t DefaultIncrement<T>::operator()(const std::size_t /*size*/, const std::size_t capacity) const
	{
		return capacity;
	}
}

