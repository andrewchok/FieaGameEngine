#include "pch.h"
#include "ActionCreateAction.h"
#include "ActionList.h"
#include "Entity.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() : 
		ActionCreateAction(std::string())
	{
	}

	ActionCreateAction::ActionCreateAction(const std::string& name) : 
		Action(TypeIdClass(), name)
	{
	}

	void ActionCreateAction::SetClassName(const std::string& className)
	{
		mClassName = className;
	}

	void ActionCreateAction::SetInstanceName(const std::string& instanceName)
	{
		mInstanceName = instanceName;
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		worldState.action = this;

		if (mClassName.empty() && mInstanceName.empty())
		{
			mClassName = (*this).Find("PrototypeName")->Get<std::string>();
			mInstanceName = (*this).Find("InstanceName")->Get<std::string>();
		}

		auto parent = GetParent();
		if (parent->Is(Entity::TypeIdClass()))
		{
			parent->As<Entity>()->CreateAction(mClassName, mInstanceName);
		}
		else if (parent->Is(ActionList::TypeIdClass()))
		{
			parent->As<ActionList>()->CreateAction(mClassName, mInstanceName);
		}
	}

	gsl::owner<Scope*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	Vector<Signature> ActionCreateAction::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "PrototypeName", Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, mClassName) });
		signatures.PushBack({ "InstanceName", Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, mInstanceName) });

		return signatures;
	}

}