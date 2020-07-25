#include "pch.h"
#include "World.h"
#include "Sector.h"

namespace Library
{
	RTTI_DEFINITIONS(World)

	World::World() :
		Attributed(TypeIdClass())
	{
	}

	World::World(const std::string& name) :
		Attributed(TypeIdClass()), mWorldName(name)
	{
	}

	World::World(const std::string& name, EventQueue& eventQueue) : 
		Attributed(TypeIdClass()), mWorldName(name), mEventQueue(&eventQueue)
	{
	}

	const std::string& World::Name() const
	{
		return mWorldName;
	}

	void World::SetName(const std::string& name)
	{
		mWorldName = name;
	}

	Datum& World::Sectors()
	{
		return (*this)[sectorsIndex];
	}

	Sector* World::CreateSector(const std::string& sectorName)
	{
		Sector* sector = new Sector(sectorName);
		sector->SetWorld(this);
		return sector;
	}

	void World::Update(WorldState& worldState)
	{
		worldState.world = this;
		mWorldState = &worldState;

		if (mEventQueue != nullptr)
		{
			mEventQueue->Update(mWorldState->GetGameTime());
		}

		Datum& sectors = Sectors();
		for (std::size_t i = 0; i < sectors.Size(); ++i)
		{
			Scope& sectorScope = sectors[i];
			assert(sectorScope.Is(Sector::TypeIdClass()));

			Sector& sector = static_cast<Sector&>(sectorScope);
			worldState.sector = &sector;

			sector.Update(worldState);
		}

		// Delete Graveyard (marked actions to delete)
		for (auto& entry : mGraveyard)
		{
			entry->OrphanSelf();
			delete entry;
		}
		mGraveyard.Clear();

		worldState.sector = nullptr;
	}

	gsl::owner<Scope*> World::Clone() const
	{
		return new World(*this);
	}

	Vector<Signature> World::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(World, mWorldName) },
			{ "Sectors", Datum::DatumTypes::Table, 0, 0 }
		};
	}

	SList<Scope*>& World::GetGraveyard()
	{
		return mGraveyard;
	}

	void World::Bury(Scope* scope)
	{
		bool isAlreadyBuried = false;
		Scope* tempScope = scope;

		// search if self or parent(some ancestor) is buried already
		while (!isAlreadyBuried && tempScope->Is(Action::TypeIdClass()))
		{
			for (auto& entry : mGraveyard)
			{
				if (entry == tempScope)
				{
					isAlreadyBuried = true;
					break;
				}
			}

			tempScope = tempScope->GetParent();
		}

		if (!isAlreadyBuried)
		{
			mGraveyard.PushBack(scope);
		}
	}

	WorldState& World::GetWorldState()
	{
		return *mWorldState;
	}

	EventQueue& World::GetEventQueue()
	{
		return *mEventQueue;
	}

}