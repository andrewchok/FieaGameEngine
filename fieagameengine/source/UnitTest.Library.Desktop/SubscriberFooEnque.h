#pragma once
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "EventQueue.h"
#include "GameTime.h"

namespace Library
{
	class SubscriberFooEnque final : public IEventSubscriber
	{
	public:
		SubscriberFooEnque() = default;
		SubscriberFooEnque(EventQueue* eventQueue, GameTime* gameTime);
		SubscriberFooEnque(const SubscriberFooEnque& rhs) = default;
		SubscriberFooEnque(SubscriberFooEnque&& rhs) = default;
		SubscriberFooEnque& operator=(const SubscriberFooEnque& rhs) = default;
		SubscriberFooEnque& operator=(SubscriberFooEnque&& rhs) = default;
		~SubscriberFooEnque() = default;

		void SetData(std::int32_t value);

		std::int32_t Data() const;
		virtual void Notify(const IEventPublisher& event) override;

	private:
		std::int32_t mValue{ 0 };
		EventQueue* mEventQueue;
		GameTime* mGameTime;
	};

}

