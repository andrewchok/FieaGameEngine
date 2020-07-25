#include "vector.h"

#include <cassert>
#include <algorithm>

namespace Library
{
#pragma region Iterator

	template<typename T>
	inline Vector<T>::Iterator::Iterator(Vector<T>* owner, size_t index) :
		mOwner(owner), mIndex(index)
	{
	}

	template<typename T>
	inline const bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mIndex == rhs.mIndex);
	}

	template<typename T>
	inline const bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<typename T>
	inline const bool Vector<T>::Iterator::operator<(const Iterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return mIndex < rhs.mIndex;
	}

	template<typename T>
	inline const bool Vector<T>::Iterator::operator>(const Iterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return mIndex > rhs.mIndex;
	}

	template<typename T>
	inline const bool Vector<T>::Iterator::operator<=(const Iterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return (operator<(rhs)) || (operator==(rhs));
	}

	template<typename T>
	inline const bool Vector<T>::Iterator::operator>=(const Iterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return (operator>(rhs)) || (operator==(rhs));
	}

	// pre-increment
	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner pointing to nullptr");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Incrementing Iterator to out of bounds");
		}

		++mIndex;

		return *this;
	}

	// post-increment
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();

		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner pointing to nullptr");
		}

		if (mIndex == 0)
		{
			throw std::runtime_error("Decrementing Iterator to out of bounds");
		}

		--mIndex;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		operator--();

		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator+(const size_t rhs) 
	{
		if ((mIndex + rhs) >= mOwner->mSize)
		{
			throw std::runtime_error("Accessing Iterator to out of bounds");
		}

		return Iterator(mOwner, mIndex + rhs);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator-(const size_t rhs) 
	{
		if (mIndex < rhs)
		{
			throw std::runtime_error("Accessing Iterator to out of bounds");
		}

		return Iterator(mOwner, mIndex - rhs);
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner is nullptr.");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator is trying to dereference out of bounds.");
		}

		return (*mOwner)[mIndex];
	}

	template<typename T>
	inline T* Vector<T>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner is nullptr.");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator is trying to dereference out of bounds.");
		}

		return &(*mOwner)[mIndex];
	}

#pragma endregion Iterator

