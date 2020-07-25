#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
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

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(glm::vec4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(glm::mat4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<> inline
		std::wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		RETURN_WIDE_STRING(static_cast<size_t>(t));
	}
}

template<typename T>
Datum::DatumTypes GetDatumTypeOf()
{
	if constexpr (std::is_same<T, std::int32_t>::value) return Datum::DatumTypes::Integer;
	else if constexpr (std::is_same<T, std::float_t>::value) return Datum::DatumTypes::Float;
	else if constexpr (std::is_same<T, glm::vec4>::value) return Datum::DatumTypes::Vector;
	else if constexpr (std::is_same<T, glm::mat4>::value) return Datum::DatumTypes::Matrix;
	else if constexpr (std::is_same<T, Scope*>::value) return Datum::DatumTypes::Table;
	else if constexpr (std::is_same<T, std::string>::value) return Datum::DatumTypes::String;
	else if constexpr (std::is_same<T, RTTI*>::value) return Datum::DatumTypes::Pointer;
	else throw std::runtime_error("Invalid Data Type");
}

template<typename T>
void CheckStorageMethods(T value)
{
	Datum d;
	Assert::IsTrue(d.IsEmpty());
	Assert::AreEqual(0_z, d.Size());
	Assert::AreEqual(0_z, d.Capacity());

	d.PushBack(value);
	Assert::IsFalse(d.IsEmpty());
	Assert::AreEqual(1_z, d.Size()); 
	Assert::AreEqual(1_z, d.Capacity());
	Assert::AreNotEqual(static_cast<size_t>(Datum::DatumTypes::Unknown), static_cast<size_t>(d.Type()));
	Assert::AreNotEqual(static_cast<size_t>(Datum::DatumTypes::Unknown), static_cast<size_t>(d.Type()));

	d.Reserve(5_z);
	Assert::AreEqual(1_z, d.Size()); 
	Assert::AreEqual(5_z, d.Capacity());

	d.Resize(3);
	Assert::AreEqual(3_z, d.Size()); 
	Assert::AreEqual(3_z, d.Capacity());
}

template<typename T>
void CheckElementAccess(T value)
{
	Datum d;
	d.PushBack(value);
	d.PushBack(value);
	d.PushBack(value);
	const Datum d2 = d;

	Assert::AreEqual(value, d.Get<T>());
	Assert::AreEqual(value, d2.Get<T>());
	Assert::AreEqual(value, d.Front<T>());
	Assert::AreEqual(value, d2.Front<T>());
	Assert::AreEqual(value, d.Back<T>());
	Assert::AreEqual(value, d2.Back<T>());
}

template<typename T>
void CheckModifiers(T value1, T value2)
{
	Datum d;
	d.PushBack(value1);
	d.PushBack(value1);
	d.PushBack(value1);

	Assert::AreEqual(value1, d.Get<T>());
	d.Set(value2);
	Assert::AreEqual(value2, d.Get<T>());

	Datum d2;
	Assert::AreEqual(static_cast<size_t>(Datum::DatumTypes::Unknown), static_cast<size_t>(d2.Type()));
	d2.SetType(Datum::DatumTypes::Float);
	Assert::AreNotEqual(static_cast<size_t>(Datum::DatumTypes::Unknown), static_cast<size_t>(d2.Type()));
	
	Assert::AreEqual(3_z, d.Size());
	d.Clear();
	Assert::AreEqual(0_z, d.Size());
	d.PushBack(value1);
	Assert::AreEqual(1_z, d.Size());
	d.RemoveAt<T>(0);
	Assert::AreEqual(0_z, d.Size());

}

