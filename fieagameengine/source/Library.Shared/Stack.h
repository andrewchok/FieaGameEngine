#pragma once
#include "SList.h"

namespace Library
{
	template <typename T>
	class Stack final
	{
	public:
		Stack() = default;
		Stack(const Stack& rhs);
		Stack& operator=(const Stack& rhs);
		Stack(Stack&& rhs);
		Stack& operator=(Stack&& rhs);
		~Stack();

		void Push(const T& value);
		void Push(const T&& value);

		void Pop();

		T& Top();

		const T& Top() const;

		std::size_t Size() const;

		bool IsEmpty() const;

		void Clear();

	private:
		SList<T> mList;
	};
}

#include "Stack.inl"
