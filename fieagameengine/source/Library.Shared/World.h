#pragma once
#include "Attributed.h"
#include "SList.h"
#include "Scope.h"
#include "TypeManager.h"
#include "EventQueue.h"
#include "Reaction.h"

namespace Library
{
	class Sector;
	class WorldState;

	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)

	public:

#pragma region Constructors, Assignments & Destructor:
		/// <summary>
		/// Default Constructor
		/// </summary>
		World();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit World(const std::string& name);
		World(const std::string& name, EventQueue& eventQueue);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const World reference</param>
		World(const World& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a World&&</param>
		/// <returns>Moved World</returns>
		World(World&& rhs) noexcept = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const World reference</param>
		/// <returns>Copied World</returns>
		World& operator=(const World& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a World&&</param>
		/// <returns>Moved World</returns>
		World& operator=(World&& rhs) noexcept = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~World() = default;

#pragma endregion Constructors, Assignments & Destructor:
		/// <summary>
		/// returns a string with the name of the world
		/// </summary>
		/// <returns>string</returns>
		const std::string& Name() const;
		/// <summary>
		/// sets the name of the world.
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// return a reference to the Datum containing Sectors
		/// </summary>
		/// <returns>Datum ref</returns>
		Datum& Sectors();
		/// <summary>
		/// instantiates a new sector, adopts it into the World (i.e. Sector::SetWorld) 
		/// and returns the address of the new sector.
		/// </summary>
		/// <param name="sectorName">sector name</param>
		/// <returns>address of the new sector</returns>
		Sector* CreateSector(const std::string& sectorName);

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Create a clone of an world
		/// </summary>
		/// <returns>Cloned world</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Get Graveyard storing actions to be destroyed
		/// </summary>
		/// <returns>SList of Scope pointers (actions)</returns>
		SList<Scope*>& GetGraveyard();
		void Bury(Scope* scope);
		
		WorldState& GetWorldState();

		EventQueue& GetEventQueue();
		
	private:
		std::string mWorldName;
		WorldState* mWorldState{ nullptr };
		EventQueue* mEventQueue{ nullptr };
		SList<Scope*> mGraveyard;
		
		const static inline std::size_t sectorsIndex = 2;
	};
}