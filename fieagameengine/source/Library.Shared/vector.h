#pragma once

#include <functional>

#include "DefaultEquality.h"
#include "DefaultIncrement.h"

namespace Library
{
	/// <summary>
	/// Represents a generic Vector list
	/// </summary>
	template <typename T>
	class Vector final
	{
	public:
		using value_type = T;
		using IncrementFunctor = std::function<size_t(const size_t, const size_t)>;

		/// <summary>
		/// Embedded class for Vector for Index traversal
		/// mOwner: member variable to keep track of Vector that owns this Iterator
		/// mIndex: member variable to keep track of Index
		/// </summary>
		struct Iterator final
		{
			friend Vector;
			friend struct ConstIterator;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::random_access_iterator_tag;

			/// <summary>
			/// Default Constructor
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Default Copy Constructor
			/// </summary>
			/// <param name="">Takes in a const Iterator&</param>
			Iterator(const Iterator&) = default;
			/// <summary>
			/// Default Move Constructor
			/// </summary>
			/// <param name="">Takes in a Iterator&&</param>
			/// <returns>Moved Iterator</returns>
			Iterator(Iterator&&) noexcept = default;
			/// <summary>
			/// Default Copy Assignment
			/// </summary>
			/// <param name="">Takes in a const Iterator&</param>
			/// <returns>Copied Iterator</returns>
			Iterator& operator=(const Iterator&) = default;
			/// <summary>
			/// Default Move Assignment
			/// </summary>
			/// <param name="">Takes in a Iterator&&</param>
			/// <returns>Moved Iterator</returns>
			Iterator& operator=(Iterator&&) noexcept = default;
			/// <summary>
			/// Default Destructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator == (const Iterator& rhs) const;
			/// <summary>
			/// Not Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator != (const Iterator& rhs) const;
			/// <summary>
			/// Lesser than Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator < (const Iterator& rhs) const;
			/// <summary>
			/// Greater than Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator > (const Iterator& rhs) const;
			/// <summary>
			/// Lesser Than or Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator <= (const Iterator& rhs) const;
			/// <summary>
			/// Greater Than or Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator >= (const Iterator& rhs) const;


			/// <summary>
			/// Pre-Increment Operator, adds one to the Iterator Index
			/// </summary>
			/// <returns>Incremented Iterator</returns>
			Iterator& operator ++ ();
			/// <summary>
			/// Post-Increment Operator, adds one to the Iterator Index
			/// </summary>
			/// <returns>Copy of Iterator before Increment</returns>
			Iterator operator ++ (int);
			/// <summary>
			/// Pre-Decrement Operator, subtracts one from the Iterator Index
			/// </summary>
			/// <returns>Decremented Iterator</returns>
			Iterator& operator -- ();
			/// <summary>
			/// Post-Decrement Operator, subtracts one from the Iterator Index
			/// </summary>
			/// <returns>>Copy of Iterator before Decrement</returns>
			Iterator operator -- (int);
			/// <summary>
			/// Addition Operator
			/// </summary>
			/// <param name="rhs">r-value</param>
			/// <returns>changed Iterator</returns>
			Iterator operator + (const size_t rhs);
			/// <summary>
			/// Subtraction Operator
			/// </summary>
			/// <param name="rhs">r-value</param>
			/// <returns>changed Iterator</returns>
			Iterator operator - (const size_t rhs);

			Iterator& operator+=(std::size_t offset);
			Iterator& operator-=(std::size_t offset);

			/// <summary>
			/// Dereference Operator
			/// </summary>
			/// <returns>Returns a reference to the item contained by the Index of the Iterator</returns>
			T& operator * () const;
			/// <summary>
			/// Pointer Dereference Operator
			/// </summary>
			/// <returns>Returns a reference to the pointer pointing to the item containted by the Index of the Iterator</returns>
			T* operator -> () const;

		private:
			/// <summary>
			/// Iterator Constructor
			/// </summary>
			/// <param name="owner">member variable to keep track of Vector that owns this Iterator</param>
			/// <param name="index">an Index that points to a T value</param>
			Iterator(Vector* owner, size_t index);

			Vector* mOwner{ nullptr };
			size_t mIndex{ 0 };
		};

		struct ConstIterator final
		{
			friend class Vector;
			friend struct Iterator;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::random_access_iterator_tag;

