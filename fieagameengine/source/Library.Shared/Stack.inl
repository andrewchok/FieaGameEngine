#include "Stack.h"
#include <cstddef>

namespace Library
{
	template<typename T>
	inline Stack<T>::Stack(const Stack& rhs) :
		mList(rhs.mList)
	{
	}

	template<typename T>
	inline Stack<T>::Stack(Stack&& rhs) :
		mList(std::move(rhs.mList))
	{
	}

	template<typename T>
	inline Stack<T>& Stack<T>::operator=(const Stack& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mList = rhs.mList;
		}
		return *this;
	}

	template<typename T>
	inline Stack<T>& Stack<T>::operator=(Stack&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mList = std::move(rhs.mList);
		}
		return *this;
	}

	template <typename T>
	inline void Stack<T>::Clear()
	{
		mList.Clear();
	}

	template <typename T>
	Stack<T>::~Stack()
	{
		Clear();
	}

	template <typename T>
	inline void Stack<T>::Push(const T& value)
	{
		mList.PushFront(value);
	}

	template<typename T>
	inline void Stack<T>::Push(const T&& value)
	{
		mList.PushFront(std::move(value));
	}

	template <typename T>
	inline void Stack<T>::Pop()
	{
		mList.PopFront();
	}

	template <typename T>
	inline T& Stack<T>::Top()
	{
		return mList.Front();
	}

	template <typename T>
	inline const T& Stack<T>::Top() const
	{
		return mList.Front();
	}

	template <typename T>
	inline std::size_t Stack<T>::Size() const
	{
		return mList.Size();
	}

	template <typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return mList.IsEmpty();
	}
}