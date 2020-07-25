#include "pch.h"
#include "CppUnitTest.h"
#include "JsonTableParseHelper.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::Scope>(Library::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::JsonParseMaster>(Library::JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::JsonTableParseHelper::SharedData>(Library::JsonTableParseHelper::SharedData* t)
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
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonTableParseHelperTests)
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
			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);

			parseMaster.AddHelper(tableParseHelper);
			Assert::AreEqual(&tableSharedData, parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());

			Assert::ExpectException<std::runtime_error>([&parseMaster, &tableParseHelper]
			{
				parseMaster.AddHelper(tableParseHelper);
			});

			Scope scope2;
			JsonTableParseHelper::SharedData tableSharedData2(scope2);
			parseMaster.Initialize();

			parseMaster.SetSharedData(tableSharedData2);
			Assert::AreEqual(&tableSharedData2, parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());

			auto clone = parseMaster.Clone();
			Assert::ExpectException<std::runtime_error>([&clone, &tableSharedData]
			{
				clone->SetSharedData(tableSharedData);
			});

			delete clone->GetSharedData()->As<JsonTableParseHelper::SharedData>()->GetSharedData();
			delete clone;
		}

		TEST_METHOD(Move)
		{
			Scope scope;
			Scope scope2;
			JsonTableParseHelper tableParseHelper;
			JsonTableParseHelper::SharedData tableSharedData(scope);

			{
				JsonParseMaster parseMaster(tableSharedData);
				parseMaster.AddHelper(tableParseHelper);

				JsonParseMaster parseMasterMove(std::move(parseMaster));
				Assert::IsFalse(parseMasterMove.IsClone());
				Assert::AreEqual(1_z, parseMasterMove.GetListOfHelpers().Size());
				Assert::AreEqual(&parseMasterMove, tableSharedData.GetJsonParseMaster());
			}

			{
				JsonParseMaster parseMaster(tableSharedData);
				parseMaster.AddHelper(tableParseHelper);

				JsonTableParseHelper::SharedData tableSharedData2(scope2);
				JsonParseMaster parseMasterMove(tableSharedData2);

				parseMasterMove = std::move(parseMaster);
				Assert::IsFalse(parseMasterMove.IsClone());
				Assert::AreEqual(1_z, parseMasterMove.GetListOfHelpers().Size());
				Assert::AreEqual(&parseMasterMove, tableSharedData.GetJsonParseMaster());
			}
		}

		TEST_METHOD(Clone)
		{
			Scope scope;
			Scope scope2;
			JsonTableParseHelper testParseHelper;
			JsonTableParseHelper::SharedData testSharedData(scope);
			JsonParseMaster parseMaster(testSharedData);
			parseMaster.AddHelper(testParseHelper);
			Assert::IsFalse(parseMaster.IsClone());

			auto clone = parseMaster.Clone();
			Assert::IsTrue(clone->IsClone());
			Assert::AreNotEqual(&parseMaster, clone);

			Assert::ExpectException<std::runtime_error>([&clone]
			{
				JsonTableParseHelper testParseHelper2;
				clone->AddHelper(testParseHelper2);
			});
			Assert::ExpectException<std::runtime_error>([&clone, &testParseHelper]
			{
				clone->RemoveHelper(testParseHelper);
			});
			Assert::ExpectException<std::runtime_error>([&clone, &scope2]
			{
				JsonTableParseHelper::SharedData testSharedData2(scope2);
				clone->SetSharedData(testSharedData2);
			});

			auto cloneListOfHelpers = clone->GetListOfHelpers();

			Assert::AreNotEqual(cloneListOfHelpers[0], parseMaster.GetListOfHelpers()[0]);
			Assert::AreNotSame(clone->GetSharedData(), parseMaster.GetSharedData());

			delete clone->GetSharedData()->As<JsonTableParseHelper::SharedData>()->GetSharedData();
			delete clone;
		}

		TEST_METHOD(ParseStringPrimitives)
		{
			const std::string input = R"(
			{
			  "name": {
				"type": "string",
				"value": "John Doe"
			  },
			  "health": {
				"type": "integer",
				"value": 100
			  },
			  "dps": {
				"type": "float",
				"value": 49.9
			  }
			}
			)"s;

			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Initialize();
			parseMaster.Parse(input);

			Assert::AreEqual("John Doe"s, scope["name"].Get<std::string>());
			Assert::AreEqual(100, scope["health"].Get<std::int32_t>());
			Assert::AreEqual(49.9f, scope["dps"].Get<std::float_t>());
		}

		TEST_METHOD(ParseStringIntegerArray)
		{
			const std::string input = R"(
			{
			  "Prime Number": {
				"type": "integer",
				"value": [
				  {
					"type": "integer",
					"value": 3
				  },
				  {
					"type": "integer",
					"value": 5
				  },
				  {
					"type": "integer",
					"value": 7
				  },
				  {
					"type": "integer",
					"value": 11
				  }
				]
			  }
			}
			)"s;

			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Initialize();
			parseMaster.Parse(input);

			Assert::AreEqual(3, scope["Prime Number"].Get<std::int32_t>(0));
			Assert::AreEqual(5, scope["Prime Number"].Get<std::int32_t>(1));
			Assert::AreEqual(7, scope["Prime Number"].Get<std::int32_t>(2));
			Assert::AreEqual(11, scope["Prime Number"].Get<std::int32_t>(3));
		}

		TEST_METHOD(ParseStringTableArray)
		{
#pragma region jsondata


			const std::string input = R"(
			{
			  "NestedScopeArray": {
				"type": "table",
				"value": [
				  {
					"type": "table",
					"value": {
					  "Name": {
						"type": "string",
						"value": "Laser Vision"
					  },
					  "Dps": {
						"type": "float",
						"value": 1.5
					  }
					}
				  },
				  {
					"type": "table",
					"value": {
					  "Name": {
						"type": "string",
						"value": "X-Ray Vision"
					  },
					  "Dps": {
						"type": "float",
						"value": 0
					  }
					}
				  },
				  {
					"type": "table",
					"value": {
					  "Name": {
						"type": "string",
						"value": "Heat Ray"
					  },
					  "Dps": {
						"type": "float",
						"value": 4.9
					  }
					}
				  }
				]
			  }
			}
			)"s;