template<typename T>
void CheckConstructor(T value)
{
	{
		Datum datum;
		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(0_z, datum.Capacity());
		Assert::AreEqual(Datum::DatumTypes::Unknown, datum.Type());
		Assert::IsFalse(datum.IsExternal());
	}

	{
		Datum datum;
		datum.Set(value);

		Datum datumCopy(datum);
		Assert::AreEqual(1_z, datumCopy.Size());
		Assert::AreEqual(1_z, datumCopy.Capacity());
		Assert::AreEqual(GetDatumTypeOf<T>(), datumCopy.Type());
		Assert::IsFalse(datumCopy.IsExternal());
	}

	{
		Datum datum;
		datum.Set(value);

		Datum datumCopy;
		datumCopy = datum;
		Assert::AreEqual(1_z, datumCopy.Size());
		Assert::AreEqual(1_z, datumCopy.Capacity());
		Assert::AreEqual(GetDatumTypeOf<T>(), datumCopy.Type());
		Assert::IsFalse(datumCopy.IsExternal());
	}

	{
		Datum datum;
		datum.Set(value);

		Datum datumMove = std::move(datum);
		Assert::AreEqual(1_z, datumMove.Size());
		Assert::AreEqual(1_z, datumMove.Capacity());
		Assert::AreEqual(GetDatumTypeOf<T>(), datumMove.Type());
		Assert::IsFalse(datumMove.IsExternal());

		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(0_z, datum.Capacity());
		Assert::AreEqual(Datum::DatumTypes::Unknown, datum.Type());
		Assert::IsFalse(datum.IsExternal());
	}

	{
		Datum datum;
		datum.Set(value);

		Datum datumMove(std::move(datum));
		Assert::AreEqual(1_z, datumMove.Size());
		Assert::AreEqual(1_z, datumMove.Capacity());
		Assert::AreEqual(GetDatumTypeOf<T>(), datumMove.Type());
		Assert::IsFalse(datumMove.IsExternal());

		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(0_z, datum.Capacity());
		Assert::AreEqual(Datum::DatumTypes::Unknown, datum.Type());
		Assert::IsFalse(datum.IsExternal());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTests)
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



#pragma region Datum (general) Tests

		TEST_METHOD(ConstructorsAndAssignments)
		{
			CheckConstructor<std::int32_t>(11);
			CheckConstructor<std::float_t>(11.1f);
			CheckConstructor<glm::vec4>(glm::vec4(1));
			CheckConstructor<glm::mat4>(glm::mat4(1));
			CheckConstructor<std::string>("Happy");		
		}

		TEST_METHOD(StorageMethods)
		{
			CheckStorageMethods<std::int32_t>(1);
			CheckStorageMethods<std::float_t>(1);
			CheckStorageMethods<glm::vec4>(glm::vec4(1));
			CheckStorageMethods<glm::mat4>(glm::mat4(1));
			CheckStorageMethods<std::string>("Happy");
		}

		TEST_METHOD(ElementAccess)
		{
			CheckElementAccess<std::int32_t>(1);
			CheckElementAccess<std::float_t>(1);
			CheckElementAccess<glm::vec4>(glm::vec4(1));
			CheckElementAccess<glm::mat4>(glm::mat4(1));
			CheckElementAccess<std::string>("Happy");
		}

		TEST_METHOD(Modifiers)
		{
			CheckModifiers<std::int32_t>(1,2);
			CheckModifiers<std::float_t>(1,1.1f);
			CheckModifiers<glm::vec4>(glm::vec4(1), glm::vec4(2));
			CheckModifiers<glm::mat4>(glm::mat4(1), glm::mat4(2));
			CheckModifiers<std::string>("Happy", "Sad");
		}

		TEST_METHOD(RandomTests)
		{
			Datum d;
			Datum d1(Datum::DatumTypes::Float, true, 10_z, [](const size_t, const size_t) { return 12; });
			Datum d2(Datum::DatumTypes::Vector);
			Datum d3(Datum::DatumTypes::Matrix);
			Datum d4(Datum::DatumTypes::String);
			Datum d5(Datum::DatumTypes::Pointer);

			int i = 10;
			d.PushBack(i);
			d.PushBack(11);
			d.PushBack(12);

			float f = 10.0f;
			d1.PushBack(f);

			glm::vec4 v = glm::vec4(glm::vec3(0.0f), 1.0f);
			d2.PushBack(v);
			d2.Resize(5);
			d2.Resize(1);

			d.PopBack();
			d.PopBack();
			d.PopBack();

			d.PushBack(13);
			d.PushBack(14);
			d.PushBack(15);
			d.PushBack(16);
			d.PushBack(17);
			d.PushBack(18);

			i = d.Get<int>();
			i = d.Get<int>(1);
			i = d.Front<int>();
			i = d.Back<int>();

			d1.PopBack();

			v = glm::vec4(glm::vec3(4.0f), 1.0f);

			d2.Set(v);

			d.Find(14);

			d.Remove<int>(17);
			d.RemoveAt<int>(d.Size() - 2);

			d2.PushBack(glm::vec4(glm::vec3(4.0f), 1.0f));
			d2.PushBack(glm::vec4(glm::vec3(3.0f), 1.0f));
			d2.PushBack(glm::vec4(glm::vec3(2.0f), 1.0f));

			d2.Remove<glm::vec4>(glm::vec4(glm::vec3(3.0f), 1.0f));

			d2.RemoveAt<glm::vec4>(0);
			d3.PushBack(glm::mat4(1.0));

			d1.PushBack(35.0f);
			d1.PushBack(1.2f);

			d4.PushBack("Happy");

			auto str = d1.ToString<float>();
			str = d2.ToString<glm::vec4>();
			str = d.ToString<int>();
			str = d4.ToString<std::string>();
			str = d3.ToString<glm::mat4>();

			d2.SetFromString("vec4(1,2,3,4)");
			d3.SetFromString("mat4x4((1,2,3,4),(1,2,3,4),(1,2,3,4),(1,2,3,4))");
		}

#pragma endregion Datum (general) Tests


#pragma region Datum (std::string) Tests

		TEST_METHOD(empty6)
		{
		}

#pragma endregion Datum (std::string) Tests

#pragma region Datum (RTTI*) Tests

		TEST_METHOD(empty7)
		{
		}

#pragma endregion Datum (RTTI*) Tests

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTests::sStartMemState;
}