			/// <summary>
			/// Default Constructor
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// ConstIterator Constructor
			/// </summary>
			/// <param name="it">const Iterator reference</param>
			ConstIterator(const Iterator& it);
			/// <summary>
			/// Default Copy Constructor
			/// </summary>
			/// <param name="">Takes in a const ConstIterator&</param>
			ConstIterator(const ConstIterator&) = default;
			/// <summary>
			/// Default Move Constructor
			/// </summary>
			/// <param name="">Takes in a ConstIterator&&</param>
			/// <returns>Moved ConstIterator</returns>
			ConstIterator(ConstIterator&&) noexcept = default;
			/// <summary>
			/// Default Copy Assignment
			/// </summary>
			/// <param name="">Takes in a const ConstIterator&</param>
			/// <returns>Copied ConstIterator</returns>
			ConstIterator& operator=(const ConstIterator&) = default;
			/// <summary>
			/// Default Move Assignment
			/// </summary>
			/// <param name="">Takes in a ConstIterator&&</param>
			/// <returns>Moved ConstIterator</returns>
			ConstIterator& operator=(ConstIterator&&) noexcept = default;
			/// <summary>
			/// Default Destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator == (const ConstIterator& rhs) const;
			/// <summary>
			/// Not Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator != (const ConstIterator& rhs) const;
			/// <summary>
			/// Lesser than Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator < (const ConstIterator& rhs) const;
			/// <summary>
			/// Greater than Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator > (const ConstIterator& rhs) const;
			/// <summary>
			/// Lesser Than or Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator <= (const ConstIterator& rhs) const;
			/// <summary>
			/// Greater Than or Equal Operator
			/// </summary>
			/// <param name="rhs">takes in a rhs</param>
			/// <returns>bool</returns>
			const bool operator >= (const ConstIterator& rhs) const;

			/// <summary>
			/// Pre-Increment Operator, adds one to the ConstIterator Index
			/// </summary>
			/// <returns>Incremented ConstIterator</returns>
			ConstIterator& operator ++ ();
			/// <summary>
			/// Post-Increment Operator, adds one to the ConstIterator Index
			/// </summary>
			/// <returns>Copy of ConstIterator before Increment</returns>
			ConstIterator operator ++ (int);
			/// <summary>
			/// Pre-Decrement Operator, subtracts one from the ConstIterator Index
			/// </summary>
			/// <returns>Decremented ConstIterator</returns>
			ConstIterator& operator -- ();
			/// <summary>
			/// Post-Decrement Operator, subtracts one from the ConstIterator Index
			/// </summary>
			/// <returns>>Copy of ConstIterator before Decrement</returns>
			ConstIterator operator -- (int);
			/// <summary>
			/// Addition Operator
			/// </summary>
			/// <param name="rhs">r-value</param>
			/// <returns>changed ConstIterator</returns>
			ConstIterator operator + (const size_t rhs);
			/// <summary>
			/// Subtraction Operator
			/// </summary>
			/// <param name="rhs">r-value</param>
			/// <returns>changed ConstIterator</returns>
			ConstIterator operator - (const size_t rhs);

			/// <summary>
			/// Dereference Operator
			/// </summary>
			/// <returns>Returns a const reference to the item contained by the Index of the ConstIterator</returns>
			const T& operator * () const;

			/// <summary>
			/// Pointer Dereference Operator
			/// </summary>
			/// <returns>Returns a const reference to the pointer pointing to the item at the Index of the ConstIterator</returns>
			const T* operator -> () const;

		private:
			/// <summary>
			/// ConstIterator Constructor
			/// </summary>
			/// <param name="owner">member variable to keep track of Vector that owns this ConstIterator</param>
			/// <param name="index">an Index that points to a T value</param>
			ConstIterator(const Vector* owner, size_t index);

			const Vector* mOwner{ nullptr };
			size_t mIndex{ 0 };
		};

		/// <summary>
		/// Default Constructor
		/// </summary>
		Vector(size_t capacity = 0, IncrementFunctor incrementFunctor = DefaultIncrement<T>());
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Vector reference</param>
		Vector(const Vector& rhs);
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Vector&&</param>
		/// <returns>Moved Vector</returns>
		Vector(Vector&& rhs) noexcept;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Vector reference</param>
		/// <returns>Copied Vector</returns>
		Vector& operator=(const Vector& rhs);
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Vector&&</param>
		/// <returns>Moved Vector</returns>
		Vector& operator=(Vector&& rhs) noexcept;
		/// <summary>
		/// Destructor
		/// </summary>
		~Vector();

