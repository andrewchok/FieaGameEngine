#include "pch.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() : 
		ActionList(std::string())
	{
	}

	ActionList::ActionList(const std::string& name) : 
		Action(TypeIdClass(), name)
	{
	}

	void ActionList::Update(WorldState& worldState)
	{
		worldState.action = this;

		Datum& actions = Actions();
		for (uint32_t i = 0; i < actions.Size(); ++i)
		{
			Scope& actionScope = actions[i];
			assert(actionScope.Is(Action::TypeIdClass()));

			Action& tempAction = static_cast<Action&>(actionScope);
			tempAction.Update(worldState);
		}
	}

	const std::string& ActionList::Name() const
	{
		return Action::Name();
	}

	void ActionList::SetName(const std::string& name)
	{
		Action::SetName(name);
	}

	Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = Factory<Scope>::Create(className)->As<Action>();

		if (action == nullptr)
		{
			return nullptr;
		}

		action->SetName(instanceName);
		Adopt(*action, "Actions");

		return action;
	}

	Datum& ActionList::Actions()
	{
		return (*this)[actionsIndex];
	}

	Vector<Signature> ActionList::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "Actions", Datum::DatumTypes::Table, 0, 0 });

		return signatures;
	}

	gsl::owner<Scope*> ActionList::Clone() const
	{
		return new ActionList(*this);;
	}
	
	ActionList::ActionList(RTTI::IdType typeId, const std::string& name) :
		Action(typeId, name)
	{
	}

}