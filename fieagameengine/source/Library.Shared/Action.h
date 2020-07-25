#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "WorldState.h"


namespace Library
{
	/// <summary>
	/// Forward Declare Classes
	/// </summary>
	class Entity;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Action reference</param>
		Action(const Action& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Action&&</param>
		/// <returns>Moved Action</returns>
		Action(Action&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Action reference</param>
		/// <returns>Copied Action</returns>
		Action& operator=(const Action& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Action&&</param>
		/// <returns>Moved Action</returns>
		Action& operator=(Action&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// returns a string with the name of the action
		/// </summary>
		/// <returns>string</returns>
		const std::string& Name() const;
		/// <summary>
		/// sets the name of the action.
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// returns the address of the Entity which contains this object.
		/// </summary>
		/// <returns>Entity pointer</returns>
		Entity* GetEntity() const;
		/// <summary>
		/// takes the address of a Entity, and adopts the entity into that sector.
		/// </summary>
		/// <param name="entity">Entity</param>
		void SetEntity(Entity* entity);

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		virtual void Update(WorldState& worldState) = 0;

		/// <summary>
		/// Create a clone of an Action
		/// </summary>
		/// <returns>Cloned Entity</returns>
		virtual gsl::owner<Scope*> Clone() const = 0;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();
		
		virtual Datum* Find(const std::string& key) override;

	protected:
		/// <summary>
		/// Default Contructor
		/// </summary>
		/// <param name="typeId">RTTI Id</param>
		/// <param name="name">instance name</param>
		Action(RTTI::IdType typeId, const std::string& name);

		WorldState* GetWorldState();

		std::string mActionName;
	};
}