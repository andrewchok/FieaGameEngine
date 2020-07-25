#include "pch.h"
#include "ActionIncrement.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() : 
		Action(TypeIdClass(), std::string())
	{
	}

	ActionIncrement::ActionIncrement(const std::string& name) :
		Action(TypeIdClass(), name)
	{
	}

	void ActionIncrement::Update(WorldState& worldState)
	{
		worldState.action = this;

		Datum& datum = *((*this).Find("Target"));
		Datum* target = Search(datum.Get<std::string>());

		if (nullptr != target && (Datum::DatumTypes::Integer == target->Type()))
		{
			++(target->Get<std::int32_t>());
		}
	}

	Vector<Signature> ActionIncrement::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "Target", Datum::DatumTypes::String, 1, offsetof(ActionIncrement, mTarget) });

		return signatures;
	}

	gsl::owner<Scope*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

}