#include "pch.h"
#include "CppUnitTest.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "EventQueue.h"
#include "SubscriberFoo.h"
#include "SubscriberFooEnque.h"
#include "SubscriberFooSubNewFoo.h"
#include "SubscriberFooUnsubSelf.h"
#include "Factory.h"
#include "Foo.h"
#include "Bar.h"
#include "GameTime.h"
#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::Foo>(const Library::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Library::Foo>(const Library::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Foo>(Library::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
//#if defined(DEBUG) || defined(_DEBUG)
//			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
//			_CrtMemCheckpoint(&sStartMemState);
//#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
//#if defined(DEBUG) || defined(_DEBUG)
//			_CrtMemState endMemState, diffMemState;
//			_CrtMemCheckpoint(&endMemState);
//			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
//			{
//				_CrtMemDumpStatistics(&diffMemState);
//				Assert::Fail(L"Memory Leaks!");
//			}
//#endif
		}

		TEST_METHOD(RTTITest)
		{
			Foo foo;
			Event<Foo> eventFoo(foo);
			RTTI* rtti = &eventFoo;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is("IEventPublisher"s));
			Assert::IsTrue(rtti->Is(IEventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			Bar* bar = rtti->As<Bar>();
			Assert::IsNull(bar);

			IEventPublisher* eventPub = rtti->As<IEventPublisher>();
			Assert::IsNotNull(eventPub);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(IEventPublisher::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(EventSubscribeUnsubscribe)
		{
			Foo foo(25);
			SubscriberFoo subFoo1, subFoo2(10);
			Event<Foo> event(foo);
			Event<Foo>::Subscribe(subFoo1);

			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());

			event.Deliver();
			Assert::AreEqual(25, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());

			Event<Foo>::Unsubscribe(subFoo1);
			Event<Foo>::Subscribe(subFoo2);

			event.Deliver();
			Assert::AreEqual(25, subFoo1.Data());
			Assert::AreEqual(25, subFoo2.Data());

			Event<Foo>::Unsubscribe(subFoo2);

			// to handle cleanup of static memory
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventUnsubscribeAll)
		{
			Foo foo(25);
			SubscriberFoo subFoo1, subFoo2(10);
			Event<Foo>::Subscribe(subFoo1);
			Event<Foo>::Subscribe(subFoo2);
			Event<Foo> event(foo);

			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());

			event.Deliver();
			Assert::AreEqual(25, subFoo1.Data());
			Assert::AreEqual(25, subFoo2.Data());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventMessage)
		{
			Foo foo(25);
			Event<Foo> event(foo);
			Assert::AreEqual(foo, event.Message());
		}

		TEST_METHOD(EventQueueIsEmpty)
		{
			SubscriberFoo subFoo;
			Foo foo(25);
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			EventQueue queue;
			GameTime gameTime;

			Assert::IsTrue(queue.IsEmpty());
			queue.Enqueue(event, gameTime);
			Assert::IsFalse(queue.IsEmpty());
		}

		TEST_METHOD(EventQueueSize)
		{
			SubscriberFoo subFoo;
			Foo foo(25);
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			EventQueue queue;
			GameTime gameTime;

			Assert::AreEqual(queue.Size(), 0_z);

			queue.Enqueue(event, gameTime);
			Assert::AreEqual(queue.Size(), 1_z);

			queue.Clear();
			Assert::AreEqual(queue.Size(), 0_z);
		}

		TEST_METHOD(EventQueueSend)
		{
			SubscriberFoo subFoo1, subFoo2(10);
			Event<Foo>::Subscribe(subFoo1);
			Event<Foo>::Subscribe(subFoo2);
			Foo foo(50);
			EventQueue eventQueue;
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);

			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());

			eventQueue.Send(event);
			Assert::AreEqual(50, subFoo1.Data());
			Assert::AreEqual(50, subFoo2.Data());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueUpdate)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			GameTime tempGameTime;
			tempGameTime.SetCurrentTime(gameTime.CurrentTime());

			SubscriberFoo subFoo1, subFoo2(10);
			Event<Foo>::Subscribe(subFoo1);
			Event<Foo>::Subscribe(subFoo2);
			Foo foo1(50);
			Foo foo2(60);
			Foo foo3(9000);
			EventQueue eventQueue;

			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo1);
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo2);
			std::shared_ptr<Event<Foo>> event3 = std::make_shared<Event<Foo>>(foo3);

			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event1, gameTime, MilliSeconds(500));
			eventQueue.Enqueue(event2, gameTime, MilliSeconds(1000));
			eventQueue.Enqueue(event3, gameTime, MilliSeconds(1500));
			Assert::AreEqual(3_z, eventQueue.Size());

			eventQueue.Update(tempGameTime);
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(501));
			eventQueue.Update(tempGameTime);
			Assert::AreEqual(50, subFoo1.Data());
			Assert::AreEqual(50, subFoo2.Data());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(1000));
			eventQueue.Update(tempGameTime);
			Assert::AreEqual(60, subFoo1.Data());
			Assert::AreEqual(60, subFoo2.Data());

			Foo foo4(70);
			Foo foo5(80);
			std::shared_ptr<Event<Foo>> event4 = std::make_shared<Event<Foo>>(foo4);
			std::shared_ptr<Event<Foo>> event5 = std::make_shared<Event<Foo>>(foo5);

			Assert::AreEqual(1_z, eventQueue.Size());
			eventQueue.Enqueue(event4, gameTime, MilliSeconds(500));
			eventQueue.Enqueue(event5, gameTime, MilliSeconds(1000));
			Assert::AreEqual(3_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(1000));
			eventQueue.Update(tempGameTime);
			Assert::AreEqual(80, subFoo1.Data());
			Assert::AreEqual(80, subFoo2.Data());

			Event<Foo>::UnsubscribeAll();

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(1500));
			eventQueue.Update(tempGameTime);
			Assert::AreEqual(80, subFoo1.Data());
			Assert::AreEqual(80, subFoo2.Data());			
		}

		TEST_METHOD(PendingUnsubscribe)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			GameTime tempGameTime;
			tempGameTime.SetCurrentTime(gameTime.CurrentTime());

			SubscriberFooUnsubSelf subFoo1, subFoo2;
			Event<Foo>::Subscribe(subFoo1);
			Event<Foo>::Subscribe(subFoo2);
			Foo foo1(10);
			Foo foo2(20);
			EventQueue eventQueue;

			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo1);
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo2);

			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event1, gameTime, MilliSeconds(500));
			eventQueue.Enqueue(event2, gameTime, MilliSeconds(1000));
			Assert::AreEqual(2_z, eventQueue.Size());

			eventQueue.Update(tempGameTime);
			// nothing happens
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(0, subFoo2.Data());
			Assert::AreEqual(2_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(501));
			eventQueue.Update(tempGameTime);
			// subFoo1 & subFoo2 should be pending unsubscribed, because they both got Notified by event1
			Assert::AreEqual(10, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());
			Assert::AreEqual(1_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(1001));
			eventQueue.Update(tempGameTime);
			// subFoo1 & subFoo2 should be unsubscribed
			Assert::AreEqual(10, subFoo1.Data());
			Assert::AreEqual(10, subFoo2.Data());
			Assert::AreEqual(0_z, eventQueue.Size());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(PendingSubscribe)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			GameTime tempGameTime;
			tempGameTime.SetCurrentTime(gameTime.CurrentTime());

			SubscriberFoo subFoo2(12);
			SubscriberFooSubNewFoo subFoo1(&subFoo2);
			Event<Foo>::Subscribe(subFoo1);
			Foo foo1(11);
			Foo foo2(21);
			EventQueue eventQueue;

			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo1);
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo2);

			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event1, gameTime, MilliSeconds(500));
			eventQueue.Enqueue(event2, gameTime, MilliSeconds(1000));
			Assert::AreEqual(2_z, eventQueue.Size());

			eventQueue.Update(tempGameTime);
			// nothing happens
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(12, subFoo2.Data());
			Assert::AreEqual(2_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(501));
			eventQueue.Update(tempGameTime);
			// subFoo2 should be pending subscribe, subFoo1 should be pending unsubscribed
			Assert::AreEqual(11, subFoo1.Data());
			Assert::AreEqual(12, subFoo2.Data());
			Assert::AreEqual(1_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(1001));
			eventQueue.Update(tempGameTime);
			// subFoo2 data = 21, subFoo1 should be unsubscribed
			Assert::AreEqual(11, subFoo1.Data());
			Assert::AreEqual(21, subFoo2.Data());
			Assert::AreEqual(0_z, eventQueue.Size());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(PendingEventQueue)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			GameTime tempGameTime;
			tempGameTime.SetCurrentTime(gameTime.CurrentTime());

			EventQueue eventQueue;
			SubscriberFooEnque subFoo1(&eventQueue, &gameTime);
			SubscriberFoo subFoo2(12);
			Event<Foo>::Subscribe(subFoo1);
			Event<Foo>::Subscribe(subFoo2);
			Foo foo1;
			Foo foo2(21);

			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo1);
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo2);
			
			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event1, gameTime, MilliSeconds(500));
			eventQueue.Enqueue(event2, gameTime, MilliSeconds(1000));
			Assert::AreEqual(2_z, eventQueue.Size());

			eventQueue.Update(tempGameTime);
			// nothing happens
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(12, subFoo2.Data());
			Assert::AreEqual(2_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(501));
			eventQueue.Update(tempGameTime);
			// subFoo2 data = 0, subFoo1 should be pending unsubscribed, 
			// a new Event should have been queued to pending
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(0, subFoo2.Data());
			Assert::AreEqual(2_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime());
			eventQueue.Update(tempGameTime);
			// subFoo2 data = 2020, subFoo1 should be unsubscribed
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(2020, subFoo2.Data());
			Assert::AreEqual(1_z, eventQueue.Size());

			tempGameTime.SetCurrentTime(gameTime.CurrentTime() + MilliSeconds(1001));
			eventQueue.Update(tempGameTime);
			// subFoo2 data = 21
			Assert::AreEqual(0, subFoo1.Data());
			Assert::AreEqual(21, subFoo2.Data());
			Assert::AreEqual(0_z, eventQueue.Size());

			Event<Foo>::UnsubscribeAll();
		}


		TEST_METHOD(AsyncSubscribeUnsubscribe)
		{
			SubscriberFoo subFoos[1000];
			SubscriberFooUnsubSelf unsubFoos[500];

			for (int i = 0; i < 500; ++i)
			{
				subFoos[i].SetData(i);
				Event<Foo>::Subscribe(subFoos[i]);
				unsubFoos[i].SetData(i);
				Event<Foo>::Subscribe(unsubFoos[i]);
			}

			for (int i = 500; i < 1000; ++i)
			{
				subFoos[i].SetData(i);
				Event<Foo>::Subscribe(subFoos[i]);
			}

			Foo foo(2020);
			Event<Foo> fooEvent(foo);
			fooEvent.Deliver();

			for (int i = 0; i < 500; ++i)
			{
				Assert::AreEqual(2020, subFoos[i].Data());
				Assert::AreEqual(2020, unsubFoos[i].Data());
			}
			for (int i = 500; i < 1000; ++i)
			{
				Assert::AreEqual(2020, subFoos[i].Data());
			}

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncEnqueueUpdate)
		{
			EventQueue eventQueue;
			GameTime gameTime;

			SubscriberFoo subFoos[10];
			Vector<shared_ptr<SubscriberFooEnque>> subEnqueue;
			std::shared_ptr<SubscriberFooEnque> subEnqueuePtr[10];

			for (int i = 0; i < 10; ++i)
			{
				SubscriberFooEnque subscriber(&eventQueue, &gameTime);
				subEnqueuePtr[i] = std::make_shared<SubscriberFooEnque>(subscriber);
				subEnqueuePtr[i]->SetData(i);
				subEnqueue.PushBack(subEnqueuePtr[i]);
			}

			// Subscribe all subscribers
			for (int i = 0; i < 10; ++i)
			{
				subFoos[i].SetData(i);
				Event<Foo>::Subscribe(subFoos[i]);
				Event<Foo>::Subscribe(*subEnqueue[i]);
			}

			// Create and Enqueue Events in Event Queue.
			for (int i = 0; i < 10; ++i)
			{
				Foo temp(2019);
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(temp);
				eventQueue.Enqueue(event, gameTime, 0ms);
			}

			// Call Update to check queued subscribers
			gameTime.SetCurrentTime(gameTime.CurrentTime() + 500ms);
			eventQueue.Update(gameTime);

			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(2019, subFoos[i].Data());
				Assert::AreEqual(2019, subEnqueue[i]->Data());
			}

			// Enqueued event should now carry a Foo(2020) event, and subEnqueue array has been unsubscribed
			eventQueue.Update(gameTime);

			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(2020, subFoos[i].Data());
				Assert::AreEqual(2019, subEnqueue[i]->Data());
			}

			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTests::sStartMemState;
}
