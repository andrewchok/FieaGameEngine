#include "hashmap.h"

namespace Library
{
#pragma region Iterator

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Iterator::Iterator(Hashmap* owner, typename ChainType::Iterator chainIterator, size_t index) :
		mOwner(owner), mChainIterator(chainIterator), mIndex(index)
	{
	}

	// pre-increment
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator& Hashmap<TKey, TData>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner pointing to nullptr");
		}

		if (mIndex >= mOwner->BucketSize() || mOwner->Size() == 0)
		{
			throw std::runtime_error("Incrementing Iterator to out of bounds");
		}

		bool hasIncrementedIterator = false;
		while (mIndex != mOwner->BucketSize())
		{
			if (mChainIterator != mOwner->mBucket.At(mIndex).end())
			{
				if (!hasIncrementedIterator)
				{
					hasIncrementedIterator = true;
					mChainIterator++;
				}

				if (mChainIterator != mOwner->mBucket.At(mIndex).end())
				{
					return *this;
				}
				
			}
			++mIndex;
			if (mIndex == mOwner->BucketSize())
			{
				break;
			}
			mChainIterator = mOwner->mBucket.At(mIndex).begin();
		}
		return *this;
	}

	// post-increment
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();

		return temp;
	}

	template<typename TKey, typename TData>
	inline const bool Hashmap<TKey, TData>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mChainIterator == rhs.mChainIterator) && (mIndex == rhs.mIndex);
	}

	template<typename TKey, typename TData>
	inline const bool Hashmap<TKey, TData>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::PairType& Hashmap<TKey, TData>::Iterator::operator*() const
	{
		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::PairType* Hashmap<TKey, TData>::Iterator::operator->() const
	{
		return &(*mChainIterator);
	}
	
#pragma endregion Iterator

#pragma region ConstIterator

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& it) :
		mOwner(it.mOwner), mChainIterator(it.mChainIterator), mIndex(it.mIndex)
	{
	}

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Hashmap* owner, typename ChainType::ConstIterator chainIterator, size_t index) :
		mOwner(owner), mChainIterator(chainIterator), mIndex(index)
	{
	}

	// pre-increment
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator& Hashmap<TKey, TData>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Owner pointing to nullptr");
		}

		if (mIndex >= mOwner->BucketSize() || mOwner->Size() == 0)
		{
			throw std::runtime_error("Incrementing Iterator to out of bounds");
		}

		bool hasIncrementedIterator = false;
		while (mIndex != mOwner->BucketSize())
		{
			if (mChainIterator != mOwner->mBucket.At(mIndex).end())
			{
				if (!hasIncrementedIterator)
				{
					hasIncrementedIterator = true;
					mChainIterator++;
				}

				if (mChainIterator != mOwner->mBucket.At(mIndex).end())
				{
					return *this;
				}

			}
			++mIndex;
			if (mIndex == mOwner->BucketSize())
			{
				break;
			}
			mChainIterator = mOwner->mBucket.At(mIndex).begin();
		}
		return *this;
	}

	// post-increment
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();

		return temp;
	}

	template<typename TKey, typename TData>
	inline const bool Hashmap<TKey, TData>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mChainIterator == rhs.mChainIterator) && (mIndex == rhs.mIndex);
	}

	template<typename TKey, typename TData>
	inline const bool Hashmap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return !(operator==(rhs));
	}
	
	template<typename TKey, typename TData>
	inline const typename Hashmap<TKey, TData>::PairType& Hashmap<TKey, TData>::ConstIterator::operator*() const
	{
		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline const typename Hashmap<TKey, TData>::PairType* Hashmap<TKey, TData>::ConstIterator::operator->() const
	{
		return &(*mChainIterator);
	}

#pragma endregion ConstIterator

#pragma region Hashmap

#pragma region Constructors, Destructor & Assignments

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Hashmap(size_t bucketSize, HashFunctor hashFunctor, EqualityFunctor equalityFunctor) :
		mBucketSize(bucketSize), mHashFunctor(hashFunctor), mEqualityFunctor(equalityFunctor)
	{
		mBucket.Resize(bucketSize);
	}
	
#pragma endregion Constructors, Destructor & Assignments

#pragma region Functions

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Hashmap(std::initializer_list<PairType> list, HashFunctor hashFunctor, EqualityFunctor equalityFunctor) :
		mHashFunctor(hashFunctor), mEqualityFunctor(equalityFunctor)
	{
		mBucket.Resize(list.size());

		for (auto& pair : list)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TData>
	inline std::tuple<typename Hashmap<TKey, TData>::Iterator, bool> Hashmap<TKey, TData>::Insert(const PairType& pair)
	{
		size_t hash = mHashFunctor(pair.first);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if (mEqualityFunctor((*it).first, pair.first)) break;
		}

		if (it == mBucket.At(bucket).end())
		{
			mBucket.At(bucket).PushFront(pair);
			++mSize;
			return std::tuple<Iterator, bool>(Hashmap<TKey, TData>::Iterator(this, mBucket.At(bucket).begin(), bucket), true);
		}
		else
		{
			return std::tuple<Iterator, bool>(Hashmap<TKey, TData>::Iterator(this, it, bucket), false);
		}
	}

	template<typename TKey, typename TData>
	inline std::tuple<typename Hashmap<TKey, TData>::Iterator, bool> Hashmap<TKey, TData>::Insert(PairType&& pair)
	{
		size_t hash = mHashFunctor(pair.first);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if (mEqualityFunctor((*it).first, pair.first)) break;
		}

		if (it == mBucket.At(bucket).end())
		{
			mBucket.At(bucket).PushFront(std::move(pair));
			++mSize;
			return std::tuple<Iterator, bool>(Hashmap<TKey, TData>::Iterator(this, mBucket.At(bucket).begin(), bucket), true);
		}
		else
		{
			return std::tuple<Iterator, bool>(Hashmap<TKey, TData>::Iterator(this, it, bucket), false);
		}
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Find(const TKey& key)
	{
		size_t hash = mHashFunctor(key);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if (mEqualityFunctor((*it).first, key))
			{
				return Iterator(this, it, bucket);
			}
		}

		return end();
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::Find(const TKey& key) const
	{
		size_t hash = mHashFunctor(key);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if (mEqualityFunctor((*it).first, key))
			{
				return ConstIterator(this, it, bucket);
			}
		}

		return cend();
	}

	template<typename TKey, typename TData>
	inline void Hashmap<TKey, TData>::Remove(const TKey& key)
	{
		Remove(Find(key));
	}

	template<typename TKey, typename TData>
	inline void Hashmap<TKey, TData>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Mismatch Owner: Invalid Iterator");
		}

		if (it != end())
		{
			mBucket.At(it.mIndex).Remove(it.mChainIterator);
			--mSize;
		}
	}

	template<typename TKey, typename TData>
	inline const bool Hashmap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		size_t hash = mHashFunctor(key);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if ((*it).first == key) break;
		}

		return it != mBucket.At(bucket).end();
	}

	template<typename TKey, typename TData>
	inline void Hashmap<TKey, TData>::Clear()
	{
		if (mSize == 0)
		{
			return;
		}

		for (ChainType& it : mBucket)
		{
			it.Clear();
		}

		mSize = 0;
	}

	template<typename TKey, typename TData>
	inline TData& Hashmap<TKey, TData>::operator[](const TKey& key)
	{
		size_t hash = mHashFunctor(key);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if ((*it).first == key) break;
		}

		if (it == mBucket.At(bucket).end())
		{
			mBucket.At(bucket).PushFront(std::pair<TKey, TData>(key, TData()));
			it = mBucket.At(bucket).begin();
			++mSize;
		}

		return (*it).second;
	}

	template<typename TKey, typename TData>
	inline const TData& Hashmap<TKey, TData>::operator[](const TKey& key) const
	{
		return this->At(key);
	}

