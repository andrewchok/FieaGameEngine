#include "pch.h"
#include "CppUnitTest.h"
#include "Entity.h"
#include "Sector.h"
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
	std::wstring ToString<Entity>(const Entity& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntityTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			
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
			std::string name = "TestEntity";
			Entity entity(name);
			Assert::IsTrue(entity.IsPrescribedAttribute("Name"s));
			Assert::AreEqual(name, entity.Name());			
		}

		TEST_METHOD(CopyTest)
		{
			Entity entity1("TestEntity");
			{
				Entity entity2("TestEntityOther");

				Assert::AreNotEqual(entity2.Name(), entity1.Name());

				entity2 = entity1;
				Assert::AreEqual(entity2.Name(), entity1.Name());
			}
			{
				Entity entity2(entity1);

				Assert::AreEqual(entity2.Name(), entity1.Name());
			}
		}

		TEST_METHOD(MoveTest)
		{
			Entity entity1("TestEntity");
			Entity entity2(std::move(entity1));

			Assert::AreEqual("TestEntity"s, entity2.Name());

			Entity entity3;
			entity3 = std::move(entity2);
			Assert::AreEqual("TestEntity"s, entity3.Name());
		}

		TEST_METHOD(Name)
		{
			Entity entity("");
			string name = "TestEntity";
			Assert::IsTrue(entity.Name().empty());
			entity.SetName(name);
			Assert::AreEqual(name, entity.Name());
		}

		TEST_METHOD(SectorGetSetTest)
		{
			EntityFactory factory;
			Sector sector("TestSector1");
			Sector sector2("TestSector2");

			Entity* entity = sector.CreateEntity("Entity", "TestEntity1Then2");

			Assert::IsTrue(&sector == entity->GetSector());

			entity->SetSector(&sector2);
			Assert::IsTrue(&sector2 == entity->GetSector());
		}

		TEST_METHOD(CloneTest)		
		{
			Entity entity("TestEntity");
			auto clone = entity.Clone();
			Entity* entityClone = clone->As<Entity>();

			Assert::IsNotNull(entityClone);
			Assert::AreNotSame(entity, *entityClone);

			delete clone;
		}

		TEST_METHOD(UpdateTest)
		{
			Entity entity("TestEntity");
			GameTime gameTime;
			WorldState worldState;

			worldState.SetGameTime(gameTime);

			entity.Update(worldState);
		}

		TEST_METHOD(ParsingFromFileTest)
		{
			EntityFactory entityfactory;

			const std::string fileName = "Content\\EntityTest.json";

			JsonTableParseHelper tableParseHelper;
			Entity entity;
			JsonTableParseHelper::SharedData tableSharedData(entity);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("Content\\EntityTest.json"s, parseMaster.GetFileName());
			
			Assert::AreEqual("Entity1"s, entity.Name());
			Assert::AreEqual("Entity1"s, entity["Name"].Get<std::string>());
			Assert::AreEqual(123, entity["A"].Get<std::int32_t>());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EntityTests::sStartMemState;
}
