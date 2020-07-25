#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "Foo.h"
#include <gsl/gsl>
#include <glm/glm.hpp>

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
	class Bar : public Scope
	{
		RTTI_DECLARATIONS(Bar, Scope)

	public:

		gsl::owner<Scope*> Clone() const override
		{
			return new Bar(*this);
		}
	};

	RTTI_DEFINITIONS(Bar)

	TEST_CLASS(ScopeTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
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

#pragma region Scope Tests
		TEST_METHOD(ScopeConstructor)
		{
			Scope s;
			Bar* child = new Bar;
			s.Adopt(*child, "Children");

			Scope copy(s);
			Datum& copyOfChildren = copy["Children"];
			Assert::IsTrue(Datum::DatumTypes::Table == copyOfChildren.Type());
			Assert::AreEqual(size_t(1), copyOfChildren.Size());
			Scope* copyOfChild = copyOfChildren.Get<Scope*>();
			Assert::IsNotNull(copyOfChild);
			Assert::IsTrue(copyOfChild->Is(Bar::TypeIdClass()));
		}

		TEST_METHOD(DeepCopy)
		{
			Scope s;
			Assert::IsTrue(s.IsEmpty());
			s.Append("name") = "hero1";
			s.Append("hp") = 20;

			Scope items;
			items.Append("name") = "knife";
			items.Append("dps") = 3.4f;

			//s.AppendScope("items") = items;

			{
				Scope s2(s);
				Assert::AreEqual(s.Size(), s2.Size());
				Assert::AreEqual(s.Capacity(), s2.Capacity());
			}

			{
				Scope s2 = s;
				Assert::AreEqual(s.Size(), s2.Size());
				Assert::AreEqual(s.Capacity(), s2.Capacity());
			}
		}

		TEST_METHOD(SimpleTest)
		{
			Scope s;
			s.Append("hp") = 20;
		}
		TEST_METHOD(ShallowCopy)
		{
			Scope s;
			s.Append("name") = "hero1";
			s.Append("hp") = 20;

			Scope items;
			items.Append("name") = "knife";
			items.Append("dps") = 3.4f;

			//s.AppendScope("items") = items;

			{
				size_t size = s.Size();
				size_t capacity = s.Capacity();

				Scope s2(std::move(s));
				Assert::AreEqual(size, s2.Size());
				Assert::AreEqual(capacity, s2.Capacity());
				Assert::AreEqual(0_z, s.Size());
				Assert::AreEqual(0_z, s.Capacity());
			}

			{
				size_t size = s.Size();
				size_t capacity = s.Capacity();

				Scope s2 = std::move(s);
				Assert::AreEqual(size, s2.Size());
				Assert::AreEqual(capacity, s2.Capacity());
				Assert::AreEqual(0_z, s.Size());
				Assert::AreEqual(0_z, s.Capacity());
			}
		}

		TEST_METHOD(BracketOperator)
		{
			Scope s;
			s.Append("name") = "hero1";
			s.Append("hp") = 20;

			Scope items;
			items.Append("name") = "knife";
			items.Append("dps") = 3.4f;

			s.AppendScope("items") = items;

			Assert::AreEqual(0, s[0].Get<std::string>().compare("hero1"));
			Assert::AreEqual(20, s[1].Get<std::int32_t>());
			Assert::AreEqual(20, s["hp"].Get<std::int32_t>());

			const Scope s2(s);
			Assert::AreEqual(0, s2[0].Get<std::string>().compare("hero1"));
			Assert::AreEqual(20, s2[1].Get<std::int32_t>());
		}

		TEST_METHOD(Find)
		{
			Scope s;
			s.Append("name") = "hero1";
			s.Append("hp") = 20;

			Scope items;
			items.Append("name") = "knife";
			items.Append("dps") = 3.4f;

			s.AppendScope("items") = items;
			Assert::AreEqual(20, s.Find("hp")->Get<std::int32_t>());

			const Scope s2(s);
			Assert::AreEqual(20, s2.Find("hp")->Get<std::int32_t>());
		}

		TEST_METHOD(Search)
		{
			Scope s;
			s.Append("name") = "hero1";
			s.Append("hp") = 20;

			Scope items;
			items.Append("name") = "knife";
			items.Append("dps") = 3.4f;

			s.Adopt(items, "items");	
			Assert::AreEqual(20, items.Search("hp")->Get<std::int32_t>());

			{
				Scope* scoop;
				Assert::AreEqual(20, items.Search("hp", scoop)->Get<std::int32_t>());
			}
		}


#pragma endregion Scope Tests


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTests::sStartMemState;
}