#include "pch.h"
#include "ActionDestroyAction.h"


namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyAction)

		ActionDestroyAction::ActionDestroyAction() : 
		ActionDestroyAction(std::string())
	{
	}

	ActionDestroyAction::ActionDestroyAction(const std::string& name) : 
		Action(TypeIdClass(), name), mActionInstanceName(name)
	{
	}

	void ActionDestroyAction::Update(WorldState& world_state)
	{
		world_state.action = this;

		SetWorld();
		mActionInstanceName = (*this).Find("ActionInstanceName")->Get<std::string>();

		if (!mActionInstanceName.empty() && mWorld != nullptr)
		{
			bool found = false;

			Scope* currentScope = this;
			Datum* datum = currentScope->Search("Actions");

			while (nullptr != datum && false == found)
			{
				for (std::size_t i = 0; i < datum->Size(); ++i)
				{
					Scope* actionScope = datum->Get<Scope*>(i);
					if ((*actionScope)["Name"].Get<std::string>() == mActionInstanceName)
					{
						// Implement Graveyard Concept
						mWorld->Bury(actionScope);
						
						found = true;
						break;
					}
				}

				currentScope = currentScope->GetParent();
				datum = currentScope->Find("Actions");
			}
		}

		mWorld->Bury(this);
	}

	void ActionDestroyAction::SetActionInstanceName(const std::string& instanceName)
	{
		SetWorld();
		mActionInstanceName = instanceName;
	}

	void ActionDestroyAction::SetWorld()
	{
		if (mWorld != nullptr) return;

		Scope* worldCheckScope = GetParent();
		while (worldCheckScope->GetParent() != nullptr)
		{
			worldCheckScope = worldCheckScope->GetParent();
		}

		if (worldCheckScope->Is(World::TypeIdClass()))
		{
			mWorld = worldCheckScope->As<World>();
		}
	}

	void ActionDestroyAction::SetWorld(World* world)
	{
		mWorld = world;
	}

	gsl::owner<Scope*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	Vector<Signature> ActionDestroyAction::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "ActionInstanceName", Datum::DatumTypes::String, 1, offsetof(ActionDestroyAction, mActionInstanceName) });

		return signatures;
	}


}