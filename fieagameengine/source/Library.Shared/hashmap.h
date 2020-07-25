#pragma once
#include "vector.h"
#include "SList.h"

#include "DefaultEquality.h"
#include "DefaultHash.h"

namespace Library
{
	/// <summary>
	/// Represents a Templated Hashmap using Chaining with a Vector List 
	/// Combined with a Singularly Linked List
	/// </summary>
	template <typename TKey, typename TData>
	class Hashmap final
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using HashFunctor = std::function<size_t(const TKey&)>;
		using EqualityFunctor = std::function<bool(const TKey&, const TKey&)>;

		/// <summary>
		/// Embedded class for Hashmap to use to traverse its members.
		/// It will include an index to represent its buckets and
		/// A SList Iterator to traverse the buckets contents
		/// </summary>
		struct Iterator final
		{
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = PairType;
			using reference = PairType&;
			using pointer = PairType*;
			using iterator_category = std::forward_iterator_tag;

			friend Hashmap;
			friend struct ConstIterator;

		public:
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
			/// Pre-Increment Operator
			/// </summary>
			/// <returns>Incremented Iterator</returns>
			Iterator& operator ++ ();
			/// <summary>
			/// Post-Increment Operator
			/// </summary>
			/// <returns>Copy of Iterator before Increment</returns>
			Iterator operator ++ (int);

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
			/// Dereference Operator
			/// </summary>
			/// <returns>Returns a reference to the item pointed by the Iterator</returns>
			PairType& operator * () const;

			/// <summary>
			/// Pointer Dereference Operator
			/// </summary>
			/// <returns>Returns a Pointer reference to the item pointed by the Iterator</returns>
			PairType* operator -> () const;

		private:
			/// <summary>
			/// Iterator Constructor
			/// </summary>
			/// <param name="owner">member variable to keep track of Hashmap that owns this Iterator</param>
			/// <param name="chainIterator">a SList::Iterator that points to the TKey</param>
			/// <param name="index">an Index that points to bucket that holds SList containing TKey</param>
			Iterator(Hashmap* owner, typename ChainType::Iterator chainIterator, size_t index);

			Hashmap* mOwner{ nullptr };
			typename ChainType::Iterator mChainIterator;
			size_t mIndex{ 0 };
		};

		/// <summary>
		/// Embedded class for Hashmap to use to traverse its members.
		/// It will include an index to represent its buckets and
		/// A SList Iterator to traverse the buckets contents
		/// </summary>
		struct ConstIterator final
		{
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = PairType;
			using reference = PairType&;
			using pointer = PairType*;
			using iterator_category = std::forward_iterator_tag;

			friend Hashmap;

		public:
			/// <summary>
			/// Default Constructor
			/// </summary>
			ConstIterator() = default;			
			/// <summary>
			/// ConstIterator Constructor
			/// </summary>
			/// <param name="it">const Iterator reference</param>
			ConstIterator(const Iterator & it);
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
			/// Pre-Increment Operator
			/// </summary>
			/// <returns>Incremented Iterator</returns>
			ConstIterator& operator ++ ();
			/// <summary>
			/// Post-Increment Operator
			/// </summary>
			/// <returns>Copy of Iterator before Increment</returns>
			ConstIterator operator ++ (int);

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
			/// Dereference Operator
			/// </summary>
			/// <returns>Returns a const reference to the item pointed by the ConstIterator</returns>
			const PairType& operator * () const;

			/// <summary>
			/// Pointer Dereference Operator
			/// </summary>
			/// <returns>Returns a const Pointer reference to the item pointed by the ConstIterator</returns>
			const PairType* operator -> () const;

		private:
			/// <summary>
			/// ConstIterator Constructor
			/// </summary>
			/// <param name="owner">member variable to keep track of Hashmap that owns this Iterator</param>
			/// <param name="chainIterator">a SList::Iterator that points to the TKey</param>
			/// <param name="index">an Index that points to bucket that holds SList containing TKey</param>
			ConstIterator(const Hashmap * owner, typename ChainType::ConstIterator chainIterator, size_t index);

