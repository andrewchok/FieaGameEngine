#include "pch.h"
#include "CppUnitTest.h"
#include "DefaultHash.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;

namespace Library
{
	template<>
	struct DefaultHash<Foo>
	{
		inline std::size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTests)
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

		TEST_METHOD(CharStar)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<char*> hashFunc;

			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
		TEST_METHOD(ConstCharStar)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<const char*> hashFunc;

			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
		TEST_METHOD(CharStarConst)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<char* const> hashFunc;

			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
		TEST_METHOD(ConstCharStarConst)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<const char* const> hashFunc;

			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(String)
		{
			std::string a = "Hello";
			std::string b = "Goodbye";
			std::string c(a);
			DefaultHash<std::string> hashFunc;

			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(StringConst)
		{
			std::string a = "Hello";
			std::string b = "Goodbye";
			std::string c(a);
			DefaultHash<const std::string> hashFunc;

			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DefaultHashTests::sStartMemState;
}