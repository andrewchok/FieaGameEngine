#include "pch.h"
#include "CppUnitTest.h"
#include "Event.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "EventQueue.h"
#include "Factory.h"
#include "GameTime.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "JsonTableParseHelper.h"
#include "ActionEvent.h"
#include "ActionIncrement.h"
#include "Foo.h"
#include "Bar.h"
#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;
using namespace std::chrono_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<ReactionAttributed>(ReactionAttributed* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionEvent>(ActionEvent* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	std::wstring ToString<EventMessageAttributed>(EventMessageAttributed* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	std::wstring ToString<ReactionAttributed>(const ReactionAttributed& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeManager::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeManager::RegisterType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::RegisterType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::RegisterType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeManager::RegisterType(Reaction::TypeIdClass(), Reaction::Signatures());
			TypeManager::RegisterType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
			TypeManager::RegisterType(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
			TypeManager::RegisterType(ActionEvent::TypeIdClass(), ActionEvent::Signatures());

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
#pragma region RTTI tests
			   
		TEST_METHOD(ReactionAttributedRTTITest)
		{
			ReactionAttributed reactionAttributed;
			RTTI* rtti = &reactionAttributed;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));

			Assert::IsTrue(rtti->Is("Reaction"s));
			Assert::IsTrue(rtti->Is("ActionList"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsTrue(rtti->Is(ReactionAttributed::TypeIdClass()));
			
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ReactionAttributed::TypeIdClass(), rtti->TypeIdInstance());

			Bar* bar = rtti->As<Bar>();
			Assert::IsNull(bar);

			ReactionAttributed* reactionAttributedptr = rtti->As<ReactionAttributed>();
			Assert::IsNotNull(reactionAttributedptr);
			Assert::AreEqual(&reactionAttributed, reactionAttributedptr);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ReactionAttributed::TypeIdClass());
			Assert::IsNotNull(r);

			// cleanup
			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ActionEventRTTITest)
		{
			ActionEvent actionEvent;
			RTTI* rtti = &actionEvent;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));

			Assert::IsTrue(rtti->Is("ActionEvent"s));
			Assert::IsTrue(rtti->Is("Action"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsTrue(rtti->Is(ActionEvent::TypeIdClass()));

			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionEvent::TypeIdClass(), rtti->TypeIdInstance());

			Bar* bar = rtti->As<Bar>();
			Assert::IsNull(bar);

			ActionEvent* actionEventPtr = rtti->As<ActionEvent>();
			Assert::IsNotNull(actionEventPtr);
			Assert::AreEqual(&actionEvent, actionEventPtr);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionEvent::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(EventMessageAttributedRTTITest)
		{
			EventMessageAttributed eventMessageAttributed;
			RTTI* rtti = &eventMessageAttributed;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));

			Assert::IsTrue(rtti->Is("EventMessageAttributed"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsTrue(rtti->Is(EventMessageAttributed::TypeIdClass()));

			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(EventMessageAttributed::TypeIdClass(), rtti->TypeIdInstance());

			Bar* bar = rtti->As<Bar>();
			Assert::IsNull(bar);

			EventMessageAttributed* eventMessageAttributedPtr = rtti->As<EventMessageAttributed>();
			Assert::IsNotNull(eventMessageAttributedPtr);
			Assert::AreEqual(&eventMessageAttributed, eventMessageAttributedPtr);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(EventMessageAttributed::TypeIdClass());
			Assert::IsNotNull(r);
		}

#pragma endregion	

#pragma region Constructor tests

		TEST_METHOD(ReactionAttributedConstructorTest)
		{
			ReactionAttributed reactionAttributed;
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("this"s));
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("SubType"s));

			// cleanup
			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ActionEventConstructorTest)
		{
			ActionEvent actionEvent;
			Assert::IsTrue(actionEvent.IsPrescribedAttribute("this"s));
			Assert::IsTrue(actionEvent.IsPrescribedAttribute("SubType"s));
			Assert::IsTrue(actionEvent.IsPrescribedAttribute("Delay"s));
		}

		TEST_METHOD(EventMessageAttributedConstructorTest)
		{
			EventMessageAttributed eventMessageAttributed;
			Assert::IsTrue(eventMessageAttributed.IsPrescribedAttribute("this"s));
			Assert::IsTrue(eventMessageAttributed.IsPrescribedAttribute("SubType"s));
		}

#pragma endregion

#pragma region Clone tests

		TEST_METHOD(ReactionAttributedCloneTest)
		{
			ReactionAttributedFactory reactionsAttributedFactory;

			World world("TestWorld");
			ReactionAttributed reaction("TestReaction"s); //= world.CreateReaction("ReactionAttributed")->As<ReactionAttributed>();
			reaction.SetSubtype("Integer"s);

			auto clone = reaction.Clone();
			ReactionAttributed* reactionAttributedClone = clone->As<ReactionAttributed>();
			Assert::IsNotNull(reactionAttributedClone);
			Assert::AreNotSame(reaction, *reactionAttributedClone);
			Assert::AreEqual("Integer"s, reactionAttributedClone->GetSubtype());
			Assert::AreEqual(reaction, *reactionAttributedClone);

			delete clone;

			// cleanup
			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ActionEventCloneTest)
		{
			ReactionAttributedFactory reactionsAttributedFactory;
			ActionEventFactory actionEventFactory;
			EntityFactory entityFactory;

			World world("TestWorld");
			ActionEvent* actionEvent = world.CreateSector("TestSector"s)->CreateEntity("Entity"s, "TestEntity"s)->CreateAction("ActionEvent", "TestAction")->As<ActionEvent>();
			actionEvent->AppendAuxiliaryAttribute("Number") = 5;
			actionEvent->SetSubtype("Integer"s);

			auto clone = actionEvent->Clone();
			ActionEvent* actionEventClone = clone->As<ActionEvent>();
			Assert::IsNotNull(actionEventClone);
			Assert::AreNotSame(actionEvent, actionEventClone);
			Assert::AreEqual("Integer"s, actionEventClone->GetSubtype());
			Assert::AreEqual(5, (*actionEventClone)["Number"].Get<std::int32_t>());

			delete clone;
		}

		TEST_METHOD(EventMessageAttributedCloneTest)
		{
			ReactionAttributedFactory reactionsAttributedFactory;
			ActionEventFactory actionEventFactory;

			World world("TestWorld");
			EventMessageAttributed eventMessageAttributed;
			eventMessageAttributed.SetSubType("Integer");
			eventMessageAttributed.SetWorld(world);

			auto clone = eventMessageAttributed.Clone();
			EventMessageAttributed* eventMessageAttributedClone = clone->As<EventMessageAttributed>();
			Assert::IsNotNull(eventMessageAttributedClone);
			Assert::AreNotSame(&eventMessageAttributed, eventMessageAttributedClone);
			Assert::AreEqual("Integer"s, eventMessageAttributedClone->GetSubType());
			Assert::IsTrue(&world == eventMessageAttributedClone->GetWorld());

			delete clone;
		}

#pragma endregion

		TEST_METHOD(ActionEventUpdateTest)
		{
			ReactionAttributedFactory reactionsAttributedFactory;
			EntityFactory entityFactory;
			ActionEventFactory actionEventFactory;
			ActionIncrementFactory actionIncrementFactory;

			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			WorldState worldState;
			worldState.SetGameTime(gameTime);
			EventQueue eventQueue;

			World world("TestWorld", eventQueue);
			Entity* entity = world.CreateSector("TestSector"s)->CreateEntity("Entity"s, "TestEntity"s);

			ReactionAttributed* reaction = entity->CreateAction("ReactionAttributed","TestReaction"s)->As<ReactionAttributed>();
			reaction->SetSubtype("Integer"s);
			Assert::AreEqual("Integer"s, reaction->GetSubtype());

			ActionIncrement* actionIncrement = reaction->CreateAction("ActionIncrement", "actionIncrement")->As<ActionIncrement>();
			(*actionIncrement)["Target"] = "Number2";
			(*actionIncrement)["Number1"] = 10;

			ActionEvent* actionEvent = entity->CreateAction("ActionEvent", "TestAction")->As<ActionEvent>();
			actionEvent->SetSubtype("Integer");
			actionEvent->SetDelay(0ms);
			actionEvent->AppendAuxiliaryAttribute("Number2") = 5;

			Assert::AreEqual("Integer"s, actionEvent->GetSubtype());
			Assert::IsTrue(0ms == actionEvent->GetDelay());

			world.Update(worldState);
			Assert::AreEqual(1_z, eventQueue.Size());
			Assert::AreEqual(1_z, world.GetEventQueue().Size());

			worldState.SetGameTime(gameTime);
			world.Update(worldState);
			Assert::AreEqual(5, (*actionEvent)["Number2"].Get<std::int32_t>());
			Assert::AreEqual(10, (*actionIncrement)["Number1"].Get<std::int32_t>());

			(*actionIncrement)["Target"] = "Number1";
			world.Update(worldState);
			Assert::AreEqual(11, (*actionIncrement)["Number1"].Get<std::int32_t>());

			// cleanup
			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ParsingFromFileTest)
		{
			SectorFactory sectorFactory;
			EntityFactory entityFactory;
			ActionIncrementFactory actionIncrementFactory;
			ReactionAttributedFactory reactionAttributedFactory;

			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			WorldState worldState;
			worldState.SetGameTime(gameTime);

			const std::string fileName = "Content\\ReactionTest.json";

			JsonTableParseHelper tableParseHelper;
			World world;
			JsonTableParseHelper::SharedData tableSharedData(world);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("Content\\ReactionTest.json"s, parseMaster.GetFileName());
			
			Assert::AreEqual("TestWorld"s, world.Name());

			Sector* sector1 = world["Sectors"].Get<Scope*>(0)->As<Sector>();
			Assert::AreEqual("TestSector1"s, sector1->Name());
			
			Entity* entity1_1 = (*sector1)["Entities"].Get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("TestEntity1_1"s, entity1_1->Name());

			ActionIncrement* actionIncrement = (*entity1_1)["Actions"].Get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*actionIncrement)["Target"].Get<std::string>(0));
			Assert::AreEqual(2019, (*actionIncrement)["Number"].Get<std::int32_t>(0));

			ReactionAttributed* reactionAttributed = (*entity1_1)["Actions"].Get<Scope*>(1)->As<ReactionAttributed>();
			Assert::AreEqual("integer"s, (*reactionAttributed)["SubType"].Get<std::string>(0));

			ActionIncrement* reactionActionIncrement = (*reactionAttributed)["Actions"].Get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*reactionActionIncrement)["Target"].Get<std::string>(0));
			Assert::AreEqual(1999, (*reactionActionIncrement)["Number"].Get<std::int32_t>(0));

			world.Update(worldState);
			Assert::AreEqual(2020, (*actionIncrement)["Number"].Get<std::int32_t>(0));
			Assert::AreEqual(1999, (*reactionActionIncrement)["Number"].Get<std::int32_t>(0));
			
			// cleanup
			Event<EventMessageAttributed>::UnsubscribeAll();
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ReactionTests::sStartMemState;
}
