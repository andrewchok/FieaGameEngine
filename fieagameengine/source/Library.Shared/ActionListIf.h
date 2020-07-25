#pragma once
#include "Action.h"
#include "ActionList.h"
#include "Scope.h"
#include "Factory.h"

namespace Library
{
	class ActionListIf final : public Action
	{
		RTTI_DECLARATIONS(ActionListIf, Action)
	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionListIf();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ActionListIf(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const ActionListIf reference</param>
		ActionListIf(const ActionListIf& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a ActionListIf&&</param>
		/// <returns>Moved ActionListIf</returns>
		ActionListIf(ActionListIf&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const ActionListIf reference</param>
		/// <returns>Copied ActionListIf</returns>
		ActionListIf& operator=(const ActionListIf& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a ActionListIf&&</param>
		/// <returns>Moved ActionListIf</returns>
		ActionListIf& operator=(ActionListIf&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~ActionListIf() = default;

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
		/// Create a clone of an ActionListif
		/// </summary>
		/// <returns>Cloned Entity</returns>
		virtual gsl::owner<Scope*> Clone() const override;

	private:
		std::int32_t mConditionValue;

		const static inline std::size_t condtionIndex = 2;
		const static inline std::size_t thenActionsIndex = 3;
		const static inline std::size_t elseActionsIndex = 4;
	};

	ConcreteFactory(ActionListIf, Scope)
}