		Vector(std::initializer_list<T> list, IncrementFunctor incrementFunctor = DefaultIncrement<T>());

#pragma region Constant Time
		/// <summary>
		/// Index Operator, Accessing the T value at the Index specified
		/// </summary>
		/// <param name="index">index to look at</param>
		/// <returns>T reference</returns>
		T& operator [] (size_t index);
		/// <summary>
		/// Const Index Operator, Accessing the T value at the Index sepcified
		/// </summary>
		/// <param name="index">index to look at</param>
		/// <returns>const T reference</returns>
		const T& operator [] (size_t index) const;

		/// <summary>
		/// Index Accessor, Accessing the T value at the Index specified
		/// </summary>
		/// <param name="index">index to look at</param>
		/// <returns>T reference</returns>
		T& At(size_t index);
		/// <summary>
		/// Const Index Accessor, Accessing the T value at the Index specified
		/// </summary>
		/// <param name="index">index to look at</param>
		/// <returns>const T reference</returns>
		const T& At(size_t index) const;

		/// <summary>
		/// remove the last item from the list and destructs the item, memory is preserved
		/// </summary>
		void PopBack();

		/// <summary>
		/// Return a bool indicating whether the list contains any items
		/// </summary>
		/// <returns>true if empty and false if not</returns>
		bool IsEmpty() const;

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
		/// return the number of items in the list as an unsigned int
		/// </summary>
		/// <returns>number of items in list as an unsigned int</returns>
		size_t Size() const;

		/// <summary>
		/// return the capacity of the list as an unsigned int
		/// </summary>
		/// <returns>capacity of list as an unsigned int</returns>
		size_t Capacity() const;

		/// <summary>
		/// Returns an Vector::Iterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an Vector::Iterator pointing to the head of the list</returns>
		Iterator begin();
		/// <summary>
		/// Returns an Vector::ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an Vector::ConstIterator pointing to the head of the list</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an Vector::ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an Vector::ConstIterator pointing to the head of the list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an Vector::Iterator pointing one past the end of the list.
		/// </summary>
		/// <returns>Returns an Vector::Iterator pointing one past the end of the list.</returns>
		Iterator end();
		/// <summary>
		/// Returns an Vector::ConstIterator pointing one past the end of the list.
		/// </summary>
		/// <returns>Returns an Vector::ConstIterator pointing one past the end of the list.</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an Vector::ConstIterator pointing one past the end of the list.
		/// </summary>
		/// <returns>Returns an Vector::ConstIterator pointing one past the end of the list.</returns>
		ConstIterator cend() const;

#pragma endregion Constant Time

#pragma region Linear Time
		/// <summary>
		/// append given item after the end of the list, if the Capacity has been reached, 
		/// Reserve is called to allocate more space
		/// </summary>
		/// <param name="data">item to be appended</param>
		void PushBack(const T& data);
		void PushBack(T&& data);

		/// <summary>
		/// Used to Allocate more space in memory, if given a smaller capacity than current
		/// it will increase the current capcity by one.
		/// </summary>
		/// <param name="capacity">Capacity to increase allocated memory to</param>
		void Reserve(const size_t capacity);

		void Resize(const size_t size);
		void ShrinkToFit();

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
		/// remove all items in the list, memory is preserved
		/// </summary>
		void Clear();
		/// <summary>
		/// Remove the item associated with the given data and maintain list integrity. 
		/// Calling Remove on a non-existent item will not cause any harm.
		/// memory is preserved
		/// </summary>
		/// <param name="value">Value to search and remove from the list</param>
		void Remove(const T& value);
		/// <summary>
		/// Remove the item associated with the Iterator pointing to it
		/// and maintain list integrity. 
		/// Calling Remove on a non-existent item will not cause any harm.
		/// memory is preserved
		/// </summary>
		/// <param name="value">Value to search and remove from the list</param>
		void Remove(const Iterator& it);
		/// <summary>
		/// Remove a range of items from a starting point a Iterator is pointing to
		/// to an ending point anotherIterator is pointing to.
		/// memory is preserved
		/// </summary>
		/// <param name="first">starting point</param>
		/// <param name="last">ending point</param>
		void Remove(const Iterator& first, const Iterator& last);
#pragma endregion Linear Time

	private:
		size_t mCapacity{ 0 };
		size_t mSize{ 0 };
		T* mData{ nullptr };
		IncrementFunctor mIncrementFunctor;
	};
}

#include "vector.inl"