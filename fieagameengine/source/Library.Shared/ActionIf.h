#pragma once
#include "Action.h"
#include "ActionList.h"
#include "Scope.h"
#include "Factory.h"

namespace Library
{
	class ActionIf final : public Action
	{
		RTTI_DECLARATIONS(ActionIf, Action)
	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionIf();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ActionIf(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const ActionIf reference</param>
		ActionIf(const ActionIf& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a ActionIf&&</param>
		/// <returns>Moved ActionIf</returns>
		ActionIf(ActionIf&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const ActionIf reference</param>
		/// <returns>Copied ActionIf</returns>
		ActionIf& operator=(const ActionIf& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a ActionIf&&</param>
		/// <returns>Moved ActionIf</returns>
		ActionIf& operator=(ActionIf&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~ActionIf() = default;

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Set Condition 0 = false, anything else is true
		/// </summary>
		/// <param name="condition">int32_t</param>
		void SetCondition(const std::int32_t& condition);

		/// <summary>
		/// Set the ThenActions block to the action given to execute when condition is true
		/// </summary>
		/// <param name="ifAction">action to execute</param>
		void SetIfBlock(Action& ifAction);
		/// <summary>
		/// Set the elseActions block to the action given to execute when condition is false
		/// </summary>
		/// <param name="ifAction">action to execute</param>
		void SetElseBlock(Action& elseAction);

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Create a clone of an ActionIf
		/// </summary>
		/// <returns>Cloned Entity</returns>
		virtual gsl::owner<Scope*> Clone() const override;

	private:
		std::int32_t mConditionValue;
	};

	ConcreteFactory(ActionIf, Scope)
}