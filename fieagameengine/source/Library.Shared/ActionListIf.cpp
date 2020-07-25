#include "pch.h"
#include "ActionListIf.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf() : 
		ActionListIf(std::string())
	{
	}

	ActionListIf::ActionListIf(const std::string& name) : 
		Action(TypeIdClass(), name)
	{
	}

	void ActionListIf::Update(WorldState& worldState)
	{
		worldState.action = this;

		int32_t& condition = (*this)[condtionIndex].Get<int32_t>();
		Datum& actionIf = (*this)[thenActionsIndex];
		Datum& actionElse = (*this)[elseActionsIndex];

		if (condition)
		{
			assert(actionIf.Get<Scope*>()->Is(Action::TypeIdClass()));
			static_cast<Action*>(actionIf.Get<Scope*>())->Update(worldState);
		}
		else
		{
			assert(actionElse.Get<Scope*>()->Is(Action::TypeIdClass()));
			static_cast<Action*>(actionElse.Get<Scope*>())->Update(worldState);
		}
	}

	void ActionListIf::SetCondition(const int32_t& condition)
	{
		mConditionValue = condition;
	}

	void ActionListIf::SetIfBlock(Action& ifAction)
	{
		Datum& datum = (*this)[thenActionsIndex];

		if (0 == datum.Size())
		{
			Adopt(ifAction, "ThenAction");
		}
		else
		{
			Scope* scope = datum.Get<Scope*>();
			scope->OrphanSelf();

			datum.Set(&ifAction);
		}
	}

	void ActionListIf::SetElseBlock(Action& elseAction)
	{
		Datum& datum = (*this)[elseActionsIndex];

		if (0 == datum.Size())
		{
			Adopt(elseAction, "ElseAction");
		}
		else
		{
			Scope* scope = datum.Get<Scope*>();
			scope->OrphanSelf();

			datum.Set(&elseAction);
		}
	}

	Vector<Signature> ActionListIf::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "Condition", Datum::DatumTypes::Integer, 1, offsetof(ActionListIf, mConditionValue) });
		signatures.PushBack({ "ThenAction", Datum::DatumTypes::Table, 0, 0 });
		signatures.PushBack({ "ElseAction", Datum::DatumTypes::Table, 0, 0 });

		return signatures;
	}

	gsl::owner<Scope*> ActionListIf::Clone() const
	{
		return new ActionListIf(*this);
	}

}