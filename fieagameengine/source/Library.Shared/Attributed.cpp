#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace Library
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(std::size_t typeId) :
		Scope()
	{
		(*this)["this"] = this;
		Populate(typeId);
	}

	Attributed::Attributed(const Attributed& rhs) :
		Scope(rhs)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& rhs) noexcept:
		Scope(std::move(rhs))
	{
		(*this)["this"] = this;
		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);

			(*this)["this"] = this;
			UpdateExternalStorage(rhs.TypeIdInstance());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Scope::operator=(std::move(rhs));

			(*this)["this"] = this;
			UpdateExternalStorage(rhs.TypeIdInstance());
		}
		return *this;
	}

	bool Attributed::operator==(const Attributed& rhs) const
	{
		bool isEqual = (GetParent() == rhs.GetParent() && GetOrderedVector().Size() == rhs.GetOrderedVector().Size());
		if (isEqual)
		{
			// skip "this" attribute
			for (size_t i = 1; i < rhs.Size(); ++i)
			{
				if (GetOrderedVector()[i]->first != rhs.GetOrderedVector()[i]->first &&
					GetOrderedVector()[i]->second != rhs.GetOrderedVector()[i]->second)
				{
					isEqual = false;
					break;
				}
			}
		}

		return isEqual;
	}

	bool Attributed::Equals(const RTTI* rhs) const
	{
		const Attributed* attributed = rhs->As<Attributed>();
		if (attributed != nullptr)
		{
			return *attributed == *this;
		}
		return false;
	}

	bool Attributed::operator!=(const Attributed& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return Find(name) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		bool found = false;
		const OrderedVector attrPtr = Scope::GetOrderedVector();
		if ("this" == name)
		{
			return true;
		}

		Vector<Signature> signatures = TypeManager::GetSignatures(TypeIdInstance());
		for (auto& sig : signatures)
		{
			if (sig.mName == name)
			{
				found = true;
				break;
			}
		}
		return found;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::exception("Trying to append a Prescribed Attribute.");
		}
		return Append(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const HashTablePair& pair)
	{
		if (IsPrescribedAttribute(pair.first))
		{
			throw std::exception("Trying to append a Prescribed Attribute.");
		}
		return Append(pair.first, pair.second);
	}

	const typename Scope::OrderedVector& Attributed::GetAttributes() const
	{
		return Scope::GetOrderedVector();
	}

	typename Scope::OrderedVector Attributed::GetPrescribedAttributes() const
	{
		Vector<Signature> signatures = TypeManager::GetSignatures(TypeIdInstance());
		OrderedVector pAttributes(Scope::GetOrderedVector());
		pAttributes.Resize(signatures.Size());

		return pAttributes;
	}

	typename Scope::OrderedVector Attributed::GetAuxiliaryAttributes() const
	{
		Vector<Signature> signatures = TypeManager::GetSignatures(TypeIdInstance());
		OrderedVector copyAttributes(Scope::GetOrderedVector());
		OrderedVector aAttributes;

		for (std::size_t i = signatures.Size() + 1; i < copyAttributes.Size(); ++i)
		{
			aAttributes.PushBack(copyAttributes[i]);
		}

		aAttributes.Resize(aAttributes.Size());

		return aAttributes;
	}

	void Attributed::ForEachAuxiliaryAttribute(AuxiliaryAttributeFunction func) const
	{
		const auto signatures = TypeManager::GetSignatures(TypeIdInstance());
		size_t auxiliaryAttributeBeginIndex = signatures.Size() + 1; // +1 for the "this" attribute
		for (size_t i = auxiliaryAttributeBeginIndex; i < GetAttributes().Size(); ++i)
		{
			func(*GetAttributes()[i]);
		}
	}

	void Attributed::Populate(std::size_t typeId)
	{
		Vector<Signature> signatures = TypeManager::GetSignatures(typeId);

		for (auto& sig : signatures)
		{
			Datum& datum = Append(sig.mName);
			datum.SetType(sig.mType);

			// set all storage to be external except Type Tables
			if (sig.mType != Datum::DatumTypes::Table)
			{
				void* offset = reinterpret_cast<uint8_t*>(this) + sig.mOffset;			
				datum.SetStorage(offset, sig.mSize);
			}
			else
			{
				if(sig.mSize > 0) datum.Reserve(sig.mSize);
			}
		}
	}

	void Attributed::UpdateExternalStorage(std::size_t typeId)
	{
		Vector<Signature> signatures = TypeManager::GetSignatures(typeId);

		for (auto& sig : signatures)
		{
			Datum* datum = Find(sig.mName);
			if (datum == nullptr)
			{
				throw std::runtime_error("Signature Name was not found.");
			}
			datum->SetType(sig.mType);

			// set all storage to be external except Type Tables
			if (sig.mType != Datum::DatumTypes::Table)
			{
				void* offset = reinterpret_cast<uint8_t*>(this) + sig.mOffset;
				datum->SetStorage(offset, sig.mSize);
			}
			else
			{
				if (sig.mSize > 0) datum->Reserve(sig.mSize);
			}
		}
	}

}