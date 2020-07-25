#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "WorldState.h"
#include "Factory.h"

namespace Library
{
	class Sector;
	class Action;

	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:

#pragma region Constructors, Assignments & Destructor:
		/// <summary>
		/// Default Constructor
		/// </summary>
		Entity();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit Entity(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Entity reference</param>
		Entity(const Entity& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Entity&&</param>
		/// <returns>Moved Entity</returns>
		Entity(Entity&& rhs) noexcept = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Entity reference</param>
		/// <returns>Copied Entity</returns>
		Entity& operator=(const Entity& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Entity&&</param>
		/// <returns>Moved Entity</returns>
		Entity& operator=(Entity&& rhs) noexcept = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Entity() = default;

#pragma endregion Constructors, Assignments & Destructor:

		/// <summary>
		/// returns a string with the name of the entity
		/// </summary>
		/// <returns>string</returns>
		const std::string& Name() const;
		/// <summary>
		/// sets the name of the entity.
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// returns the address of the Sector which contains this object.
		/// </summary>
		/// <returns>Sector pointer</returns>
		Sector* GetSector() const;
		/// <summary>
		/// takes the address of a Sector, and adopts the entity into that sector.
		/// </summary>
		/// <param name="sector">Sector</param>
		void SetSector(Sector* sector);

		Datum& Actions();
		Action* CreateAction(const std::string& className, const std::string& instanceName);

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

	protected:
		Entity(RTTI::IdType typeId);

		std::string mEntityName;

	private:
		const static inline std::size_t actionsIndex = 2;
	};

	ConcreteFactory(Entity, Scope);
}