#pragma endregion Functions

#pragma region Member Accessors

	template<typename TKey, typename TData>
	inline TData& Hashmap<TKey, TData>::At(const TKey& key)
	{
		size_t hash = mHashFunctor(key);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if ((*it).first == key) break;
		}

		if (it == mBucket.At(bucket).end())
		{
			throw std::runtime_error("Invalid Key: Key not found");
		}

		return (*it).second;
	}

	template<typename TKey, typename TData>
	inline const TData& Hashmap<TKey, TData>::At(const TKey& key) const
	{
		size_t hash = mHashFunctor(key);
		size_t bucket = hash % mBucket.Size();

		auto it = mBucket.At(bucket).begin();
		for (; it != mBucket.At(bucket).end(); ++it)
		{
			if ((*it).first == key) break;
		}

		if (it == mBucket.At(bucket).end())
		{
			throw std::runtime_error("Invalid Key: Key not found");
		}

		return (*it).second;
	}

	template<typename TKey, typename TData>
	inline const size_t Hashmap<TKey, TData>::Size() const
	{
		return mSize;
	}

	template<typename TKey, typename TData>
	inline const size_t Hashmap<TKey, TData>::BucketSize() const
	{
		return mBucket.Size();
	}

#pragma endregion Member Accessors

#pragma region Iterator Accessors

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::begin()
	{
		Iterator it = Iterator(this, this->mBucket.At(0).begin(), 0);
		while (it.mIndex != it.mOwner->BucketSize())
		{
			if (it.mChainIterator != it.mOwner->mBucket.At(it.mIndex).end())
			{
				return it;				
			}

			++(it.mIndex);

			if (it.mIndex == it.mOwner->BucketSize())
			{
				break;
			}

			it.mChainIterator = it.mOwner->mBucket.At(it.mIndex).begin();
		}
		return it;
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::begin() const
	{
		ConstIterator it = ConstIterator(this, this->mBucket.At(0).begin(), 0);
		while (it.mIndex != it.mOwner->BucketSize())
		{
			if (it.mChainIterator != it.mOwner->mBucket.At(it.mIndex).end())
			{
				return it;
			}

			++(it.mIndex);

			if (it.mIndex == it.mOwner->BucketSize())
			{
				break;
			}

			it.mChainIterator = it.mOwner->mBucket.At(it.mIndex).begin();
		}
		return it;
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cbegin() const
	{
		return this->begin();
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::end()
	{
		return Iterator(this, this->mBucket.At(mBucket.Size() -1).begin(), mBucket.Size());
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::end() const
	{
		return ConstIterator(this, this->mBucket.At(mBucket.Size() - 1).begin(), mBucket.Size());
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cend() const
	{
		return this->end();
	}

#pragma endregion Iterator Accessors

#pragma endregion Hashmap
}