#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionList();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ActionList(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const ActionList reference</param>
		ActionList(const ActionList& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a ActionList&&</param>
		/// <returns>Moved ActionList</returns>
		ActionList(ActionList&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const ActionList reference</param>
		/// <returns>Copied ActionList</returns>
		ActionList& operator=(const ActionList& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a ActionList&&</param>
		/// <returns>Moved ActionList</returns>
		ActionList& operator=(ActionList&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~ActionList() = default;
		
		/// <summary>
		/// returns a string with the name of the actionlist
		/// </summary>
		/// <returns>string</returns>
		const std::string& Name() const;
		/// <summary>
		/// sets the name of the actionlist.
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// which takes an action class name and its instance name, 
		/// and uses the action factory to make a new object of the given type, 
		/// adopts the action into the actionlist, 
		/// and returns the address of the new action.
		/// </summary>
		/// <param name="className">action class name</param>
		/// <param name="instanceName">instance name</param>
		/// <returns>address of the new action</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);
		
		/// <summary>
		///  returns the Datum reference that stores the contained Actions in the actionlist
		/// </summary>
		/// <returns>Datum reference</returns>
		Datum& Actions();

		/// <summary>
		/// Create a clone of an actionlist
		/// </summary>
		/// <returns>Cloned actionlist</returns>
		virtual gsl::owner<Scope*> Clone() const override;
		
		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// Default construtor
		/// </summary>
		/// <param name="typeId"></param>
		/// <param name="name"></param>
		ActionList(RTTI::IdType typeId, const std::string& name);

	private:
		const static inline std::size_t actionsIndex = 2;
	};

	ConcreteFactory(ActionList, Scope)
}