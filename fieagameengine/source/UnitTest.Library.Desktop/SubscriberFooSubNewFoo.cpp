#include "pch.h"
#include "SubscriberFooSubNewFoo.h"
#include "Event.h"
#include "Foo.h"

namespace Library
{
	SubscriberFooSubNewFoo::SubscriberFooSubNewFoo(SubscriberFoo* subscriberFoo) :
		mSubscriberFoo(subscriberFoo)
	{
	}

	int32_t SubscriberFooSubNewFoo::Data() const
	{
		return mValue;
	}

	void SubscriberFooSubNewFoo::Notify(const IEventPublisher& event)
	{
		const Event<Foo>* foo = event.As<Event<Foo>>();
		if (foo)
		{
			mValue = foo->Message().Data();
			Event<Foo>::Subscribe(*mSubscriberFoo);
			Event<Foo>::Unsubscribe(*this);
		}
	}

}
