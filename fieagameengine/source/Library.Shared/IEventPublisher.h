#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "IEventSubscriber.h"
#include <mutex>

namespace Library
{
	/// <summary>
	/// Abstract Base class for EventPublisher
	/// </summary>
	class IEventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(IEventPublisher, RTTI)
	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~IEventPublisher() = default;

		/// <summary>
		/// Notify all subscribers of this event.
		/// </summary>
		void Deliver() const;
		void UpdatePendings() const;
		void DeleteUnsubscribed() const;
	protected:
		struct SubscriberEntry
		{
			IEventSubscriber* EventSubscriberPtr{ nullptr };
			bool IsUnsubscribed{ false };
		};

		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit IEventPublisher(Vector<IEventSubscriber*>& subscribers, std::mutex& mutex);
		/// <summary>
		/// Default Constructor
		/// </summary>
		IEventPublisher() = delete;
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Entity reference</param>
		IEventPublisher(const IEventPublisher& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Entity&&</param>
		/// <returns>Moved Entity</returns>
		IEventPublisher(IEventPublisher&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Entity reference</param>
		/// <returns>Copied Entity</returns>
		IEventPublisher& operator=(const IEventPublisher& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Entity&&</param>
		/// <returns>Moved Entity</returns>
		IEventPublisher& operator=(IEventPublisher&& rhs) = default;


	private:
		std::mutex* mMutex{ nullptr };
		Vector<IEventSubscriber*>* mSubscribersList{ nullptr };
	};
}
