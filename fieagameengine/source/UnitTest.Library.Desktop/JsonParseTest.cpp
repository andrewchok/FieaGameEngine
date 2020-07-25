#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTestParseHelper.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::JsonTestParseHelper::SharedData>(Library::JsonTestParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::JsonTestParseHelper::SharedData>(const Library::JsonTestParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}
	
	template<>
	inline std::wstring ToString<Library::JsonTestParseHelper>(Library::JsonTestParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::IJsonParseHelper>(Library::IJsonParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::JsonParseMaster::SharedData>(Library::JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::JsonParseMaster>(Library::JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseTests)
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

		TEST_METHOD(Constructor)
		{
			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			JsonParseMaster parseMaster(testSharedData);

			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(&testSharedData, parseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());

			Assert::ExpectException<std::runtime_error>([&parseMaster, &testParseHelper] 
			{
				parseMaster.AddHelper(testParseHelper);
			});

			JsonTestParseHelper::SharedData testSharedData2;
			parseMaster.Initialize();

			parseMaster.SetSharedData(testSharedData2);
			Assert::AreEqual(&testSharedData2, parseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());

			auto clone = parseMaster.Clone();
			Assert::ExpectException<std::runtime_error>([&clone, &testSharedData]
			{
				clone->SetSharedData(testSharedData);
			});

			delete clone;
		}

		TEST_METHOD(Move)
		{
			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			
			{
				JsonParseMaster parseMaster(testSharedData);
				parseMaster.AddHelper(testParseHelper);
				
				JsonParseMaster parseMasterMove(std::move(parseMaster));
				Assert::IsFalse(parseMasterMove.IsClone());
				Assert::AreEqual(1_z, parseMasterMove.GetListOfHelpers().Size());
				Assert::AreEqual(&parseMasterMove, testSharedData.GetJsonParseMaster());
			}
			
			{
				JsonParseMaster parseMaster(testSharedData);
				parseMaster.AddHelper(testParseHelper);

				JsonTestParseHelper::SharedData testSharedData2;
				JsonParseMaster parseMasterMove(testSharedData2);

				parseMasterMove = std::move(parseMaster);
				Assert::IsFalse(parseMasterMove.IsClone());
				Assert::AreEqual(1_z, parseMasterMove.GetListOfHelpers().Size());
				Assert::AreEqual(&parseMasterMove, testSharedData.GetJsonParseMaster());
			}
		}

		TEST_METHOD(Clone)
		{
			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			JsonParseMaster parseMaster(testSharedData);
			parseMaster.AddHelper(testParseHelper);
			Assert::IsFalse(parseMaster.IsClone());

			auto clone = parseMaster.Clone();
			Assert::IsTrue(clone->IsClone());
			Assert::AreNotEqual(&parseMaster, clone);

			Assert::ExpectException<std::runtime_error>([&clone]
			{
				JsonTestParseHelper testParseHelper2;
				clone->AddHelper(testParseHelper2);
			});		
			Assert::ExpectException<std::runtime_error>([&clone, &testParseHelper]
			{
				clone->RemoveHelper(testParseHelper);
			});
			Assert::ExpectException<std::runtime_error>([&clone]
			{
				JsonTestParseHelper::SharedData testSharedData2;
				clone->SetSharedData(testSharedData2);
			});

			auto cloneListOfHelpers = clone->GetListOfHelpers();

			Assert::AreNotEqual(cloneListOfHelpers[0], parseMaster.GetListOfHelpers()[0]);
			Assert::AreNotSame(clone->GetSharedData(), parseMaster.GetSharedData());
						
			delete clone;			
		}

		TEST_METHOD(ParseString)
		{
			const std::string input = R"({
				"array": [
					1,
					2,
					3
				],
				"boolean": true,
				"color": "gold",
				"null": null,
				"number": 123,
				"object": {
				"a": "b",
				"c": "d"
				},
				"string": "Hello World"	
			})"s;

			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			JsonParseMaster parseMaster(testSharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.mIsInitialized);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.mIsInitialized);

			parseMaster.Parse(input);
			Assert::AreEqual(11_z, testParseHelper.mStartHandlerCount);
			Assert::AreEqual(11_z, testParseHelper.mEndHandlerCount);
			Assert::AreEqual(0_z, testSharedData.Depth());
			Assert::AreEqual(2_z, testSharedData.mMaxDepth);
		}

		TEST_METHOD(ParseStringStream)
		{
			std::stringstream input;
			input << R"({
				"array": [
					1,
					2,
					3
				],
				"boolean": true,
				"color": "gold",
				"null": null,
				"number": 123,
				"object": {
				"a": {
					"aa" : "bb"
					},
				"b": {
					"cc" : "dd"
					}
				},
				"string": "Hello World"			
			})"s;

			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			JsonParseMaster parseMaster(testSharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.mIsInitialized);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.mIsInitialized);

			parseMaster.Parse(input);
			Assert::AreEqual(13_z, testParseHelper.mStartHandlerCount);
			Assert::AreEqual(13_z, testParseHelper.mEndHandlerCount);
			Assert::AreEqual(0_z, testSharedData.Depth());
			Assert::AreEqual(3_z, testSharedData.mMaxDepth);
		}

		TEST_METHOD(ParseFile)
		{
			const std::string fileName = "test.json";
			const std::string input = R"({
				"array": [
					1,
					2,
					3
				],
				"boolean": true,
				"color": "gold",
				"null": null,
				"number": 123,
				"object": {
				"a": "b",
				"c": "d"
				},
				"string": "Hello World",
				"string2": "Goodbye World"		
			})"s;
			std::ofstream jsonFile;
			jsonFile.open(fileName);
			jsonFile << input;
			jsonFile.close();

			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			JsonParseMaster parseMaster(testSharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.mIsInitialized);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.mIsInitialized);

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("test.json"s, parseMaster.GetFileName());
			Assert::AreEqual(12_z, testParseHelper.mStartHandlerCount);
			Assert::AreEqual(12_z, testParseHelper.mEndHandlerCount);
			Assert::AreEqual(0_z, testSharedData.Depth());
			Assert::AreEqual(2_z, testSharedData.mMaxDepth);

			const std::string file404 = "404.json";
			parseMaster.Initialize();

			Assert::ExpectException<std::runtime_error>([&parseMaster, &file404]
			{
				parseMaster.ParseFromFile(file404);
			});
		}

		TEST_METHOD(AddAndRemoveHelper)
		{
			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData testSharedData;
			JsonParseMaster parseMaster(testSharedData);

			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(1_z, parseMaster.GetListOfHelpers().Size());
			Assert::AreEqual(&testSharedData, parseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());

			Assert::ExpectException<std::runtime_error>([&parseMaster, &testParseHelper]
			{
				parseMaster.AddHelper(testParseHelper);
			});

			parseMaster.RemoveHelper(testParseHelper);
			Assert::AreEqual(0_z, parseMaster.GetListOfHelpers().Size());

			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(1_z, parseMaster.GetListOfHelpers().Size());
		}

		TEST_METHOD(NoHandler)
		{
			const std::string input = R"({
				"array": [
					1,
					2,
					3
				],
				"boolean": true,
				"color": "gold",
				"null": null,
				"number": 123,
				"object": {
				"a": {
					"aa" : "bb"
					},
				"b": {
					"cc" : "dd"
					}
				},
				"string": "Hello World"			
			})"s;

			{
				JsonTestParseHelper testParseHelper;
				JsonTestParseHelper::SharedData testSharedData;
				JsonParseMaster parseMaster(testSharedData);

				parseMaster.Initialize();
				Assert::AreEqual(0_z, testSharedData.mMaxDepth);

				parseMaster.Parse(input);
				// no JsonTestParseHelper to handle so no depth explored
				Assert::AreEqual(0_z, testSharedData.mMaxDepth);
			}
		}

		TEST_METHOD(JsonParseTestHelperRTTI)
		{
		}

		TEST_METHOD(JsonParseTestHelperSharedDataRTTI)
		{
			JsonTestParseHelper::SharedData sharedData;


		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParseTests::sStartMemState;
}
