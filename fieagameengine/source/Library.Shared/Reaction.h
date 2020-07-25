#pragma once
#include "ActionList.h"
#include "IEventSubscriber.h"

namespace Library
{
	class Reaction : public ActionList, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		virtual ~Reaction() = default;
			
		/// <summary>
		/// Overriden to do nothing, call ActionList::Update instead
		/// </summary>
		/// <param name="">WorldState</param>
		virtual void Update(WorldState&) override;

		/// <summary>
		/// Get ActionList Signatures
		/// </summary>
		/// <returns>list of ActionList Signatures</returns>
		static Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// Default Constructor
		/// </summary>
		Reaction() = delete;
		/// <summary>
		/// Default Constructor
		/// </summary>
		/// <param name="typeId">RTTI Id</param>
		/// <param name="name">name</param>
		Reaction(RTTI::IdType typeId, const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Reaction reference</param>
		Reaction(const Reaction& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Reaction&&</param>
		/// <returns>Moved Reaction</returns>
		Reaction(Reaction && rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Reaction reference</param>
		/// <returns>Copied Reaction</returns>
		Reaction& operator=(const Reaction & rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Reaction&&</param>
		/// <returns>Moved Reaction</returns>
		Reaction& operator=(Reaction && rhs) = default;
	};
}