#pragma once
#include "IEventSubscriber.h"
#include "IEventPublisher.h"

namespace Library
{
	class SubscriberFooUnsubSelf final : public IEventSubscriber
	{
	public:
		SubscriberFooUnsubSelf() = default;
		SubscriberFooUnsubSelf(const SubscriberFooUnsubSelf& rhs) = default;
		SubscriberFooUnsubSelf(SubscriberFooUnsubSelf&& rhs) = default;
		SubscriberFooUnsubSelf& operator=(const SubscriberFooUnsubSelf& rhs) = default;
		SubscriberFooUnsubSelf& operator=(SubscriberFooUnsubSelf&& rhs) = default;
		~SubscriberFooUnsubSelf() = default;

		void SetData(std::int32_t value);
		std::int32_t Data() const;
		virtual void Notify(const IEventPublisher& event) override;

	private:
		std::int32_t mValue{ 0 };
	};


}