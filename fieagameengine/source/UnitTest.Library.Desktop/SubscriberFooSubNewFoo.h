#pragma once
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "SubscriberFoo.h"

namespace Library
{
	class SubscriberFooSubNewFoo final : public IEventSubscriber
	{
	public:
		SubscriberFooSubNewFoo() = default;
		SubscriberFooSubNewFoo(SubscriberFoo* subscriberFoo);
		SubscriberFooSubNewFoo(const SubscriberFooSubNewFoo& rhs) = default;
		SubscriberFooSubNewFoo(SubscriberFooSubNewFoo&& rhs) = default;
		SubscriberFooSubNewFoo& operator=(const SubscriberFooSubNewFoo& rhs) = default;
		SubscriberFooSubNewFoo& operator=(SubscriberFooSubNewFoo&& rhs) = default;
		~SubscriberFooSubNewFoo() = default;

		std::int32_t Data() const;
		virtual void Notify(const IEventPublisher& event) override;

	private:
		std::int32_t mValue{ 0 };
		SubscriberFoo* mSubscriberFoo;
	};
	
}