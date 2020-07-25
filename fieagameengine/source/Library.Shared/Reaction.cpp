#include "pch.h"
#include "Reaction.h"

namespace Library
{
	RTTI_DEFINITIONS(Reaction)

	// override update to do nothing
	void Reaction::Update(WorldState&)
	{
	}

	Vector<Signature> Reaction::Signatures()
	{
		return ActionList::Signatures();
	}

	Reaction::Reaction(RTTI::IdType typeId, const std::string& name) :
		ActionList(typeId, name)
	{
	}
		
}