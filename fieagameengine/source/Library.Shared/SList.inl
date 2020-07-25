#include "SList.h"

#include <stdexcept>

namespace Library
{
#pragma region Node

	template<typename T>
	inline SList<T>::Node::Node(const T& data, Node* next) :
		Data(data), Next(next)
	{
	}

	template<typename T>
	inline SList<T>::Node::Node(T&& data, Node* next) :
		Data(std::move(data)), Next(next)
	{
	}

#pragma endregion Node

#pragma region Iterator

	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList<T>* owner, Node* nodePtr) :
		mOwner(owner), mNodePtr(nodePtr)
	{
	}

	template<typename T>
	inline const bool SList<T>::Iterator::operator==(const Iterator& rhs) const 
	{
		return mOwner == rhs.mOwner && mNodePtr == rhs.mNodePtr;
	}

	template<typename T>
	inline const bool SList<T>::Iterator::operator!=(const Iterator& rhs) const 
	{
		return !(operator==(rhs));
	}

	// pre-increment
	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mNodePtr == nullptr)
		{
			throw std::runtime_error("Incrementing Iterator with a Node pointing to nullptr.");
		}

		mNodePtr = mNodePtr->Next;

		return *this;
	}

	// post-increment
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();

		return temp;
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (mNodePtr == nullptr)
		{
			throw std::runtime_error("Iterator is at end and is pointing to nullptr.");
		}

		return mNodePtr->Data;
	}

#pragma endregion Iterator

#pragma region ConstIterator

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList<T>* owner, Node* nodePtr) :
		mOwner(owner), mNodePtr(nodePtr)
	{
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& it) : 
		mOwner(it.mOwner), mNodePtr(it.mNodePtr)
	{
	}

	template<typename T>
	inline const bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return mOwner == rhs.mOwner && mNodePtr == rhs.mNodePtr;
	}

	template<typename T>
	inline const bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return !(operator==(rhs));
	}

	// pre-increment
	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (mNodePtr == nullptr)
		{
			throw std::runtime_error("Incrementing Iterator with a Node pointing to nullptr.");
		}

		mNodePtr = mNodePtr->Next;

		return *this;
	}

	// post-increment
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator it(this->mOwner, this->mNodePtr);
		operator++();

		return it;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (mNodePtr == nullptr)
		{
			throw std::runtime_error("Iterator is at end and is pointing to nullptr.");
		}

		return mNodePtr->Data;
	}

#pragma endregion ConstIterator

#pragma region SList

	template<typename T>
	inline SList<T>::SList(const SList& rhs)
	{
		Node* node = rhs.mFront;
		while (node != nullptr)
		{
			PushBack(node->Data);
			node = node->Next;
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& rhs) noexcept :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.mSize = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Node* node = rhs.mFront;
			while (node != nullptr)
			{
				PushBack(node->Data);
				node = node->Next;
			}
		}
		
		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();

			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;

			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopFront();
		}
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mFront->Data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mFront->Data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mBack->Data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mBack->Data;
	}

	template<typename T>
	void SList<T>::PushFront(const T& data)
	{
		mFront = new Node(data, mFront);

		if (IsEmpty())
		{
			mBack = mFront;
		}

		mSize++;
	}

	template<typename T>
	void SList<T>::PushFront(T&& data)
	{
		mFront = new Node(std::move(data), mFront);

		if (IsEmpty())
		{
			mBack = mFront;
		}

		mSize++;
	}

	template<typename T>
	inline void SList<T>::PushBack(const T& data)
	{		
		Node* node = new Node(data, nullptr);

		if (IsEmpty())
		{
			mFront = node;
		}
		else
		{
			mBack->Next = node;
		}

		mBack = node;
		mSize++;
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			Node* node = mFront->Next;
			delete mFront;
			mFront = node;
			mSize--;

			if (mSize <= 1)
			{
				mBack = mFront;
			}
		}
	}

	template<typename T>
	inline void SList<T>::PopBack()
	{
		if (mSize == 1)
		{
			PopFront();
		}
		else
		{
			Node* node = mFront;
			while (node->Next != mBack)
			{
				node = node->Next;
			}

			delete mBack;
			mSize--;
			mBack = node;
			mBack->Next = nullptr;
		}
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(this, mFront);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(this, mFront);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(this, mFront);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const T& value, const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Mismatch Owner: Iterator and SList");
		}
		if (it.mNodePtr == nullptr || it.mNodePtr->Next == nullptr)
		{
			PushBack(value);
			return Iterator(this, mBack);
		}
		else
		{
			Node* node = new Node(value, it.mNodePtr->Next);
			it.mNodePtr->Next = node;
			++mSize;
			return Iterator(this, node);
		}
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		Iterator it = begin();
		for (; it != end(); it++)
		{
			if (*it == value) break;
		}
		return it;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		ConstIterator it = begin();
		for (; it != end(); it++)
		{
			if (*it == value) break;
		}
		return it;
	}

	template<typename T>
	inline void SList<T>::Remove(const T& value)
	{
		Remove(Find(value));
	}
	template<typename T>
	inline void SList<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Mismatch Owner: Invalid Iterator");
		}

		bool found = false;
		if (it != end())
		{
			if (it.mNodePtr == mBack)
			{
				PopBack();
			}
			else
			{
				Node* next = it.mNodePtr->Next;
				it.mNodePtr->Data.~T();
				new(&it.mNodePtr->Data)T(std::move(next->Data));
				it.mNodePtr->Next = next->Next;
				delete next;

				if (it.mNodePtr->Next == nullptr)
				{
					mBack = it.mNodePtr;
				}
				--mSize;
			}

			found = true;
		}

	}

#pragma endregion SList
}