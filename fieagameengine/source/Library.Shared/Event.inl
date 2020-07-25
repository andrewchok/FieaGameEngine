#pragma once
#include "Event.h"
#include "IEventSubscriber.h"

namespace Library
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	std::mutex Event<T>::mMutex;
	
	template<typename T>
	Vector<IEventSubscriber*> Event<T>::mSubscribersList;

	template<typename T>
	Event<T>::Event(const T& payload) : 
		IEventPublisher(mSubscribersList, mMutex), mPayload(payload)
	{
	}

	template<typename T>
	inline Event<T>::Event(T&& payload) :
		IEventPublisher(mSubscribersList, mMutex), mPayload(std::move(payload))
	{
	}

	template<typename T>
	inline void Event<T>::Subscribe(IEventSubscriber& subscriber)
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		mSubscribersList.PushBack(&subscriber);
	}

	template<typename T>
	inline void Event<T>::Unsubscribe(IEventSubscriber& subscriber)
	{
		std::scoped_lock<std::mutex> lock(mMutex);
				
		mSubscribersList.Remove(&subscriber);		
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		mSubscribersList.Clear();
		mSubscribersList.ShrinkToFit();
	}

	template<typename T>
	inline const T& Event<T>::Message() const
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		return mPayload;
	}
}