#pragma once
#include "IEventPublisher.h"
#include "Vector.h"
#include <chrono>

using MilliSeconds = std::chrono::milliseconds;

namespace Library
{
	class GameTime;

	/// <summary>
	/// EventQueue System to enqueue and send events to EventSubscribers
	/// </summary>
	class EventQueue
	{
	public:
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using MilliSeconds = std::chrono::milliseconds;

		/// <summary>
		/// Default Constructor
		/// </summary>
		EventQueue() = default;
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const EventQueue reference</param>
		EventQueue(const EventQueue& rhs) = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a EventQueue&&</param>
		/// <returns>Moved EventQueue</returns>
		EventQueue(EventQueue&& rhs) = default;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const EventQueue reference</param>
		/// <returns>Copied EventQueue</returns>
		EventQueue& operator=(const EventQueue& rhs) = default;
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a EventQueue&&</param>
		/// <returns>Moved EventQueue</returns>
		EventQueue& operator=(EventQueue&& rhs) = default;
		/// <summary>
		/// Destructor
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// Given the address of an EventPublisher, 
		/// a GameTime (used to retrieve the current time), 
		/// and an optional delay time, enqueue the event.
		/// </summary>
		/// <param name="event">address of an EventPublisher</param>
		/// <param name="gameTime">GameTime</param>
		/// <param name="delay">delay time (defaults to MilliSeconds(0))</param>
		void Enqueue(std::shared_ptr<IEventPublisher> event, const GameTime& gameTime, const MilliSeconds& delay = MilliSeconds(0));

		/// <summary>
		/// Given the address of an EventPublisher, send the event immediately.
		/// </summary>
		/// <param name="event"> address of an EventPublisher</param>
		void Send(const std::shared_ptr<IEventPublisher>& event);

		/// <summary>
		/// Given the a GameTime, publish any queued events that have expired.
		/// </summary>
		/// <param name="game_time">GameTime</param>
		void Update(const GameTime& game_time);

		/// <summary>
		/// Clear the event queue, does not send expired events
		/// </summary>
		void Clear();

		/// <summary>
		/// returns a boolean indicating the emptiness of the queue.
		/// </summary>
		/// <returns>boolean</returns>
		bool IsEmpty() const;

		/// <summary>
		/// returns the number of events in the queue.
		/// </summary>
		/// <returns>std::size_t</returns>
		std::size_t Size() const;

		void ShrinkToFit();
		
	private:
		struct EnqueuedEvent
		{
			std::shared_ptr<IEventPublisher> EventPublisherPtr{ nullptr };
			TimePoint EnqueuedTime;
			MilliSeconds Delay{ MilliSeconds(0)};
		};

		mutable std::mutex mMutex;
		Vector<EnqueuedEvent> mEventQueue;
		Vector<EnqueuedEvent> mPendingEventQueue;
	};

}