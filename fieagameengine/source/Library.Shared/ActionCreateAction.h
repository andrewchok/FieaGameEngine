#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionCreateAction();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ActionCreateAction(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Sector reference</param>
		ActionCreateAction(const ActionCreateAction& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Sector&&</param>
		/// <returns>Moved Sector</returns>
		ActionCreateAction(ActionCreateAction&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Sector reference</param>
		/// <returns>Copied Sector</returns>
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Sector&&</param>
		/// <returns>Moved Sector</returns>
		ActionCreateAction& operator=(ActionCreateAction&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Set class name for factory to use
		/// </summary>
		/// <param name="className">class name</param>
		void SetClassName(const std::string& className);
		/// <summary>
		/// Set instance name for the created class to be called
		/// </summary>
		/// <param name="instanceName">instance name</param>
		void SetInstanceName(const std::string& instanceName);
		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Create a clone of an ActionCreateAction
		/// </summary>
		/// <returns>Cloned ActionCreateAction</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

	private:
		std::string mClassName;
		std::string mInstanceName;

	};

	ConcreteFactory(ActionCreateAction, Scope)
}