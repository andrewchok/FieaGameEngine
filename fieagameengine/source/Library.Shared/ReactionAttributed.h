#pragma once
#include "Reaction.h"
#include "Factory.h"

namespace Library
{
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ReactionAttributed();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit ReactionAttributed(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const ReactionAttributed reference</param>
		ReactionAttributed(const ReactionAttributed& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a ReactionAttributed&&</param>
		/// <returns>Moved ReactionAttributed</returns>
		ReactionAttributed(ReactionAttributed&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const ReactionAttributed reference</param>
		/// <returns>Copied ReactionAttributed</returns>
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a ReactionAttributed&&</param>
		/// <returns>Moved ReactionAttributed</returns>
		ReactionAttributed& operator=(ReactionAttributed&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~ReactionAttributed();

		/// <summary>
		/// method that accepts the address of an EventPublisher 
		/// (the actual argument will be the event itself). 
		/// Your handlers will use the RTTI interface to verify actual the event type.
		/// </summary>
		/// <param name="event">address of an EventPublisher</param>
		void Notify(const IEventPublisher& event) override;

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Create a clone of an ReactionAttributed
		/// </summary>
		/// <returns>Cloned ReactionAttributed</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Datum of type string indicating value of event subtype to which this Reaction responds.
		/// </summary>
		/// <param name="subtype">std::string</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// Datum of type string indicating value of event subtype to which this Reaction responds.
		/// </summary>
		/// <returns>std::string</returns>
		const std::string& GetSubtype() const;

	private:
		std::string mSubtype;
	};

	ConcreteFactory(ReactionAttributed, Scope)
}
