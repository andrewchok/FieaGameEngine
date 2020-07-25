#include "pch.h"
#include "Action.h"
#include "Entity.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Action)

	const std::string& Action::Name() const
	{
		return mActionName;
	}

	void Action::SetName(const std::string& name)
	{
		mActionName = name;
	}

	Entity* Action::GetEntity() const
	{
		assert(GetParent()->Is(Entity::TypeIdClass()));

		return static_cast<Entity*>(GetParent());
	}

	void Action::SetEntity(Entity* entity)
	{
		if (entity == nullptr)
		{
			OrphanSelf();
		}

		if (entity != nullptr)
		{
			entity->Adopt(*this, "Actions");
		}
	}

	Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{"Name", Datum::DatumTypes::String, 1, offsetof(Action, mActionName)}
		};
	}

	Datum* Action::Find(const std::string& key)
	{
		WorldState* worldState = GetWorldState();
		if (worldState != nullptr && !(worldState->GetCallStack().IsEmpty()))
		{
			Scope& scopeStackFrame = worldState->GetCallStack().Top();

			auto found = scopeStackFrame.Find(key);
			if (found != nullptr)
			{
				return found;
			}
		}

		return Scope::Find(key);
	}
	
	Action::Action(RTTI::IdType typeId, const std::string& name) :
		Attributed(typeId), mActionName(name)
	{
	}

	WorldState* Action::GetWorldState()
	{
		Scope* scope = this;
		while (scope->GetParent() != nullptr)
		{
			scope = scope->GetParent();
		}

		World* world = scope->As<World>();
		if (world != nullptr)
		{
			return &(world->GetWorldState());
		}

		return nullptr;
	}
	
}