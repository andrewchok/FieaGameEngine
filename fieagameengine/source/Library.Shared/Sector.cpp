#include "pch.h"
#include "Sector.h"
#include "Factory.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector() : 
		Sector(std::string())
	{
	}

	Sector::Sector(const std::string& name) : 
		Attributed(TypeIdClass()), mSectorName(name)
	{
	}

	const std::string& Sector::Name() const
	{
		return mSectorName;
	}

	void Sector::SetName(const std::string& name)
	{
		mSectorName = name;
	}

	Datum& Sector::Entities()
	{
		return (*this)[entitiesIndex];
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		// todo remove the As cast and check later with IS assert
		Entity* entity = Factory<Scope>::Create(className)->As<Entity>();

		if (entity == nullptr)
		{
			//todo throw error that factory not registered
			return nullptr;
		}

		//todo do a IS assert
		entity->SetName(instanceName);
		entity->SetSector(this);
		return entity;
	}

	World* Sector::GetWorld() const
	{
		assert(GetParent()->Is(World::TypeIdClass()));
		return static_cast<World*>(GetParent());
	}

	void Sector::SetWorld(World* world)
	{
		if (world == nullptr)
		{
			OrphanSelf();
		}

		if (world != nullptr)
		{
			world->Adopt(*this, "Sectors");
		}
	}

	void Sector::Update(WorldState& worldState)
	{
		worldState.sector = this;

		Datum& entities = Entities();
		for (std::size_t i = 0; i < entities.Size(); ++i)
		{
			Scope& entityScope = entities[i];
			assert(entityScope.Is(Entity::TypeIdClass()));

			Entity& entity = static_cast<Entity&>(entityScope);
			worldState.entity = &entity;

			entity.Update(worldState);
		}

		worldState.entity = nullptr;
	}

	gsl::owner<Scope*> Sector::Clone() const
	{
		return new Sector(*this);
	}

	Vector<Signature> Sector::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(Sector, mSectorName) },
			{ "Entities", Datum::DatumTypes::Table, 0, 0 }
		};
	}
}