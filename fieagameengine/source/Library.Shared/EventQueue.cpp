#include "pch.h"
#include "EventQueue.h"
#include "GameTime.h"
#include <future>

namespace Library
{
	void EventQueue::Enqueue(std::shared_ptr<IEventPublisher> event, const GameTime& gameTime, const MilliSeconds& delay)
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		mPendingEventQueue.PushBack({ std::move(event), gameTime.CurrentTime(), delay });
	}

	void EventQueue::Send(const	std::shared_ptr<IEventPublisher>& event)
	{
		event->Deliver();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		if (!IsEmpty())
		{
			// add all pending events to main event queue
			for (auto& event : mPendingEventQueue)
			{
				mEventQueue.PushBack(std::move(event));
			}
			mPendingEventQueue.Resize(0);

			// create predicate to pass into std::partition()
			auto expression = [&gameTime](EnqueuedEvent event)
			{
				return (gameTime.CurrentTime() < (event.EnqueuedTime + event.Delay));
			};

			// Reorders the elements in the range [first, last) in such a way that all 
			// elements for which the predicate returns true precede the elements for 
			// which predicate returns false. Relative order of the elements is not preserved.

			auto partitionElement = std::partition(mEventQueue.begin(), mEventQueue.end(), expression);

			if (partitionElement != mEventQueue.end())
			{
				// iterate through all expired events and call deliver() on them, async-ly
				std::vector<std::future<void>> futures;
				
				for (auto it = partitionElement; it != mEventQueue.end(); ++it)
				{
					auto& expiredEvent = (*it).EventPublisherPtr;
					futures.emplace_back(std::async(std::launch::async, [&expiredEvent]()
					{
						expiredEvent->Deliver();
					}));
				}
				
				for (auto& f : futures)
				{
					f.get();
				}
												
				// resize the vector remove expired events
				auto end = mEventQueue.end() - 1;
				mEventQueue.Remove(partitionElement, end);
			}
		}
	}

	void EventQueue::Clear()
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		mPendingEventQueue.Clear();
		mEventQueue.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		return mEventQueue.IsEmpty() && mPendingEventQueue.IsEmpty();
	}

	std::size_t EventQueue::Size() const
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		return mEventQueue.Size() + mPendingEventQueue.Size();
	}

	void EventQueue::ShrinkToFit()
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		mEventQueue.ShrinkToFit();
		mPendingEventQueue.ShrinkToFit();
	}
}