#pragma endregion
			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Initialize();
			parseMaster.Parse(input);

			Datum& datumScope = scope["NestedScopeArray"];
			Scope* NestedScope0 = scope["NestedScopeArray"].Get<Scope*>(0);
			Scope* NestedScope1 = scope["NestedScopeArray"].Get<Scope*>(1);
			Scope* NestedScope2 = scope["NestedScopeArray"].Get<Scope*>(2);
			Assert::AreEqual(3_z, datumScope.Size());
			Assert::AreEqual("Laser Vision"s, (*NestedScope0)["Name"].Get<std::string>());
			Assert::AreEqual(1.5f, (*NestedScope0)["Dps"].Get<std::float_t>());
			Assert::AreEqual("X-Ray Vision"s, (*NestedScope1)["Name"].Get<std::string>());
			Assert::AreEqual(0.0f, (*NestedScope1)["Dps"].Get<std::float_t>());
			Assert::AreEqual("Heat Ray"s, (*NestedScope2)["Name"].Get<std::string>());
			Assert::AreEqual(4.9f, (*NestedScope2)["Dps"].Get<std::float_t>());
		}

		TEST_METHOD(ParseStringStreamNestedScopes)
		{
			std::stringstream input;
			input << R"(
			{
			  "Weapon": {
				"type": "table",
				"value": {
				  "Pistol": {
					"type": "table",
					"value": {
					  "Bullets": {
						"type": "integer",
						"value": 12
					  }
					}
				  }
				}
			  }
			}
			)"s;

			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Initialize();
			parseMaster.Parse(input);

			Scope* weaponScope = scope["Weapon"].Get<Scope*>();
			Scope* pistolScope = (*weaponScope)["Pistol"].Get<Scope*>();
			Assert::AreEqual(12, (*pistolScope)["Bullets"].Get<std::int32_t>());
		}

		TEST_METHOD(ParseFilePrimitives)
		{
			const std::string fileName = "tableTest.json";
			const std::string input = R"(
			{
			  "name": {
				"type": "string",
				"value": "John Doe"
			  },
			  "health": {
				"type": "integer",
				"value": 100
			  },
			  "dps": {
				"type": "float",
				"value": 49.9
			  }
			}
			)"s;
			std::ofstream jsonFile;
			jsonFile.open(fileName);
			jsonFile << input;
			jsonFile.close();

			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("tableTest.json"s, parseMaster.GetFileName());

			Assert::AreEqual("John Doe"s, scope["name"].Get<std::string>());
			Assert::AreEqual(100, scope["health"].Get<std::int32_t>());
			Assert::AreEqual(49.9f, scope["dps"].Get<std::float_t>());
			
			const std::string file404 = "404.json";
			parseMaster.Initialize();

			Assert::ExpectException<std::runtime_error>([&parseMaster, &file404]
			{
				parseMaster.ParseFromFile(file404);
			});
		}

		TEST_METHOD(ParseFileAllPrimitives)
		{
			const std::string fileName = "Content\\JsonParseTableTest.json";
			
			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);
			parseMaster.AddHelper(tableParseHelper);
			parseMaster.Initialize();

			parseMaster.ParseFromFile(fileName);
			Assert::AreEqual("Content\\JsonParseTableTest.json"s, parseMaster.GetFileName());

			Assert::AreEqual("Hello"s, scope["StringTest"].Get<std::string>());
			Assert::AreEqual(123, scope["IntegerTest"].Get<std::int32_t>());
			Assert::AreEqual(1.23f, scope["FloatTest"].Get<std::float_t>());
			Assert::AreEqual(glm::to_string(glm::vec4(1, 2, 3, 4)), glm::to_string(scope["VectorTest"].Get<glm::vec4>()));
			Assert::AreEqual(glm::to_string(glm::mat4(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4)), glm::to_string(scope["MatrixTest"].Get<glm::mat4>()));
			
			Assert::AreEqual("Hello"s, scope["StringArrayTest"].Get<std::string>(0));
			Assert::AreEqual("Goodbye"s, scope["StringArrayTest"].Get<std::string>(1));
			Assert::AreEqual(123, scope["IntegerArrayTest"].Get<std::int32_t>(0));
			Assert::AreEqual(321, scope["IntegerArrayTest"].Get<std::int32_t>(1));
			Assert::AreEqual(1.23f, scope["FloatArrayTest"].Get<std::float_t>(0));
			Assert::AreEqual(3.21f, scope["FloatArrayTest"].Get<std::float_t>(1));
			Assert::AreEqual(glm::to_string(glm::vec4(1, 2, 3, 4)), glm::to_string(scope["VectorArrayTest"].Get<glm::vec4>(0)));
			Assert::AreEqual(glm::to_string(glm::vec4(2, 2, 3, 4)), glm::to_string(scope["VectorArrayTest"].Get<glm::vec4>(1)));
			Assert::AreEqual(glm::to_string(glm::mat4(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4)), glm::to_string(scope["MatrixArrayTest"].Get<glm::mat4>(0)));
			Assert::AreEqual(glm::to_string(glm::mat4(2, 2, 3, 4, 2, 2, 3, 4, 2, 2, 3, 4, 2, 2, 3, 4)), glm::to_string(scope["MatrixArrayTest"].Get<glm::mat4>(1)));

			const std::string file404 = "404.json";
			parseMaster.Initialize();

			Assert::ExpectException<std::runtime_error>([&parseMaster, &file404]
			{
				parseMaster.ParseFromFile(file404);
			});
		}
		TEST_METHOD(AddAndRemoveHelper)
		{
			JsonTableParseHelper tableParseHelper;
			Scope scope;
			JsonTableParseHelper::SharedData tableSharedData(scope);
			JsonParseMaster parseMaster(tableSharedData);

			parseMaster.AddHelper(tableParseHelper);
			Assert::AreEqual(1_z, parseMaster.GetListOfHelpers().Size());
			Assert::AreEqual(&tableSharedData, parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());

			Assert::ExpectException<std::runtime_error>([&parseMaster, &tableParseHelper]
			{
				parseMaster.AddHelper(tableParseHelper);
			});

			parseMaster.RemoveHelper(tableParseHelper);
			Assert::AreEqual(0_z, parseMaster.GetListOfHelpers().Size());

			parseMaster.AddHelper(tableParseHelper);
			Assert::AreEqual(1_z, parseMaster.GetListOfHelpers().Size());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonTableParseHelperTests::sStartMemState;
}
