#include "pch.h"
#include "CppUnitTest.h"
#include "Sector.h"
#include "World.h"
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
	std::wstring ToString<Sector>(const Sector& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SectorTests)
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
			string name = "TestSector";
			Sector sector(name);
			Assert::IsTrue(sector.IsPrescribedAttribute("Name"s));
			Assert::AreEqual(name, sector.Name());
		}

		TEST_METHOD(CopyTest)
		{
			EntityFactory entityfactory;

			Sector sector1("TestSector");
			sector1.CreateEntity("Entity", "TestEntity");
			Assert::AreEqual(1_z, sector1.Entities().Size());
			{
				Sector sector2("TestSectorOther");
				Assert::AreNotEqual(sector2.Name(), sector1.Name());
				Assert::AreEqual(0_z, sector2.Entities().Size());

				sector2 = sector1;
				Assert::AreEqual(sector2.Name(), sector1.Name());
				Assert::AreEqual(1_z, sector2.Entities().Size());
			}
			{
				Sector sector2(sector1);

				Assert::AreEqual(sector2.Name(), sector1.Name());
				Assert::AreEqual(1_z, sector2.Entities().Size());
			}
		}

		TEST_METHOD(MoveTest)
		{
			EntityFactory entityfactory;

			Sector sector1("TestSector");
			sector1.CreateEntity("Entity", "TestEntity");
			Assert::AreEqual(1_z, sector1.Entities().Size());

			Sector sector2(std::move(sector1));

			Assert::ExpectException<std::runtime_error>([&sector1] { sector1.Entities(); });
			Assert::AreEqual("TestSector"s, sector2.Name());
			Assert::AreEqual(1_z, sector2.Entities().Size());

			Sector sector3;
			sector3 = std::move(sector2);

			Assert::ExpectException<std::runtime_error>([&sector2] { sector2.Entities(); });
			Assert::AreEqual("TestSector"s, sector3.Name());
			Assert::AreEqual(1_z, sector3.Entities().Size());
		}

		TEST_METHOD(Name)
		{
			Sector sector("");
			string name = "TestSector";
			Assert::IsTrue(sector.Name().empty());
			sector.SetName(name);
			Assert::AreEqual(name, sector.Name());
		}
		
		TEST_METHOD(WorldGetSetTest)
		{
			World world("TestWorld1");
			World world2("TestWorld2");

			Sector* sector = world.CreateSector("TestSector1Then2");

			Assert::IsTrue(&world == sector->GetWorld());

			sector->SetWorld(&world2);
			Assert::IsTrue(&world2 == sector->GetWorld());
		}

		TEST_METHOD(CloneTest)
		{
			Sector sector("TestSector");
			auto clone = sector.Clone();
			Sector* sectorClone = clone->As<Sector>();

			Assert::IsNotNull(sectorClone);
			Assert::AreNotSame(sector, *sectorClone);

			delete clone;
		}

		TEST_METHOD(UpdateTest)
		{
			Sector sector("TestSector");
			GameTime gameTime;
			WorldState worldState;

			worldState.SetGameTime(gameTime);

			sector.Update(worldState);
		}

		TEST_METHOD(ParsingFromFileTest)
		{
			SectorFactory sectorFactory;
			EntityFactory entityfactory;

			const std::string fileName = "Content\\SectorTest.json";

			JsonTableParseHelper tableParseHelper;
			Sector sector;
			JsonTableParseHelper::SharedData tableSharedData(sector);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("Content\\SectorTest.json"s, parseMaster.GetFileName());

			Assert::AreEqual(123, sector["A"].Get<std::int32_t>());
			Assert::AreEqual("Sector1"s, sector.Name());
			Assert::AreEqual("Sector1"s, sector["Name"].Get<std::string>());

			Entity* entity1 = (sector)["Entities"].Get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("Entity1-1"s, entity1->Name());
			Assert::AreEqual("Entity1-1"s, (*entity1)["Name"].Get<std::string>());

			Entity* entity2 = (sector)["Entities"].Get<Scope*>(1)->As<Entity>();
			Assert::AreEqual("Entity1-2"s, entity2->Name());
			Assert::AreEqual("Entity1-2"s, (*entity2)["Name"].Get<std::string>());
			Assert::AreEqual(123, (*entity2)["A"].Get<std::int32_t>());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SectorTests::sStartMemState;
}
