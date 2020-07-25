#pragma once
#include "Attributed.h"
#include "TypeManager.h"

namespace Library
{
	class World;

	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		EventMessageAttributed();
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit EventMessageAttributed(const std::string& name);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const EventMessageAttributed reference</param>
		EventMessageAttributed(const EventMessageAttributed& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a EventMessageAttributed&&</param>
		/// <returns>Moved EventMessageAttributed</returns>
		EventMessageAttributed(EventMessageAttributed&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const EventMessageAttributed reference</param>
		/// <returns>Copied EventMessageAttributed</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a EventMessageAttributed&&</param>
		/// <returns>Moved EventMessageAttributed</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~EventMessageAttributed() = default;

		/// <summary>
		/// Datum of type string indicating value of event subtype to which this Reaction responds. 
		/// </summary>
		/// <returns>std::string</returns>
		const std::string& GetSubType() const;

		/// <summary>
		/// Get the World Instance, the world that will process this message
		/// </summary>
		/// <returns>World pointer</returns>
		World* GetWorld() const;

		/// <summary>
		/// Datum of type string indicating value of event subtype to which this Reaction responds.
		/// </summary>
		/// <param name="subtype">std::string</param>
		void SetSubType(const std::string& subtype);

		/// <summary>
		/// Set the World Instance, the world that will process this message
		/// </summary>
		/// <param name="world">World Ref</param>
		void SetWorld(World& world);

		/// <summary>
		/// Contain Signature to Pass to TypeManager
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Create a clone of an EventMessageAttributed
		/// </summary>
		/// <returns>Cloned EventMessageAttributed</returns>
		virtual gsl::owner<Scope*> Clone() const override;

	private:
		std::string mSubtype;
		World* mWorld;
	};

}
