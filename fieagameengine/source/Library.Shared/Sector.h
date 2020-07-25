#pragma once
#include "Attributed.h"
#include "Entity.h"

namespace Library
{
	class World;

	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

	public:

#pragma region Constructors, Assignments & Destructor:
		/// <summary>
		/// Default Constructor
		/// </summary>
		Sector();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit Sector(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Sector reference</param>
		Sector(const Sector& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Sector&&</param>
		/// <returns>Moved Sector</returns>
		Sector(Sector&& rhs) noexcept = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Sector reference</param>
		/// <returns>Copied Sector</returns>
		Sector& operator=(const Sector& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Sector&&</param>
		/// <returns>Moved Sector</returns>
		Sector& operator=(Sector&& rhs) noexcept = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Sector() = default;

#pragma endregion Constructors, Assignments & Destructor:
		/// <summary>
		/// returns a string with the name of the Sector
		/// </summary>
		/// <returns>string</returns>
		const std::string& Name() const;
		/// <summary>
		/// sets the name of the sector.
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

		//todo: do the const version of Entities

		/// <summary>
		///  returns the Datum reference that stores the contained entities in the sector
		/// </summary>
		/// <returns>Datum reference</returns>
		Datum& Entities();
		/// <summary>
		/// which takes an entity class name and its instance name, 
		/// and uses the entity factory to make a new object of the given type, 
		/// adopts the entity into the sector (i.e. Entity::SetSector), 
		/// and returns the address of the new entity.
		/// </summary>
		/// <param name="className">entity class name</param>
		/// <param name="instanceName">instance name</param>
		/// <returns>address of the new entity</returns>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// returns the address of the World which contains this object.
		/// </summary>
		/// <returns>World pointer</returns>
		World* GetWorld() const;
		/// <summary>
		/// takes the address of a World, and adopts the entity into that sector.
		/// </summary>
		/// <param name="world">World</param>
		void SetWorld(World* world);

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Create a clone of an Entity
		/// </summary>
		/// <returns>Cloned Entity</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();
	private:
		std::string mSectorName;
		const static inline std::size_t entitiesIndex = 2;
	};

	ConcreteFactory(Sector, Scope);
}