#include "pch.h"
#include "SubscriberFooEnque.h"
#include "Event.h"
#include "Foo.h"

namespace Library
{
	SubscriberFooEnque::SubscriberFooEnque(EventQueue* eventQueue, GameTime* gameTime) :
		mEventQueue(eventQueue), mGameTime(gameTime)
	{
	}

	void SubscriberFooEnque::SetData(std::int32_t value)
	{
		mValue = value;
	}

	int32_t SubscriberFooEnque::Data() const
	{
		return mValue;
	}

	void SubscriberFooEnque::Notify(const IEventPublisher& event)
	{
		const Event<Foo>* foo = event.As<Event<Foo>>();
		if (foo)
		{
			mValue = foo->Message().Data();

			Foo f(2020);
			
			mEventQueue->Enqueue(std::make_shared<Event<Foo>>(f), *mGameTime);
			Event<Foo>::Unsubscribe(*this);
		}
	}

}

