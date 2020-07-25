#include "pch.h"
#include "IEventPublisher.h"
#include <future>

namespace Library
{
	RTTI_DEFINITIONS(IEventPublisher)

	IEventPublisher::IEventPublisher(Vector<IEventSubscriber*>& subscribers, std::mutex& mutex) :
		mSubscribersList(&subscribers), mMutex(&mutex)
	{
	}
	
	void IEventPublisher::Deliver() const
	{
		std::vector<std::future<void>> futures;
		{
			std::scoped_lock<std::mutex> lock(*mMutex);
						
			for (auto& subscriber : *mSubscribersList)
			{
				futures.emplace_back(std::async(std::launch::async, [subscriber, this]()
				{
					subscriber->Notify(*this);
				}));
			}
		}

		for (auto& f : futures)
		{
			f.get();
		}
	}
	
}