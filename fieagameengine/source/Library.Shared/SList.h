#pragma once

#include "DefaultEquality.h"

namespace Library
{
	/// <summary>
	/// Represents a generic sigluarly-linked list
	/// </summary>
	template <typename T>
	class SList final
	{
		friend struct Iterator;
		friend struct ConstIterator;

	private:
		/// <summary>
		/// auxiliary helper class that SList uses to create Nodes in a list, contains:
		/// Data: member variable
		/// Node: pointer to the next Node in a list
		/// </summary>
		struct Node final
		{
			T Data;
			Node* Next;

			Node(const T& data, Node* next = nullptr);
			Node(T&& data, Node* next = nullptr);
		};

	public:
		using value_type = T;

		/// <summary>
		/// Embedded class for SList for Node traversal.
		/// mOwner: member variable to keep track of SList that owns this Iterator
		/// mNodePtr: a Node pointer that points to corresponding Node
		/// </summary>
		struct Iterator final
		{
			friend SList;
			friend struct ConstIterator;

		public:
			Iterator() = default;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) noexcept = default;
			Iterator& operator=(const Iterator&) = default;
			Iterator& operator=(Iterator&&) noexcept = default;
			~Iterator() = default;

			/// <summary>
			/// Comparison operator: equal
			/// </summary>
			/// <param name="rhs">R-value to compare self to</param>
			/// <returns>Returns true if both are pointing to the same Node</returns>
			const bool operator == (const Iterator& rhs) const;
			/// <summary>
			/// Comparison operator: not equal
			/// </summary>
			/// <param name="rhs">R-value to compare self to</param>
			/// <returns>Returns false if both are pointing to the same Node</returns>
			const bool operator != (const Iterator& rhs) const;

			/// <summary>
			/// Pre-Increment Operator, moves the Iterator mNodePtr to the Next Node
			/// </summary>
			Iterator& operator ++ (); // pre-increment
			/// <summary>
			/// Post-Increment Operator, moves the Iterator mNodePtr to the Next Node
			/// </summary>
			Iterator operator ++ (int); // post-increment

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Returns a reference to the item contained by the node the Iterator points to</returns>
			T& operator * () const;

		private:
			/// <summary>
			/// Iterator Constructor
			/// </summary>
			/// <param name="owner">member variable to keep track of SList that owns this Iterator</param>
			/// <param name="nodePtr">a Node pointer that points to corresponding Node</param>
			Iterator(const SList* owner, Node* nodePtr);

			const SList* mOwner{ nullptr };
			Node* mNodePtr{ nullptr };
		};

		/// <summary>
		/// Embedded class for SList for Node traversal (Const version).
		/// mOwner: member variable to keep track of SList that owns this Iterator
		/// mNodePtr: a Node pointer that points to corresponding Node
		/// </summary>
		struct ConstIterator final
		{
			friend SList;

		public:
			ConstIterator() = default;
			ConstIterator(Iterator & it);
			ConstIterator(const ConstIterator&) = default;
			ConstIterator(ConstIterator&&) noexcept = default;
			ConstIterator& operator=(const ConstIterator&) = default;
			ConstIterator& operator=(ConstIterator&&) noexcept = default;
			~ConstIterator() = default;

			/// <summary>
			/// ConstIterator Constructor
			/// </summary>
			/// <param name="it">Iterator to create a const verison from</param>
			ConstIterator(const Iterator& it);

			/// <summary>
			/// Comparison operator: equal
			/// </summary>
			/// <param name="rhs">R-value to compare self to</param>
			/// <returns>Returns true if both are pointing to the same Node</returns>
			const bool operator == (const ConstIterator& rhs) const;
			/// <summary>
			/// Comparison operator: not equal
			/// </summary>
			/// <param name="rhs">R-value to compare self to</param>
			/// <returns>Returns false if both are pointing to the same Node</returns>
			const bool operator != (const ConstIterator& rhs) const;

			/// <summary>
			/// Pre-Increment Operator, moves the Iterator mNodePtr to the Next Node
			/// </summary>
			ConstIterator& operator ++ (); // pre-increment
			/// <summary>
			/// Post-Increment Operator, moves the Iterator mNodePtr to the Next Node
			/// </summary>
			ConstIterator operator ++ (int); // post-increment

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Returns a const reference to the item contained by the node the Iterator points to</returns>
			const T& operator * () const;

		private:
			/// <summary>
			/// ConstIterator Constructor
			/// </summary>
			/// <param name="owner">member variable to keep track of SList that owns this Iterator</param>
			/// <param name="nodePtr">a Node pointer that points to corresponding Node</param>
			ConstIterator(const SList* owner, Node* nodePtr);

			const SList* mOwner{ nullptr };
			Node* mNodePtr{ nullptr };
		};

		/// <summary>
		/// Default Constructor: 
		/// mSize = 0, mFront = nullptr, mBack = nullptr
		/// </summary>
		SList() = default;

