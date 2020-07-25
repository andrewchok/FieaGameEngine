#include "pch.h"
#include "ActionIf.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionIf)

	ActionIf::ActionIf() : 
		ActionIf(std::string())
	{
	}

	ActionIf::ActionIf(const std::string& name) : 
		Action(TypeIdClass(), name)
	{
	}

	void ActionIf::Update(WorldState& worldState)
	{
		worldState.action = this;

		mConditionValue = (*this).Find("Condition")->Get<int32_t>();
		Datum& actionIf = *((*this).Find("ThenAction"));
		Datum& actionElse = *((*this).Find("ElseAction"));
				
		if (mConditionValue)
		{
			for (size_t i = 0; i < actionIf.Size(); ++i)
			{
				assert(actionIf.Get<Scope*>()->Is(Action::TypeIdClass()));
				static_cast<Action*>(actionIf.Get<Scope*>(i))->Update(worldState);
			}
		}
		else
		{
			for (size_t i = 0; i < actionElse.Size(); ++i)
			{
				assert(actionElse.Get<Scope*>()->Is(Action::TypeIdClass()));
				static_cast<Action*>(actionElse.Get<Scope*>(i))->Update(worldState);
			}
		}
	}

	void ActionIf::SetCondition(const int32_t& condition)
	{
		mConditionValue = condition;
	}

	void ActionIf::SetIfBlock(Action& ifAction)
	{
		Datum& datum = (*this)["ThenAction"];

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

	void ActionIf::SetElseBlock(Action& elseAction)
	{
		Datum& datum = (*this)["ElseAction"];

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

	Vector<Signature> ActionIf::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "Condition", Datum::DatumTypes::Integer, 1, offsetof(ActionIf, mConditionValue) });
		signatures.PushBack({ "ThenAction", Datum::DatumTypes::Table, 0, 0 });
		signatures.PushBack({ "ElseAction", Datum::DatumTypes::Table, 0, 0 });

		return signatures;
	}

	gsl::owner<Scope*> ActionIf::Clone() const
	{
		return new ActionIf(*this);
	}

}