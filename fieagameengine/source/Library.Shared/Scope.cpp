#include "pch.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(std::size_t capacity)
	{
		mOrderedVector.Reserve(capacity);
	}

	Scope::Scope(const Scope& rhs) :
		mOrderedVector(rhs.mOrderedVector.Size())
	{
		for (const auto entry : rhs.mOrderedVector)
		{
			const std::string& key = entry->first;
			const Datum& existingDatum = entry->second;
			Datum& newDatum = Append(key);

			if (existingDatum.Size() > 0 && existingDatum.Get<Scope*>() == &rhs)
			{
				continue;
			}

			if (existingDatum.Type() == Datum::DatumTypes::Table)
			{
				newDatum.SetType(Datum::DatumTypes::Table);
				newDatum.Reserve(existingDatum.Size());
				for (size_t i = 0; i < existingDatum.Size(); ++i)
				{					
					Scope* child = existingDatum[i].Clone();
					child->mParent = this;
					newDatum.PushBack(child);
				}
			}
			else
			{
				newDatum = existingDatum;
			}
		}
	}

	Scope::Scope(Scope&& rhs) noexcept :
		mParent(rhs.mParent), 
		mOrderedVector(std::move(rhs.mOrderedVector)),
		mHashTable(std::move(rhs.mHashTable))
	{
		rhs.OrphanSelf();

	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			for (const auto entry : rhs.mOrderedVector)
			{
				const std::string& key = entry->first;
				const Datum& existingDatum = entry->second;
				Datum& newDatum = Append(key);

				if (existingDatum.Size() > 0 && existingDatum.Get<Scope*>() == &rhs)
				{
					continue;
				}

				if (existingDatum.Type() == Datum::DatumTypes::Table)
				{
					newDatum.SetType(Datum::DatumTypes::Table);
					newDatum.Reserve(existingDatum.Size());
					for (size_t i = 0; i < existingDatum.Size(); ++i)
					{
						Scope* child = existingDatum[i].Clone();
						child->mParent = this;
						newDatum.PushBack(child);
					}
				}
				else
				{
					newDatum = existingDatum;
				}
			}
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();

			mParent = rhs.mParent;
			mOrderedVector = std::move(rhs.mOrderedVector);
			mHashTable = std::move(rhs.mHashTable);
			rhs.OrphanSelf();
		}
		return *this;
	}

	Scope::~Scope()
	{
		OrphanSelf();
		Clear();
	}

	void Scope::Clear()
	{
		for (size_t i = 0; i < mOrderedVector.Size(); ++i)
		{
			Datum& existingDatum = mOrderedVector[i]->second;

			if (existingDatum.Type() == Datum::DatumTypes::Table)
			{
				for (size_t j = 0; j < existingDatum.Size(); ++j)
				{
					if (existingDatum.Get<Scope*>(j) == this)
					{
						break;
					}

					Scope* child = existingDatum.Get<Scope*>(j);
					child->mParent = nullptr;
					delete child;
				}
			}
		}

		mOrderedVector.Clear();
		mHashTable.Clear();
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		bool isEqual = (mParent == rhs.mParent && mOrderedVector.Size() == rhs.mOrderedVector.Size());
		if (isEqual)
		{
			for (size_t i = 0; i < rhs.Size(); ++i)
			{
				if (mOrderedVector[i] != rhs.mOrderedVector[i])
				{
					isEqual = false;
					break;
				}
			}
		}

		return isEqual;
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		const Scope* scope = rhs->As<Scope>();
		if (scope != nullptr)
		{
			return *scope == *this;
		}
		return false;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(operator==(rhs));
	}

#pragma region Element Access:

	Datum& Scope::operator[](size_t index)
	{
		return mOrderedVector.At(index)->second;
	}

	const Datum& Scope::operator[](size_t index) const
	{
		return mOrderedVector.At(index)->second;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	Datum* Scope::Find(const std::string& key)
	{
		return const_cast<Datum*>(FindHelper(key));
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		return FindHelper(key);
	}

	std::tuple<Datum*, size_t> Scope::FindContainedScope(const Scope& scope)
	{
		if (mParent == nullptr)
		{
			throw std::exception("Parent is nullptr");
		}

		for (auto it = mParent->mOrderedVector.begin(); it != mParent->mOrderedVector.end(); ++it)
		{
			if ((*it)->second.Type() == Datum::DatumTypes::Table)
			{
				for (std::size_t i = 0; i < ((*it)->second).Size(); ++i)
				{
					if (((*it)->second)[i] == scope)
					{
						return std::tuple<Datum*, size_t>(&(*it)->second, i);
					}
				}
			}
		}
		return std::tuple<Datum*, size_t>(nullptr, Size());
	}

	Datum* Scope::Search(const std::string& key)
	{
		return const_cast<Datum*>(SearchHelper(key));
	}

	const Datum* Scope::Search(const std::string& key) const
	{
		return SearchHelper(key);
	}

	Datum* Scope::Search(const std::string& key, Scope*& foundScope)
	{
		return const_cast<Datum*>(SearchHelper(key, &foundScope));
	}

	const Datum* Scope::Search(const std::string& key, Scope*& foundScope) const
	{
		return SearchHelper(key, &foundScope);
	}

#pragma region Element Access:

#pragma region Modifers:

	Datum& Scope::Append(const std::string& key)
	{
		auto [it, isAppended] = mHashTable.Insert({ key , Datum() });

		if (isAppended)
		{
			mOrderedVector.PushBack(&(*it));
		}

		return it->second;
	}

	Datum& Scope::Append(const std::string& key, const Datum& datum)
	{
		auto [it, isAppended] = mHashTable.Insert({ key , datum });

		if (isAppended)
		{
			mOrderedVector.PushBack(&(*it));
		}

		return it->second;
	}
	
	Scope& Scope::AppendScope(const std::string& key)
	{
		Datum& datum = Append(key);
		datum.SetType(Datum::DatumTypes::Table);
		
		Scope* newScope = new Scope();
		newScope->mParent = this;
		datum.PushBack(newScope);

		return *newScope;
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

#pragma endregion Modifers:

#pragma region Helpers:

	void Scope::Adopt(Scope& child, const std::string& key)
	{
		auto [it, isAppended] = mHashTable.Insert({ key , Datum() });
		Datum& datum = it->second;

		if (isAppended)
		{
			mOrderedVector.PushBack(&(*it));
			datum.InitialSetType<Scope*>();
		}
		else
		{
			if (!(it->second.Type() == Datum::DatumTypes::Unknown
				|| it->second.Type() == Datum::DatumTypes::Table)) 
			{
				throw std::runtime_error("bad things");
			}
		}

		child.OrphanSelf();
		child.mParent = this;
		datum.PushBack(&child);

	}

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	const Datum* Scope::FindHelper(const std::string& key) const
	{
		auto it = mHashTable.Find(key);
		if (it != mHashTable.end())
		{
			return &(it->second);
		}

		return nullptr;
	}

	const Datum* Scope::SearchHelper(const std::string& key, Scope** foundScope) const
	{
		if(foundScope != nullptr) *foundScope = const_cast<Scope*>(this);

		auto found = Find(key);

		if (found == nullptr && mParent != nullptr)
		{
			found = mParent->SearchHelper(key, foundScope);
		}

		return found;
	}

#pragma endregion Helpers:


}