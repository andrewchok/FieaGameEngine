#include "DefaultHash.h"

namespace Library
{
	template<typename T>
	inline std::size_t DefaultHash<T>::operator()(const T& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&key);
		return AdditiveHash(data, sizeof(T));
	}

	inline std::size_t DefaultHash<char*>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline std::size_t DefaultHash<const char*>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline std::size_t DefaultHash<char* const>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline std::size_t DefaultHash<const char* const>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline std::size_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length());
	}

	inline std::size_t DefaultHash<const std::string>::operator()(const std::string& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length());
	}

	inline std::size_t AdditiveHash(const uint8_t* key, size_t length)
	{
		std::size_t hash = 0;
		for (std::size_t i = 0; i < length; ++i)
		{
			hash += 17 * static_cast<std::size_t>(key[i]);
		}

		return hash;
	}

}