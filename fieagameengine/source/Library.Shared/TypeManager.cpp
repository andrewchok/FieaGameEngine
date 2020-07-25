#include "pch.h"
#include "TypeManager.h"

namespace Library
{
	Hashmap<std::size_t, Vector<Signature>> TypeManager::mHashSignatureTable;

	void TypeManager::RegisterType(const std::size_t& typeId, const Vector<Signature>& signatures)
	{
		mHashSignatureTable.Insert(std::make_pair(typeId, signatures));
	}

	void TypeManager::RegisterType(const std::size_t& typeId, Vector<Signature>&& signatures)
	{
		mHashSignatureTable.Insert(std::make_pair(typeId, std::move(signatures)));
	}

	void TypeManager::RemoveType(std::size_t& typeId)
	{
		mHashSignatureTable.Remove(typeId);
	}

	const Vector<Signature>& TypeManager::GetSignatures(const std::size_t& typeId)
	{
		return mHashSignatureTable.At(typeId);
	}

	void TypeManager::Clear()
	{
		mHashSignatureTable.Clear();
	}
}