			const Hashmap* mOwner{ nullptr };
			typename ChainType::ConstIterator mChainIterator;
			size_t mIndex{ 0 };
		};

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		/// <param name="bucketSize">size_t, needs to be > 0</param>
		/// <param name="hashFunctor">if none is provided, a Default one given</param>
		explicit Hashmap(size_t bucketSize = 31, HashFunctor hashFunctor = DefaultHash<TKey>(), EqualityFunctor equalityFunctor = DefaultEquality<TKey>());
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Vector reference</param>
		Hashmap(const Hashmap& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Vector&&</param>
		/// <returns>Moved Vector</returns>
		Hashmap(Hashmap&& rhs) noexcept = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Vector reference</param>
		/// <returns>Copied Vector</returns>
		Hashmap& operator=(const Hashmap& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Vector&&</param>
		/// <returns>Moved Vector</returns>
		Hashmap& operator=(Hashmap && rhs) noexcept = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~Hashmap() = default;
		/// <summary>
		/// Inserts a list of PairType values into the Hashmap
		/// </summary>
		/// <param name="list">list of PairType to insert</param>
		Hashmap(std::initializer_list<PairType> list, HashFunctor hashFunctor = DefaultHash<TKey>(), EqualityFunctor equalityFunctor = DefaultEquality<TKey>());

		/// <summary>
		/// Takes an “entry” argument of the appropriate type (constant reference to PairType) and which returns an Iterator. 
		/// If the table already contains an entry with a key that matches the key of the given entry, 
		/// then do not modify the entry. Simply return the iterator of the found key.
		/// </summary>
		/// <param name="pair">Takes a constant reference to PairType</param>
		/// <returns>returns a pair including Iterator and a bool (true if a new element is inserted and false if returning an existing iterator)</returns>
		std::tuple<Iterator, bool> Insert(const PairType& pair);

		std::tuple<Iterator, bool> Insert(PairType&& pair);

		/// <summary>
		/// Takes a “key” argument of the appropriate type (constant reference to TKey) and which returns an Iterator.
		/// </summary>
		/// <param name="key">constant reference to TKey</param>
		/// <returns>returns an Iterator</returns>
		Iterator Find(const TKey& key);
		/// <summary>
		/// Takes a “key” argument of the appropriate type (constant reference to TKey) and which returns an ConstIterator.
		/// </summary>
		/// <param name="key">constant reference to TKey</param>
		/// <returns>returns an ConstIterator</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Takes a “key” argument (as above) and which returns nothing. It should remove the matching entry, if it exists, otherwise it should do nothing.
		/// </summary>
		/// <param name="key">constant reference to TKey</param>
		void Remove(const TKey& key);
		/// <summary>
		/// Takes a Iterator argument (as above) and which returns nothing. It should remove the matching entry, if it exists, otherwise it should do nothing.
		/// </summary>
		/// <param name="it">Iterator</param>
		void Remove(const Iterator& it);

		/// <summary>
		/// Clears all the inserted elements in the list
		/// </summary>
		void Clear();

		TData& operator [] (const TKey& key);
		const TData& operator [] (const TKey& key) const;

		TData& At(const TKey& key);
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Gets the number of elements inserted in the hashmap
		/// </summary>
		/// <returns>const size_t of number of elements</returns>
		const size_t Size() const;
		/// <summary>
		/// Gets the Number of Buckets in the Hashmap
		/// </summary>
		/// <returns>const size_t of number of buckets</returns>
		const size_t BucketSize() const;

		/// <summary>
		/// returns a Boolean indicating the presence of a key within the hash map.
		/// </summary>
		/// <param name="key">key to check if in hashmap</param>
		/// <returns>Boolean</returns>
		const bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Returns an Hashmap::Iterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an Hashmap::Iterator pointing to the head of the list</returns>
		Iterator begin();
		/// <summary>
		/// Returns an Hashmap::ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an Hashmap::ConstIterator pointing to the head of the list</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an Hashmap::ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>Returns an Hashmap::ConstIterator pointing to the head of the list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an Hashmap::Iterator pointing one past the end of the list.
		/// </summary>
		/// <returns>Returns an Hashmap::Iterator pointing one past the end of the list.</returns>
		Iterator end();
		/// <summary>
		/// Returns an Hashmap::ConstIterator pointing one past the end of the list.
		/// </summary>
		/// <returns>Returns an Hashmap::ConstIterator pointing one past the end of the list.</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an Hashmap::ConstIterator pointing one past the end of the list.
		/// </summary>
		/// <returns>Returns an Hashmap::ConstIterator pointing one past the end of the list.</returns>
		ConstIterator cend() const;

	private:

		BucketType mBucket;
		size_t mBucketSize{ 0 };
		size_t mSize{ 0 };
		HashFunctor mHashFunctor;
		EqualityFunctor mEqualityFunctor;
	};
}

#include "hashmap.inl"