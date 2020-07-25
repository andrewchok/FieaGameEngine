#pragma once
namespace Library
{
	class IEventPublisher;

	/// <summary>
	/// Abstract Base class for EventSubscriber
	/// </summary>
	class IEventSubscriber
	{
	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~IEventSubscriber() = default;

		/// <summary>
		/// pure virtual method that accepts the address of an EventPublisher 
		/// (the actual argument will be the event itself). 
		/// Your handlers will use the RTTI interface to verify actual the event type.
		/// </summary>
		/// <param name="event">address of an EventPublisher</param>
		virtual void Notify(const IEventPublisher& event) = 0;

	protected:
		/// <summary>
		/// Default Constructor
		/// </summary>
		IEventSubscriber() = default;
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const IEventSubscriber reference</param>
		IEventSubscriber(const IEventSubscriber& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a IEventSubscriber&&</param>
		/// <returns>Moved IEventSubscriber</returns>
		IEventSubscriber(IEventSubscriber&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const IEventSubscriber reference</param>
		/// <returns>Copied IEventSubscriber</returns>
		IEventSubscriber& operator=(const IEventSubscriber& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a IEventSubscriber&&</param>
		/// <returns>Moved IEventSubscriber</returns>
		IEventSubscriber& operator=(IEventSubscriber&& rhs) = default;
	};

}