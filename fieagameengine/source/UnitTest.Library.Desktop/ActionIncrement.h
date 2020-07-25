#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)
	public:
		ActionIncrement();
		explicit ActionIncrement(const std::string& name);
		ActionIncrement(const ActionIncrement& rhs) = default;
		ActionIncrement(ActionIncrement&& rhs) = default;
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;
		ActionIncrement& operator=(ActionIncrement&& rhs) = default;
		virtual ~ActionIncrement() = default;

		virtual void Update(WorldState& world_state) override;

		static Vector<Signature> Signatures();

		virtual gsl::owner<Scope*> Clone() const override;

	private:
		std::string mTarget;
	};

	ConcreteFactory(ActionIncrement, Scope)
}