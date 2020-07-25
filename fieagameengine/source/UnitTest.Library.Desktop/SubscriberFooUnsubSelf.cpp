#include "pch.h"
#include "SubscriberFooUnsubSelf.h"
#include "Event.h"
#include "Foo.h"

namespace Library
{
	void SubscriberFooUnsubSelf::SetData(std::int32_t value)
	{
		mValue = value;
	}
	int32_t SubscriberFooUnsubSelf::Data() const
	{
		return mValue;
	}

	void SubscriberFooUnsubSelf::Notify(const IEventPublisher& event)
	{
		const Event<Foo>* foo = event.As<Event<Foo>>();
		if (foo)
		{
			mValue = foo->Message().Data();
			Event<Foo>::Unsubscribe(*this);
		}
	}

}
