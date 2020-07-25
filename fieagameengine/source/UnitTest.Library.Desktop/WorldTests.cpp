#include "pch.h"
#include "CppUnitTest.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "Factory.h"
#include "JsonTableParseHelper.h"
#include <fstream>
#include "Foo.h"
#include "DerivedFoo.h"

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
	std::wstring ToString<World>(const World& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(WorldTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeManager::RegisterType(World::TypeIdClass(), World::Signatures());

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

		TEST_METHOD(ConstructorDestructor)
		{
			string name = "TestWorld";
			World world(name);
			Assert::IsTrue(world.IsPrescribedAttribute("Name"s));
			Assert::AreEqual(name, world.Name());
		}

		TEST_METHOD(CopyTest)
		{
			SectorFactory sectorFactory;
			EntityFactory entityfactory;

			World world1("TestWorld");
			Sector* sector1 = world1.CreateSector("TestSector");

			sector1->CreateEntity("Entity", "TestEntity");
			Assert::AreEqual(1_z, world1.Sectors().Size());
			Assert::AreEqual(1_z, sector1->Entities().Size());
			{
				World world2("TestWorldOther");
				Assert::AreNotEqual(world2.Name(), world1.Name());
				Assert::AreEqual(0_z, world2.Sectors().Size());

				world2 = world1;
				Assert::AreEqual(world2.Name(), world1.Name());
				Assert::AreEqual(1_z, world2.Sectors().Size());
			}
			{
				World world2(world1);

				Assert::AreEqual(world2.Name(), world1.Name());
				Assert::AreEqual(1_z, world2.Sectors().Size());
			}
		}
		
		TEST_METHOD(Name)
		{
			World world("");
			string name = "TestWorld";
			Assert::IsTrue(world.Name().empty());
			world.SetName(name);
			Assert::AreEqual(name, world.Name());
		}

		TEST_METHOD(SectorEntityTest)
		{
			EntityFactory factory;

			Assert::IsTrue(factory.Find("Entity") == &factory);

			World world("TestWorld");
			Sector* sector = world.CreateSector("TestSector");
			world.CreateSector("TestSector2");

			Entity* entity = sector->CreateEntity("Entity"s, "TestEntity1"s);
			sector->CreateEntity("Entity"s, "TestEntity2"s);

			Datum& sectors = world.Sectors();
			Assert::AreEqual(2_z, sectors.Size());
			Assert::IsTrue(sector == sectors.Get<Scope*>(0));

			Datum& entities = sector->Entities();
			Assert::AreEqual(2_z, entities.Size());
			Assert::IsTrue(entity == entities.Get<Scope*>(0));
		}

		TEST_METHOD(CloneTest)
		{
			World world("TestWorld");
			auto clone = world.Clone();
			World* worldClone = clone->As<World>();

			Assert::IsNotNull(worldClone);
			Assert::AreNotSame(world, *worldClone);

			delete clone;
		}

		TEST_METHOD(UpdateTest)
		{
			World world("TestWorld");
			GameTime gameTime;
			WorldState worldState;

			worldState.SetGameTime(gameTime);
			world.CreateSector("TestSector");

			world.Update(worldState);
		}

		TEST_METHOD(ParsingFromFileTest)
		{
			SectorFactory sectorFactory;
			EntityFactory entityfactory;

			const std::string fileName = "Content\\WorldTest.json";

			JsonTableParseHelper tableParseHelper;
			World world;
			JsonTableParseHelper::SharedData tableSharedData(world);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("Content\\WorldTest.json"s, parseMaster.GetFileName());

			Assert::AreEqual(123, world["A"].Get<std::int32_t>());
			Assert::AreEqual("World"s, world.Name());
			Assert::AreEqual("World"s, world["Name"].Get<std::string>());

			Sector* sector1 = (world)["Sectors"].Get<Scope*>(0)->As<Sector>();
			Assert::AreEqual("Sector1"s, sector1->Name());
			Assert::AreEqual("Sector1"s, (*sector1)["Name"].Get<std::string>());
			
			Entity* entity1_1 = (*sector1)["Entities"].Get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("Entity1-1"s, entity1_1->Name());
			Assert::AreEqual("Entity1-1"s, (*entity1_1)["Name"].Get<std::string>());

			Entity* entity1_2 = (*sector1)["Entities"].Get<Scope*>(1)->As<Entity>();
			Assert::AreEqual("Entity1-2"s, entity1_2->Name());
			Assert::AreEqual("Entity1-2"s, (*entity1_2)["Name"].Get<std::string>());
			Assert::AreEqual(123, (*entity1_2)["A"].Get<std::int32_t>());

			Sector* sector2 = (world)["Sectors"].Get<Scope*>(1)->As<Sector>();
			Assert::AreEqual(123, (*sector2)["A"].Get<std::int32_t>());
			Assert::AreEqual("Sector2"s, sector2->Name());
			Assert::AreEqual("Sector2"s, (*sector2)["Name"].Get<std::string>());

			Entity* entity2_1 = (*sector2)["Entities"].Get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("Entity2-1"s, entity2_1->Name());
			Assert::AreEqual("Entity2-1"s, (*entity2_1)["Name"].Get<std::string>());
		}
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState WorldTests::sStartMemState;
}