		/// <summary>
		/// Copy Constructor: Copies the contents of the SList in the param and constructs a SList with a Deep-Copy
		/// </summary>
		/// <param name="rhs">SList to be copied</param>
		SList(const SList& rhs);

		/// <summary>
		/// Move Constructor: Does a Shallow-Copy of the SList in the param
		/// </summary>
		/// <param name="rhs">SList to be moved</param>
		/// <returns>reference to the moved SList is returned</returns>
		SList(SList&& rhs) noexcept;

		/// <summary>
		/// Destructor: Clears the List and prevents memory leaks
		/// </summary>
		~SList();

		/// <summary>
		/// Copy Assignment: Copies the contents of the SList in the rhs and assigns a SList with a Deep-Copy
		/// </summary>
		/// <param name="rhs">SList to be copied</param>
		/// <returns>reference to the copied SList is returned</returns>
		SList& operator = (const SList& rhs);

		/// <summary>
		/// Move Assignment: Does a Shallow-Copy of the SList in the rhs
		/// </summary>
		/// <param name="rhs">SList to be moved</param>
		/// <returns>reference to the moved SList is returned</returns>
		SList& operator = (SList&& rhs) noexcept;
		
		/// <summary>
		/// Return a bool indicating whether the list contains any items
		/// </summary>
		/// <returns>true if empty and false if not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// return the number of items in the list as an unsigned int
		/// </summary>
		/// <returns>number of items in list as an unsigned int</returns>
		size_t Size() const;

		/// <summary>
		/// remove all items in the list
		/// </summary>
		void Clear();

		/// <summary>
		/// return the first item in the list, if list is empty throws exception
		/// </summary>
		/// <returns>the first item in the list</returns>
		/// <exception cref="runtime_error">List is empty.</exception>
		T& Front();

		/// <summary>
		/// return the first item in the list as a const, if list is empty throws exception
		/// </summary>
		/// <returns>the first item in the list as a const</returns>
		const T& Front() const;

		/// <summary>
		/// return the last item in the list, if list is empty throws exception
		/// </summary>
		/// <returns>the last item in the list</returns>
		T& Back();

		/// <summary>
		/// return the last item in the list as a const, if list is empty throws exception
		/// </summary>
		/// <returns>return the last item in the list as a const</returns>
		const T& Back() const;

		/// <summary>
		/// insert given item before the front of the list
		/// </summary>
		/// <param name="data">item to be inserted</param>
		void PushFront(const T& data);
		void PushFront(T&& data);

		//Iterator PushFront(T && value);

		/// <summary>
		/// append given item after the end of the list
		/// </summary>
		/// <param name="data">item to be appended</param>
		void PushBack(const T& data);


		//Iterator PushBack(T && value);

		/// <summary>
		/// remove the first item from the list and destructs the item deleted
		/// </summary>
		void PopFront();

		/// <summary>
		///  remove the last item from the list and destructs the item deleted
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns an SList::Iterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an SList::Iterator pointing to the head of the list</returns>
		Iterator begin();
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an SList::ConstIterator pointing to the head of the list</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an SList::ConstIterator pointing to the head of the list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an SList::Iterator pointing past the end of the list.
		/// </summary>
		/// <returns>Returns an SList::Iterator pointing past the end of the list.</returns>
		Iterator end();
		/// <summary>
		/// Returns an SList::ConstIterator pointing past the end of the list.
		/// </summary>
		/// <returns>Returns an SList::ConstIterator pointing past the end of the list.</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an SList::ConstIterator pointing past the end of the list.
		/// </summary>
		/// <returns>Returns an SList::ConstIterator pointing past the end of the list.</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Append the given item after the item the given iterator points to.
		/// </summary>
		/// <param name="value">Item to append</param>
		/// <param name="it">Iterator to append after</param>
		/// <returns>Returns an Iterator that points to the newly appended item</returns>
		Iterator InsertAfter(const T& value, const Iterator& it);

		/// <summary>
		/// Iterates through a list until given value is found and returns an Iterator pointing to it
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Returns an Iterator pointing to a given item</returns>
		Iterator Find(const T& value);
		/// <summary>
		/// Iterates through a list until given value is found and returns an ConstIterator pointing to it (Const version)
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Returns an ConstIterator pointing to a given item</returns>
		ConstIterator Find(const T& value) const;

		/// <summary>
		/// Remove the item associated with the given data and maintain list integrity. Calling Remove on a non-existent item will not cause any harm.
		/// </summary>
		/// <param name="value">Value to search and remove from the list</param>
		void Remove(const T& value);
		/// <summary>
		/// Remove the item associated with the given data and maintain list integrity. Calling Remove on a non-existent item will not cause any harm.
		/// </summary>
		/// <param name="it">Iterator pointing to value to remove from the list</param>
		void Remove(const Iterator& it);

	private:
		size_t mSize{ 0 };
		Node* mFront{ nullptr };
		Node* mBack{ nullptr };
	};
}


#include "SList.inl"