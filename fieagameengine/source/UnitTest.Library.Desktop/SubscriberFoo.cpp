#include "pch.h"
#include "SubscriberFoo.h"
#include "Event.h"
#include "Foo.h"

namespace Library
{

	SubscriberFoo::SubscriberFoo(std::int32_t value) :
		mValue(value)
	{
	}

	void SubscriberFoo::SetData(std::int32_t value)
	{
		mValue = value;
	}

	int32_t SubscriberFoo::Data() const
	{
		return mValue;
	}

	void SubscriberFoo::Notify(const IEventPublisher& event)
	{
		const Event<Foo>* foo = event.As<Event<Foo>>();
		if (foo)
		{
			mValue = foo->Message().Data();
		}
	}

}