#pragma region ConstIterator

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector<T>* owner, size_t index) :
		mOwner(owner), mIndex(index)
	{
	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& it) :
		mOwner(it.mOwner), mIndex(it.mIndex)
	{
	}

	template<typename T>
	inline const bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mIndex == rhs.mIndex);
	}

	template<typename T>
	inline const bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<typename T>
	inline const bool Vector<T>::ConstIterator::operator<(const ConstIterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return mIndex < rhs.mIndex;
	}

	template<typename T>
	inline const bool Vector<T>::ConstIterator::operator>(const ConstIterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return mIndex > rhs.mIndex;
	}

	template<typename T>
	inline const bool Vector<T>::ConstIterator::operator<=(const ConstIterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return (operator<(rhs)) || (operator==(rhs));
	}

	template<typename T>
	inline const bool Vector<T>::ConstIterator::operator>=(const ConstIterator& rhs) const
	{
		if (mOwner != rhs.mOwner)
		{
			throw std::runtime_error("Owners do not match");
		}
		return (operator>(rhs)) || (operator==(rhs));
	}

	// pre-increment
	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner pointing to nullptr");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Incrementing Iterator to out of bounds");
		}

		mIndex = ++mIndex;

		return *this;
	}

	// post-increment
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();

		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner pointing to nullptr");
		}

		if (mIndex == 0)
		{
			throw std::runtime_error("Decrementing Iterator to out of bounds");
		}

		mIndex = --mIndex;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp = *this;
		operator--();

		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(const size_t rhs) 
	{
		if ((mIndex + rhs) >= mOwner->mSize)
		{
			throw std::runtime_error("Accessing Iterator to out of bounds");
		}

		return ConstIterator(mOwner, mIndex + rhs);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator-(const size_t rhs) 
	{
		if (mIndex < rhs)
		{
			throw std::runtime_error("Accessing Iterator to out of bounds");
		}

		return ConstIterator(mOwner, mIndex - rhs);
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (mOwner->IsEmpty())
		{
			throw std::runtime_error("No data to dereference.");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator is trying to dereference out of bounds.");
		}

		return (*mOwner)[mIndex];
	}

	template<typename T>
	inline const T* Vector<T>::ConstIterator::operator->() const
	{
		if (mOwner->IsEmpty())
		{
			throw std::runtime_error("No data to dereference.");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator is trying to dereference out of bounds.");
		}

		return &(*mOwner)[mIndex];
	}

#pragma endregion ConstIterator

#pragma region Vector

	template<typename T>
	inline Vector<T>::Vector(size_t capacity, IncrementFunctor incrementFunctor) :
		mIncrementFunctor(incrementFunctor)
	{
		Reserve(capacity);
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector<T>& rhs)
	{
		mIncrementFunctor = rhs.mIncrementFunctor;

		Reserve(rhs.mCapacity);
		auto it = rhs.begin();
		for (; it != rhs.end(); ++it)
		{
			PushBack(*it);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector<T>&& rhs) noexcept :
		mCapacity(rhs.mCapacity), mSize(rhs.mSize), mData(rhs.mData), mIncrementFunctor(rhs.mIncrementFunctor)
	{
		rhs.mCapacity = 0;
		rhs.mSize = 0;
		rhs.mData = nullptr;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mIncrementFunctor = rhs.mIncrementFunctor;

			Reserve(rhs.mCapacity);

			for (auto const it : rhs) 
			{
				PushBack(it);
			}
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			free (mData);

			mCapacity = rhs.mCapacity;
			mSize = rhs.mSize;
			mData = rhs.mData;
			mIncrementFunctor = rhs.mIncrementFunctor;

			rhs.mCapacity = 0;
			rhs.mSize = 0;
			rhs.mData = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		free (mData);
	}

	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<T> list, IncrementFunctor incrementFunctor) :
		mCapacity(0), mSize(0), mData(nullptr), mIncrementFunctor(incrementFunctor)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		if (index >= mSize )
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline T& Vector<T>::At(size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			mData[--mSize].~T();
		}
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return *mData;
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return *mData;
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mData[mSize-1];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mData[mSize-1];
	}

	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(this, 0);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(this, mSize);
	}

	template<typename T>
	inline void Vector<T>::PushBack(const T& data)
	{
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + std::max(mIncrementFunctor(mSize, mCapacity), size_t(1)));
		}

		new (mData + mSize) T(data);
		++mSize;
	}

	template<typename T>
	inline void Vector<T>::PushBack(T&& data)
	{
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + std::max(mIncrementFunctor(mSize, mCapacity), size_t(1)));
		}

		new (mData + mSize) T(std::move(data));
		++mSize;
	}

	template<typename T>
	inline void Vector<T>::Reserve(const size_t capacity)
	{
		if (capacity > mCapacity)
		{
			mCapacity = capacity;

			T* data = reinterpret_cast<T*>(realloc(mData, mCapacity * sizeof(T)));
			assert(data != nullptr);
			mData = data;
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(const size_t size)
	{
		if (size < mSize)
		{
			size_t removingElements = mSize - size;
			for (size_t i = 0; i < removingElements; ++i)
			{
				PopBack();
			}
		}
		else if (size > mSize)
		{
			if (size > mCapacity)
			{
				Reserve(size);
			}

			size_t appendingElements = size - mSize;
			for (size_t i = 0; i < appendingElements; ++i)
			{
				new (mData + mSize) T(); // default construct elements into vector
				++mSize;
			}
		}
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (mSize < mCapacity)
		{
			mData = reinterpret_cast<T*>(realloc(mData, sizeof(T) * mSize));			
			mCapacity = mSize;
		}
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		Iterator it = begin();
		for (; it != end(); it++)
		{
			if (*it == value) break;
		}
		return it;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		ConstIterator it = begin();
		for (; it != end(); it++)
		{
			if (*it == value) break;
		}
		return it;
	}

	template<typename T>
	inline void Vector<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopBack();
		}
	}

	template<typename T>
	inline void Vector<T>::Remove(const T& value)
	{
		auto it = Find(value);
		if(it != end()) Remove(it);
	}

	template<typename T>
	inline void Vector<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Owner mismatch.");
		}

		if (it < end()) Remove(it, it);
	}

	template<typename T>
	inline void Vector<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if ((first.mOwner != this) || (last.mOwner != this) || (first.mOwner != last.mOwner))
		{
			throw std::runtime_error("Owner mismatch.");
		}

		if (first > last)
		{
			throw std::runtime_error("Invalid arguments.");
		}

		if (last < end())
		{
			for (auto it = first; it <= last; ++it)
			{
				(*it).~T();
			}

			memmove(mData + first.mIndex, mData + last.mIndex + 1, (end().mIndex - last.mIndex) * sizeof(T));

			mSize -= last.mIndex - first.mIndex + 1;
		}
	}



#pragma endregion Vector
}