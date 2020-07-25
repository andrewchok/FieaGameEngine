#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() : 
		Entity(std::string())
	{
	}

	Entity::Entity(const std::string& name) : 
		Attributed(TypeIdClass()), mEntityName(name)
	{
	}

	Entity::Entity(RTTI::IdType typeId) :
		Attributed(typeId)
	{
	}

	const std::string& Entity::Name() const
	{
		return mEntityName;
	}

	void Entity::SetName(const std::string& name)
	{
		mEntityName = name;
	}

	Sector* Entity::GetSector() const
	{
		assert(GetParent()->Is(Sector::TypeIdClass()));
		return static_cast<Sector*>(GetParent());
	}

	void Entity::SetSector(Sector* sector)
	{
		if (sector == nullptr)
		{
			OrphanSelf();
		}

		if (sector != nullptr)
		{
			sector->Adopt(*this, "Entities");
		}
	}

	Datum& Entity::Actions()
	{
		return (*this)[actionsIndex];
	}

	Action* Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = Factory<Scope>::Create(className)->As<Action>();

		if (action == nullptr)
		{
			return nullptr;
		}

		action->SetName(instanceName);
		action->SetEntity(this);

		return action;
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.entity = this;
		
		Datum& actions = Actions();
		for (uint32_t i = 0; i < actions.Size(); ++i)
		{
			Scope& actionScope = actions[i];
			assert(actionScope.Is(Action::TypeIdClass()));

			Action& tempAction = static_cast<Action&>(actionScope);
			worldState.action = &tempAction;

			tempAction.Update(worldState);
		}

		worldState.action = nullptr;
	}

	gsl::owner<Scope*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(Entity, mEntityName) },
			{ "Actions", Datum::DatumTypes::Table, 0, 0 }
		};
	}
}