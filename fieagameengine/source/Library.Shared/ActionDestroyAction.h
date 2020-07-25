#pragma once
#include "Action.h"
#include "World.h"
#include "Factory.h"

namespace Library
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionDestroyAction();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ActionDestroyAction(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Sector reference</param>
		ActionDestroyAction(const ActionDestroyAction& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Sector&&</param>
		/// <returns>Moved Sector</returns>
		ActionDestroyAction(ActionDestroyAction&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Sector reference</param>
		/// <returns>Copied Sector</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Sector&&</param>
		/// <returns>Moved Sector</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~ActionDestroyAction() = default;

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Set Action to be destroyed by passing the name of the instance
		/// </summary>
		/// <param name="instanceName">instance name</param>
		void SetActionInstanceName(const std::string& instanceName);

		/// <summary>
		/// Set World member to pass action to be destroyed to Graveyard
		/// </summary>
		void SetWorld();
		/// <summary>
		/// Set World member to pass action to be destroyed to Graveyard
		/// </summary>
		/// <param name="world">world to set</param>
		void SetWorld(World* world);

		/// <summary>
		/// Create a clone of an ACtionDestroyAction
		/// </summary>
		/// <returns>Cloned ACtionDestroyAction</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

	private:
		std::string mActionInstanceName;

		World* mWorld{ nullptr};
	};

	ConcreteFactory(ActionDestroyAction, Scope)
}