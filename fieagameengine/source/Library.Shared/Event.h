#pragma once
#include "IEventPublisher.h"
#include <mutex>

namespace Library
{
	/// <summary>
	/// Template argument is the concrete message (payload) class.
	/// Derive from EventPublisher.
	/// This implies also that it must also implement the RTTI interface.
	/// </summary>
	template<typename T>
	class Event final : public IEventPublisher
	{
		RTTI_DECLARATIONS(Event, IEventPublisher);

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit Event(const T& payload);
		explicit Event(T&& payload);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Event reference</param>
		Event(const Event& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Event&&</param>
		/// <returns>Moved Event</returns>
		Event(Event&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Event reference</param>
		/// <returns>Copied Event</returns>
		Event& operator=(const Event& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Event&&</param>
		/// <returns>Moved Event</returns>
		Event& operator=(Event&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~Event() = default;

		/// <summary>
		/// (static) Given the address of an EventSubscriber, 
		/// add it to the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">address of an EventSubscriber</param>
		static void Subscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// (static) Given the address of an EventSubscriber, 
		/// remove it from the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">address of an EventSubscriber</param>
		static void Unsubscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// (static)  Unsubscribe all subscribers to this event type.
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// returns message object.
		/// </summary>
		/// <returns>T</returns>
		const T& Message() const;



	private:
		T mPayload;
		static std::mutex mMutex;
		static Vector<IEventSubscriber*> mSubscribersList;
	};
}

#include "Event.inl"