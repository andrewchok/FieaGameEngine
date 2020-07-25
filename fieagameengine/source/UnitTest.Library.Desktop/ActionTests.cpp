#include "pch.h"
#include "CppUnitTest.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "Factory.h"
#include "JsonTableParseHelper.h"
#include <fstream>
#include "Foo.h"
#include "DerivedFoo.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"

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
	
	template<>
	std::wstring ToString<Entity>( Entity* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
	{
	public:
		
		

		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeManager::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeManager::RegisterType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::RegisterType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::RegisterType(ActionIf::TypeIdClass(), ActionIf::Signatures());
			TypeManager::RegisterType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeManager::RegisterType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures());
			TypeManager::RegisterType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures());

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

		TEST_METHOD(Constructor)
		{
			ActionIncrement actionIncrement;
			Assert::IsTrue(actionIncrement.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionIncrement.IsPrescribedAttribute("Target"));
			ActionIncrement actionIncrement2("TestActionIncrement");
			Assert::AreEqual("TestActionIncrement"s, actionIncrement2.Name());

			ActionList actionList;
			Assert::IsTrue(actionList.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionList.IsPrescribedAttribute("Actions"));
			ActionList actionList2("TestActionList");
			Assert::AreEqual("TestActionList"s, actionList2.Name());

			ActionIf actionIf;
			Assert::IsTrue(actionIf.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionIf.IsPrescribedAttribute("Condition"));
			Assert::IsTrue(actionIf.IsPrescribedAttribute("ThenAction"));
			Assert::IsTrue(actionIf.IsPrescribedAttribute("ElseAction"));
			ActionIf actionIf2("TestActionIf");
			Assert::AreEqual("TestActionIf"s, actionIf2.Name());

			ActionCreateAction actionCreateAction;
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute("PrototypeName"));
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute("InstanceName"));
			ActionIf actionCreateAction2("TestActionCreateAction");
			Assert::AreEqual("TestActionCreateAction"s, actionCreateAction2.Name());

			ActionDestroyAction actionDestroyAction;
			Assert::IsTrue(actionDestroyAction.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionDestroyAction.IsPrescribedAttribute("ActionInstanceName"));
			ActionIf actionDestroyAction2("TestActionDestroyAction");
			Assert::AreEqual("TestActionDestroyAction"s, actionDestroyAction2.Name());
		}

		TEST_METHOD(UpdateTest)
		{
			GameTime gameTime;
			WorldState worldState;
			worldState.SetGameTime(gameTime);

			// ActionIncrement Update Test
			{
				ActionIncrement actionIncrement;

				actionIncrement["TestInteger"] = 2019;
				actionIncrement["Target"] = "TestInteger";
				Assert::AreEqual(2019, actionIncrement["TestInteger"].Get<std::int32_t>());

				actionIncrement.Update(worldState);
				Assert::AreEqual(2020, actionIncrement["TestInteger"].Get<std::int32_t>());
			}

			// ActionList Update Test
			{
				ActionList actionList;
				Assert::AreEqual(actionList.Actions().Size(), 0_z);

				actionList["TestInteger"] = 17;

				ActionIncrement actionIncrement1;
				ActionIncrement actionIncrement2;

				actionIncrement1["TestInteger"] = 10;
				actionIncrement1["Target"] = "TestInteger";

				actionIncrement2["TestInteger"] = 20;
				actionIncrement2["Target"] = "TestInteger";

				actionList.Adopt(actionIncrement1, "Actions");
				actionList.Adopt(actionIncrement2, "Actions");

				Assert::AreEqual(actionList.Actions().Size(), 2_z);
				Assert::AreEqual(17, actionList["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(10, actionIncrement1["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(20, actionIncrement2["TestInteger"].Get<std::int32_t>());

				actionList.Update(worldState);
				Assert::AreEqual(17, actionList["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(11, actionIncrement1["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(21, actionIncrement2["TestInteger"].Get<std::int32_t>());
			}

			// ActionIf Update Test
			{
				EntityFactory entityFactory;
				SectorFactory sectorFactory;
				ActionListFactory actionListFactory;
				ActionIfFactory ActionIfFactory;

				World world("TestWorld");
				Entity* entity = world.CreateSector("TestSector")->CreateEntity("Entity", "TestEntity");
				ActionList* actionList = entity->CreateAction("ActionList", "TestAction")->As<ActionList>();
				Assert::AreEqual("TestAction"s, actionList->Name());

				ActionIf* actionIf = actionList->CreateAction("ActionIf", "TestActionIf")->As<ActionIf>();
				Assert::AreEqual("TestActionIf"s, actionIf->Name());

				actionIf->SetCondition(1);

				ActionIncrement* actionIncrement1 = new ActionIncrement();
				(*actionIncrement1)["TestInteger"] = 10;
				(*actionIncrement1)["Target"] = "TestInteger";
				actionIf->SetIfBlock(*actionIncrement1);

				ActionIncrement* actionIncrement2 = new ActionIncrement();
				(*actionIncrement2)["TestInteger"] = 20;
				(*actionIncrement2)["Target"] = "TestInteger";
				actionIf->SetElseBlock(*actionIncrement2);

				Assert::AreEqual(10, (*actionIncrement1)["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(20, (*actionIncrement2)["TestInteger"].Get<std::int32_t>());

				entity->Update(worldState);
				Assert::AreEqual(11, (*actionIncrement1)["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(20, (*actionIncrement2)["TestInteger"].Get<std::int32_t>());

				actionIf->SetCondition(0);

				entity->Update(worldState);
				Assert::AreEqual(11, (*actionIncrement1)["TestInteger"].Get<std::int32_t>());
				Assert::AreEqual(21, (*actionIncrement2)["TestInteger"].Get<std::int32_t>());
			}
			
			// ActionCreateAction Update Test
			{
				EntityFactory entityFactory;
				SectorFactory sectorFactory;
				ActionListFactory actionListFactory;
				ActionIfFactory ActionIfFactory;
				ActionIncrementFactory actionIncrementFactory;
				ActionCreateActionFactory actionCreateActionFactory;


				World world("TestWorld");
				Entity* entity = world.CreateSector("TestSector")->CreateEntity("Entity", "TestEntity");
				ActionList* actionList = entity->CreateAction("ActionList", "TestActionList")->As<ActionList>();
				Assert::AreEqual("TestActionList"s, actionList->Name());
				Assert::AreEqual(0_z, actionList->Actions().Size());

				ActionCreateAction* actionCreateAction = actionList->CreateAction("ActionCreateAction", "TestActionCreateAction")->As<ActionCreateAction>();
				Assert::AreEqual("TestActionCreateAction"s, actionCreateAction->Name());
				Assert::AreEqual(1_z, actionList->Actions().Size() );

				actionCreateAction->SetClassName("ActionIncrement");
				actionCreateAction->SetInstanceName("TestActionIncrement");

				entity->Update(worldState);

				Assert::AreEqual(2_z, actionList->Actions().Size());
				Assert::IsTrue(actionList->Actions().Get<Scope*>(0)->Is(ActionCreateAction::TypeIdClass()));
				Assert::IsTrue(actionList->Actions().Get<Scope*>(1)->Is(ActionIncrement::TypeIdClass()));

				Entity* entity2 = world.CreateSector("TestSector2")->CreateEntity("Entity", "TestEntity2");
				ActionCreateAction* actionCreateAction2 = entity2->CreateAction("ActionCreateAction", "TestActionCreateAction2")->As<ActionCreateAction>();
				Assert::AreEqual(1_z, entity2->Actions().Size());

				actionCreateAction2->SetClassName("ActionIncrement");
				actionCreateAction2->SetInstanceName("TestActionIncrement2");

				entity2->Update(worldState);
				Assert::AreEqual(2_z, entity2->Actions().Size());
				Assert::IsTrue(entity2->Actions().Get<Scope*>(0)->Is(ActionCreateAction::TypeIdClass()));
				Assert::IsTrue(entity2->Actions().Get<Scope*>(1)->Is(ActionIncrement::TypeIdClass()));
				Assert::IsFalse(entity2->Actions().Get<Scope*>(1)->Is(ActionDestroyAction::TypeIdClass()));
			}

			// ActionDestroyAction Update Test
			{
				EntityFactory entityFactory;
				SectorFactory sectorFactory;
				ActionListFactory actionListFactory;
				ActionIfFactory ActionIfFactory;
				ActionIncrementFactory actionIncrementFactory;
				ActionCreateActionFactory actionCreateActionFactory;
				ActionDestroyActionFactory actionDestroyActionFactory;

				World world("TestWorld");
				Entity* TestEntity = world.CreateSector("TestSector")->CreateEntity("Entity", "TestEntity");
				ActionList* TestActionList = TestEntity->CreateAction("ActionList", "TestActionList")->As<ActionList>();
				Assert::AreEqual("TestActionList"s, TestActionList->Name());

				ActionDestroyAction* TestActionDestroyAction = TestActionList->CreateAction("ActionDestroyAction", "TestActionDestroyAction")->As<ActionDestroyAction>();
				TestActionDestroyAction->SetActionInstanceName("TestActionDestroyAction");
				TestActionDestroyAction->SetWorld(&world);
				Assert::AreEqual("TestActionDestroyAction"s, TestActionDestroyAction->Name());
				Assert::AreEqual(1_z, TestActionList->Actions().Size());

				world.Update(worldState);
				Assert::AreEqual(0_z, TestActionList->Actions().Size());
			}

		}
		
		TEST_METHOD(EntityGetSetTest)
		{
			EntityFactory entityFactory;

			World world("TestWorld");
			Entity* entity = world.CreateSector("TestSector")->CreateEntity("Entity", "TestEntity");
			ActionIncrement* actionIncrement = new ActionIncrement();
			actionIncrement->SetEntity(entity);
			Assert::AreEqual(entity, actionIncrement->GetEntity()->As<Entity>());
		}

		TEST_METHOD(ParsingFromFileTest)
		{
			GameTime gameTime;
			WorldState worldState;
			worldState.SetGameTime(gameTime);

			EntityFactory entityFactory;
			SectorFactory sectorFactory;
			ActionListFactory actionListFactory;
			ActionIfFactory ActionIfFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionCreateActionFactory actionCreateActionFactory;
			ActionDestroyActionFactory actionDestroyActionFactory;

			const std::string fileName = "Content\\ActionTest.json";

			JsonTableParseHelper tableParseHelper;
			World world;
			JsonTableParseHelper::SharedData tableSharedData(world);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("Content\\ActionTest.json"s, parseMaster.GetFileName());

			Assert::AreEqual("TestWorld"s, world.Name());
			Assert::AreEqual(1_z, world.Sectors().Size());

			Sector* sector = world.Sectors().Get<Scope*>(0)->As<Sector>();
			Assert::AreEqual("TestSector"s, sector->Name());
			Assert::AreEqual(1_z, sector->Entities().Size());

			Entity* entity = sector->Entities().Get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("TestEntity"s, entity->Name());
			Assert::AreEqual(4_z, entity->Actions().Size());

			ActionIncrement* actionIncrement = entity->Actions().Get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("TestActionIncrement"s, actionIncrement->Name());
			Assert::AreEqual("Number"s, (*actionIncrement)["Target"].Get<std::string>());
			Assert::AreEqual(2019, (*actionIncrement)["Number"].Get<std::int32_t>());
			
			ActionCreateAction* actionCreateAction = entity->Actions().Get<Scope*>(1)->As<ActionCreateAction>();
			Assert::AreEqual("TestActionCreateAction"s, actionCreateAction->Name());
			Assert::AreEqual("ActionIncrement"s, (*actionCreateAction)["PrototypeName"].Get<std::string>());
			Assert::AreEqual("TestActionIncrement2"s, (*actionCreateAction)["InstanceName"].Get<std::string>());
			
			ActionDestroyAction* actionDestroyAction = entity->Actions().Get<Scope*>(2)->As<ActionDestroyAction>();
			Assert::AreEqual("TestActionDestroyAction"s, actionDestroyAction->Name());
			Assert::AreEqual("TestActionCreateAction"s, (*actionDestroyAction)["ActionInstanceName"].Get<std::string>());

			ActionIf* actionIf = entity->Actions().Get<Scope*>(3)->As<ActionIf>();
			Assert::AreEqual("TestActionIf"s, actionIf->Name());
			Assert::AreEqual(20, (*actionIf)["Number"].Get<std::int32_t>());
			Assert::AreEqual(0, (*actionIf)["Conditon"].Get<std::int32_t>());

			ActionDestroyAction* ActionIfThen = (*actionIf)["ThenAction"].Get<Scope*>(0)->As<ActionDestroyAction>();
			Assert::AreEqual("TestActionDestroyAction2"s, ActionIfThen->Name());
			Assert::AreEqual("TestActionIf"s, (*ActionIfThen)["ActionInstanceName"].Get<std::string>());
			
			ActionIncrement* ActionIfElse = (*actionIf)["ElseAction"].Get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("TestActionIncrement3"s, ActionIfElse->Name());
			Assert::AreEqual("Number"s, (*ActionIfElse)["Target"].Get<std::string>());

			// first update:
			world.Update(worldState);

			Assert::AreEqual(3_z, entity->Actions().Size());
			Assert::AreEqual(2020, (*actionIncrement)["Number"].Get<std::int32_t>());
			Assert::AreEqual(21, (*actionIf)["Number"].Get<std::int32_t>());

			Assert::IsTrue(entity->Actions().Get<Scope*>(0)->Is(ActionIncrement::TypeIdClass()));
			// actionCreateAction got removed
			Assert::IsTrue(entity->Actions().Get<Scope*>(1)->Is(ActionIf::TypeIdClass()));
			// a new ActionIncrement got added
			Assert::IsTrue(entity->Actions().Get<Scope*>(2)->Is(ActionIncrement::TypeIdClass()));


			// second update:
			actionIf->SetCondition(1);
			world.Update(worldState);

			Assert::AreEqual(2_z, entity->Actions().Size());
			Assert::AreEqual(2021, (*actionIncrement)["Number"].Get<std::int32_t>());

			Assert::IsTrue(entity->Actions().Get<Scope*>(0)->Is(ActionIncrement::TypeIdClass()));
			// ActionIf got removed
			Assert::IsTrue(entity->Actions().Get<Scope*>(1)->Is(ActionIncrement::TypeIdClass()));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionTests::sStartMemState;
}
