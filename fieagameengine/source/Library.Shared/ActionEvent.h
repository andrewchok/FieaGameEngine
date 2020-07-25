#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:
		using MilliSeconds = std::chrono::milliseconds;

		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionEvent();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ActionEvent(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const ActionEvent reference</param>
		ActionEvent(const ActionEvent& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a ActionEvent&&</param>
		/// <returns>Moved ActionEvent</returns>
		ActionEvent(ActionEvent&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const ActionEvent reference</param>
		/// <returns>Copied ActionEvent</returns>
		ActionEvent& operator=(const ActionEvent& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a ActionEvent&&</param>
		/// <returns>Moved ActionEvent</returns>
		ActionEvent& operator=(ActionEvent&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// takes a WorldState reference and updates it
		/// </summary>
		/// <param name="worldState">WorldState</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Create a clone of an ActionEvent
		/// </summary>
		/// <returns>Cloned ActionEvent</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Set a delay before event is processed
		/// </summary>
		/// <param name="milliseconds">Time in Milliseconds</param>
		void SetDelay(const MilliSeconds& milliseconds);

		/// <summary>
		///  Datum of type string indicating value of event subtype to which this Reaction responds.
		/// </summary>
		/// <param name="t_subtype">std::string</param>
		void SetSubtype(const std::string& t_subtype);

		/// <summary>
		/// Get a delay before event is processed
		/// </summary>
		/// <returns>Time in Milliseconds</returns>
		const MilliSeconds GetDelay() const;

		/// <summary>
		///  Datum of type string indicating value of event subtype to which this Reaction responds.
		/// </summary>
		/// <returns>std::string</returns>
		const std::string& GetSubtype() const;


	private:
		std::string mSubtype;
		std::size_t mDelay;
	};

	ConcreteFactory(ActionEvent, Scope)
}
