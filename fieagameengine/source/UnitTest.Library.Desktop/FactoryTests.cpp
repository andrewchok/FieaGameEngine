#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
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
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTests)
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

		TEST_METHOD(ConstructorDestructor)
		{
			Assert::IsNull(Factory<Foo>::Find("DerivedFoo"s));
			{
				DerivedFooFactory derivedFooFactory;
				Assert::IsNotNull(&derivedFooFactory);
				Assert::AreEqual("DerivedFoo"s, derivedFooFactory.ClassName());
				Assert::IsNotNull(Factory<Foo>::Find("DerivedFoo"s));

				Foo* foo = Factory<Foo>::Create("DerivedFoo"s);
				delete foo;
			}
			Assert::IsNull(Factory<Foo>::Find("DerivedFoo"s));
		}

		TEST_METHOD(AddRemove)
		{
			{
				DerivedFooFactory derivedFooFactory;
				Foo* foo = derivedFooFactory.Create();
				Assert::IsTrue(Factory<Foo>::Find("DerivedFoo"s) == &derivedFooFactory);

				delete foo;
			}
				Assert::IsTrue(Factory<Foo>::Find("DerivedFoo"s) == nullptr);
		}

		TEST_METHOD(Find)
		{
			DerivedFooFactory derivedFooFactory;
			Assert::IsTrue(Factory<Foo>::Find("DerivedFoo"s) == &derivedFooFactory);
			Assert::IsTrue(Factory<Foo>::Find("DerivedBar"s) == nullptr);
		}

		TEST_METHOD(ClassName)
		{
			DerivedFooFactory derivedFooFactory;
			Assert::AreEqual("DerivedFoo"s, derivedFooFactory.ClassName());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FactoryTests::sStartMemState;
}
