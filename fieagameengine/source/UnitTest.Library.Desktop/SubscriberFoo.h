#pragma once
#include "IEventSubscriber.h"
#include "IEventPublisher.h"

namespace Library
{
	class SubscriberFoo final : public IEventSubscriber
	{
	public:
		SubscriberFoo() = default;
		SubscriberFoo(std::int32_t value);
		SubscriberFoo(const SubscriberFoo& rhs) = default;
		SubscriberFoo(SubscriberFoo&& rhs) = default;
		SubscriberFoo& operator=(const SubscriberFoo& rhs) = default;
		SubscriberFoo& operator=(SubscriberFoo&& rhs) = default;
		~SubscriberFoo() = default;

		void SetData(std::int32_t value);

		std::int32_t Data() const;
		virtual void Notify(const IEventPublisher& event) override;

	private:
		std::int32_t mValue{ 0 };
	};


}