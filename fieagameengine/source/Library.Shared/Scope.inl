#include "Scope.h"

namespace Library
{	
	inline bool Scope::IsEmpty() const
	{
		return mOrderedVector.IsEmpty();
	}

	inline size_t Scope::Size() const
	{
		return mOrderedVector.Size();
	}

	inline size_t Scope::Capacity() const
	{
		return mOrderedVector.Capacity();
	}
	   
	inline Scope* Scope::OrphanSelf()
	{
		if (mParent != nullptr)
		{
			auto [foundDatum, foundAt] = FindContainedScope(*this);
			assert(foundDatum != nullptr);
			foundDatum->RemoveAt<Scope*>(foundAt);
			mParent = nullptr;
		}

		return this;
	}

	inline const Scope::OrderedVector& Scope::GetOrderedVector() const
	{
		return mOrderedVector;
	}
}