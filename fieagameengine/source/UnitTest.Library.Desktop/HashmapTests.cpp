#include "pch.h"
#include "CppUnitTest.h"
#include "hashmap.h"
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
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashmapTests)
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

#pragma region Iterator Tests

		TEST_METHOD(PreIncrement)
		{
			using TKey = Foo;
			using TData = int;

			{
				auto itEmpty = Hashmap<TKey, TData>::Iterator();
				Assert::ExpectException<std::runtime_error>([&itEmpty] { ++itEmpty; });
			}

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			{
				auto it = list.begin();
				Assert::ExpectException<std::runtime_error>([&it] { ++it; });
			}

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			auto it = list.begin();
			Assert::AreEqual(Foo(12), (*it).first);
			Assert::AreEqual(21, (*it).second);

			auto preInc = ++it;

			Assert::AreEqual(Foo(13), (*it).first);
			Assert::AreEqual(31, (*it).second);
			Assert::AreEqual(Foo(13), (*preInc).first);
			Assert::AreEqual(31, (*preInc).second);

			++it;
			++preInc;

			Assert::AreEqual(Foo(14), (*it).first);
			Assert::AreEqual(41, (*it).second);
			Assert::AreEqual(Foo(14), (*preInc).first);
			Assert::AreEqual(41, (*preInc).second);

			++it;
			++preInc;
			Assert::ExpectException<std::runtime_error>([&preInc] { *preInc; });
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { ++it; });
		}

		TEST_METHOD(PostIncrement)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			{
				auto it = list.begin();
				Assert::ExpectException<std::runtime_error>([&it] { it++; });
			}

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			auto it = list.begin();
			Assert::AreEqual(Foo(12), (*it).first);
			Assert::AreEqual(21, (*it).second);

			auto postInc = it++;

			Assert::AreEqual(Foo(13), (*it).first);
			Assert::AreEqual(31, (*it).second);
			Assert::AreEqual(Foo(12), (*postInc).first);
			Assert::AreEqual(21, (*postInc).second);

			it++;
			postInc++;

			Assert::AreEqual(Foo(14), (*it).first);
			Assert::AreEqual(41, (*it).second);
			Assert::AreEqual(Foo(13), (*postInc).first);
			Assert::AreEqual(31, (*postInc).second);

			it++;
			postInc++;
			Assert::AreEqual(Foo(14), (*postInc).first);
			Assert::AreEqual(41, (*postInc).second);
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { it++; });
		}
		
		TEST_METHOD(IteratorEquality)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			{
				auto it = list.begin();
				auto it2 = list.end();
				Assert::IsTrue(it == it);
				Assert::IsFalse(it != it);
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
			}

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			{
				auto it = list.begin();
				auto it2 = list.begin();

				++it;
				Assert::IsTrue(it != it2);
				Assert::IsFalse(it == it2);

				++it2;
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
			}

			Hashmap<TKey, TData> list2;
			list2.Insert(std::pair<TKey, TData>(Foo(12), 21));
			auto it = list.begin();
			auto it2 = list2.begin();
			Assert::IsTrue(it != it2);
			Assert::IsFalse(it == it2);
		}

		TEST_METHOD(Dereference)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			Assert::ExpectException<std::runtime_error>([&list] { *(list.begin()); });
			Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			
			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));
			
			Assert::AreEqual(Foo(12), (list.begin())->first);
			Assert::AreEqual(21, (list.begin())->second);
		}

#pragma endregion Iterator Tests

#pragma region ConstIterator Tests

		TEST_METHOD(PreIncrementConst)
		{
			using TKey = Foo;
			using TData = int;

			{
				auto itEmpty = Hashmap<TKey, TData>::ConstIterator();
				Assert::ExpectException<std::runtime_error>([&itEmpty] { ++itEmpty; });
			}

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			{
				const Hashmap<TKey, TData> listConst(list);
				auto it = listConst.begin();
				Assert::ExpectException<std::runtime_error>([&it] { ++it; });
			}

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			const Hashmap<TKey, TData> listConst(list);

			auto it = listConst.begin();
			Assert::AreEqual(Foo(12), (*it).first);
			Assert::AreEqual(21, (*it).second);

			auto preInc = ++it;

			Assert::AreEqual(Foo(13), (*it).first);
			Assert::AreEqual(31, (*it).second);
			Assert::AreEqual(Foo(13), (*preInc).first);
			Assert::AreEqual(31, (*preInc).second);

			++it;
			++preInc;

			Assert::AreEqual(Foo(14), (*it).first);
			Assert::AreEqual(41, (*it).second);
			Assert::AreEqual(Foo(14), (*preInc).first);
			Assert::AreEqual(41, (*preInc).second);

			++it;
			++preInc;
			Assert::ExpectException<std::runtime_error>([&preInc] { *preInc; });
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { ++it; });
		}

		TEST_METHOD(PostIncrementConst)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			{
				const Hashmap<TKey, TData> listConst = list;
				auto it = listConst.begin();
				Assert::ExpectException<std::runtime_error>([&it] { it++; });
			}

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			const Hashmap<TKey, TData> listConst = list;

			auto it = listConst.begin();
			Assert::AreEqual(Foo(12), (*it).first);
			Assert::AreEqual(21, (*it).second);

			auto postInc = it++;

			Assert::AreEqual(Foo(13), (*it).first);
			Assert::AreEqual(31, (*it).second);
			Assert::AreEqual(Foo(12), (*postInc).first);
			Assert::AreEqual(21, (*postInc).second);

			it++;
			postInc++;

			Assert::AreEqual(Foo(14), (*it).first);
			Assert::AreEqual(41, (*it).second);
			Assert::AreEqual(Foo(13), (*postInc).first);
			Assert::AreEqual(31, (*postInc).second);

			it++;
			postInc++;
			Assert::AreEqual(Foo(14), (*postInc).first);
			Assert::AreEqual(41, (*postInc).second);
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { it++; });
		}

		TEST_METHOD(IteratorEqualityConst)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			{
				auto it = list.cbegin();
				auto it2 = list.cend();
				Assert::IsTrue(it == it);
				Assert::IsFalse(it != it);
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
			}

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			{
				auto it = list.cbegin();
				auto it2 = list.cbegin();

				++it;
				Assert::IsTrue(it != it2);
				Assert::IsFalse(it == it2);

				++it2;
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
			}

			Hashmap<TKey, TData> list2;
			list2.Insert(std::pair<TKey, TData>(Foo(12), 21));
			auto it = list.cbegin();
			auto it2 = list2.cbegin();
			Assert::IsTrue(it != it2);
			Assert::IsFalse(it == it2);
		}

		TEST_METHOD(DereferenceConst)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			Assert::ExpectException<std::runtime_error>([&list] { *(list.cbegin()); });
			Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });

			list.Insert(std::pair<TKey, TData>(Foo(12), 21));
			list.Insert(std::pair<TKey, TData>(Foo(13), 31));
			list.Insert(std::pair<TKey, TData>(Foo(14), 41));

			Assert::AreEqual(Foo(12), (list.cbegin())->first);
			Assert::AreEqual(21, (list.cbegin())->second);
		}

#pragma endregion ConstIterator Tests

#pragma region Hashmap Tests
		
		TEST_METHOD(HashmapConstructor_int_int)
		{
			using TKey = int;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			const Hashmap<TKey, TData> otherList;
			Assert::AreEqual(0_z, otherList.Size());
			Assert::AreEqual(31_z, otherList.BucketSize());

			Hashmap<TKey, TData> initList(79, [](const TKey&) { return 10; });
			Assert::AreEqual(0_z, initList.Size());
			Assert::AreEqual(79_z, initList.BucketSize());
		}

		TEST_METHOD(HashmapConstructor_Foo_int)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			const Hashmap<TKey, TData> otherList;
			Assert::AreEqual(0_z, otherList.Size());
			Assert::AreEqual(31_z, otherList.BucketSize());

			Hashmap<TKey, TData> initList(79, [](const TKey&) { return 10; });
			Assert::AreEqual(0_z, initList.Size());
			Assert::AreEqual(79_z, initList.BucketSize());
		}

		TEST_METHOD(DeepCopy)
		{
			using TKey = Foo;
			using TData = int;

			{
				Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());

				std::pair<TKey, TData> temp(Foo(11), 10);
				list.Insert(temp);
				list.Insert(std::pair<TKey, TData>(Foo(12), 14));

				Hashmap<TKey, TData> listCopy(list);

				auto it = list.begin();
				auto itCopy = listCopy.begin();

				while (it != list.end())
				{
					Assert::AreEqual((*it).first, (*itCopy).first);
					Assert::AreEqual((*it).second, (*itCopy).second);
					++it;
					++itCopy;
				}
			}

			{
				Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());

				std::pair<TKey, TData> temp(Foo(11), 10);
				list.Insert(temp);
				list.Insert(std::pair<TKey, TData>(Foo(12), 14));

				Hashmap<TKey, TData> listCopy;
				listCopy = list;

				auto it = list.begin();
				auto itCopy = listCopy.begin();

				while (it != list.end())
				{
					Assert::AreEqual((*it).first, (*itCopy).first);
					Assert::AreEqual((*it).second, (*itCopy).second);
					++it;
					++itCopy;
				}
			}

			{
				Hashmap<TKey, TData> list;
				list.Insert(std::pair<TKey, TData>(Foo(12), 14));
				Hashmap<TKey, TData> listCopy;
				listCopy = list;

				Assert::AreNotSame((*(list.begin())).first, (*(listCopy.begin())).first);
				Assert::AreNotSame((*(list.begin())).second, (*(listCopy.begin())).second);
				Assert::AreEqual((*(list.begin())).first, (*(listCopy.begin())).first);
				Assert::AreEqual((*(list.begin())).second, (*(listCopy.begin())).second);
			}
		}

		TEST_METHOD(ShallowCopy)
		{
			using TKey = Foo;
			using TData = int;

			{
				Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());

				std::pair<TKey, TData> temp(Foo(11), 10);
				list.Insert(temp);
				list.Insert(std::pair<TKey, TData>(Foo(12), 14));

				Hashmap<TKey, TData> listCopy(std::move(list));
				
				Assert::AreEqual(31_z, listCopy.BucketSize());
				Assert::AreEqual(2_z, listCopy.Size());
			}

			{
				Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());

				std::pair<TKey, TData> temp(Foo(11), 10);
				list.Insert(temp);
				list.Insert(std::pair<TKey, TData>(Foo(12), 14));

				Hashmap<TKey, TData> listCopy;
				listCopy = std::move(list);

				Assert::AreEqual(31_z, listCopy.BucketSize());
				Assert::AreEqual(2_z, listCopy.Size());
			}
		}
		
		TEST_METHOD(Size)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			Assert::AreEqual(1_z, list.Size());
			list.Insert(std::pair<TKey, TData>(Foo(13), 14));
			Assert::AreEqual(2_z, list.Size());

			const Hashmap<TKey, TData> otherList;
			Assert::AreEqual(0_z, otherList.Size());
			Assert::AreEqual(31_z, otherList.BucketSize());
		}

		TEST_METHOD(BucketSize)
		{
			using TKey = int;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			const Hashmap<TKey, TData> otherList;
			Assert::AreEqual(0_z, otherList.Size());
			Assert::AreEqual(31_z, otherList.BucketSize());

			Hashmap<TKey, TData> initList(79, [](const TKey&) { return 10; });
			Assert::AreEqual(0_z, initList.Size());
			Assert::AreEqual(79_z, initList.BucketSize());
		}

		TEST_METHOD(Clear)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			list.Insert(std::pair<TKey, TData>(Foo(13), 14));
			Assert::AreEqual(2_z, list.Size());
			
			list.Clear();
			Assert::AreEqual(0_z, list.Size());
		}
		
		TEST_METHOD(Begin)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			Assert::AreEqual(Foo(11), (*(list.begin())).first);
			Assert::AreEqual(10, (*(list.begin())).second);

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			Assert::AreEqual(11, (*(otherList.begin())).first);
			Assert::AreEqual(10, (*(otherList.begin())).second);
		}

		TEST_METHOD(BeginConst)
		{
			using TKey = Foo;
			using TData = int;

			{
				const Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.begin()); });
			}

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			const Hashmap<TKey, TData> constList = list;
			Assert::AreEqual(Foo(11), (*(constList.begin())).first);
			Assert::AreEqual(10, (*(constList.begin())).second);

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> constOtherList(otherList);
			Assert::AreEqual(11, (*(constOtherList.begin())).first);
			Assert::AreEqual(10, (*(constOtherList.begin())).second);
		}

		TEST_METHOD(CBegin)
		{
			using TKey = Foo;
			using TData = int;

			{
				const Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cbegin()); });
			}

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			const Hashmap<TKey, TData> constList = list;
			Assert::AreEqual(Foo(11), (*(constList.cbegin())).first);
			Assert::AreEqual(10, (*(constList.cbegin())).second);

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> constOtherList(otherList);
			Assert::AreEqual(11, (*(constOtherList.cbegin())).first);
			Assert::AreEqual(10, (*(constOtherList.cbegin())).second);
		}

		TEST_METHOD(End)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));
			
			Assert::ExpectException<std::runtime_error>([&otherList] { *(otherList.end()); });
		}

		TEST_METHOD(EndConst)
		{
			using TKey = Foo;
			using TData = int;

			{
				const Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			}

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			const Hashmap<TKey, TData> constList = list;
			Assert::ExpectException<std::runtime_error>([&constList] { *(constList.end()); });

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> constOtherList(otherList);
			Assert::ExpectException<std::runtime_error>([&constOtherList] { *(constOtherList.end()); });
		}

		TEST_METHOD(CEnd)
		{
			using TKey = Foo;
			using TData = int;

			{
				const Hashmap<TKey, TData> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			}

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			const Hashmap<TKey, TData> constList = list;
			Assert::ExpectException<std::runtime_error>([&constList] { *(constList.cend()); });

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> constOtherList(otherList);
			Assert::ExpectException<std::runtime_error>([&constOtherList] { *(constOtherList.cend()); });
		}
		
		TEST_METHOD(BracketIndex)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			list[Foo(3)];
			Assert::AreEqual(1_z, list.Size());
			Assert::IsTrue(list.ContainsKey(Foo(3)));
			Assert::AreEqual(0, list[Foo(3)]);
			
			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			list.Insert(std::pair<TKey, TData>(Foo(13), 7));
			Assert::IsTrue(list.ContainsKey(Foo(11)));
			Assert::IsTrue(list.ContainsKey(Foo(12)));
			Assert::IsTrue(list.ContainsKey(Foo(13)));

			Assert::AreEqual(10, list[Foo(11)]);
			Assert::AreEqual(14, list[Foo(12)]);
			Assert::AreEqual(7, list[Foo(13)]);

			list[Foo(14)] = 100;
			Assert::AreEqual(5_z, list.Size());
			Assert::IsTrue(list.ContainsKey(Foo(14)));
			Assert::AreEqual(100, list[Foo(14)]);

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));
			Assert::IsTrue(otherList.ContainsKey(11));
			Assert::IsTrue(otherList.ContainsKey(12));
			Assert::IsFalse(otherList.ContainsKey(13));

			Assert::AreEqual(10, otherList[11]);
			Assert::AreEqual(14, otherList[12]);
			Assert::AreEqual(2_z, otherList.Size());

			otherList[14];
			Assert::AreEqual(3_z, otherList.Size());
			otherList[14] = 90;
			Assert::AreEqual(90, otherList[14]);
		}

		TEST_METHOD(BracketIndexConst)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			const Hashmap<TKey, TData> listConstEmpty = list;
			Assert::ExpectException<std::runtime_error>([&listConstEmpty] { listConstEmpty[Foo(3)]; });

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			list.Insert(std::pair<TKey, TData>(Foo(13), 7));

			const Hashmap<TKey, TData> listConst = list;
			Assert::IsTrue(listConst.ContainsKey(Foo(11)));
			Assert::IsTrue(listConst.ContainsKey(Foo(12)));
			Assert::IsTrue(listConst.ContainsKey(Foo(13)));

			Assert::AreEqual(10, listConst[Foo(11)]);
			Assert::AreEqual(14, listConst[Foo(12)]);
			Assert::AreEqual(7, listConst[Foo(13)]);
			Assert::ExpectException<std::runtime_error>([&listConst] { listConst[Foo(14)]; });

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> otherListConst = otherList;
			Assert::IsTrue(otherListConst.ContainsKey(11));
			Assert::IsTrue(otherListConst.ContainsKey(12));
			Assert::IsFalse(otherListConst.ContainsKey(13));

			Assert::AreEqual(10, otherListConst[11]);
			Assert::AreEqual(14, otherListConst[12]);
			Assert::ExpectException<std::runtime_error>([&otherListConst] { otherListConst[14]; });
		}

		TEST_METHOD(At)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());
			Assert::ExpectException<std::runtime_error>([&list] { list.At(Foo(3)); });

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			list.Insert(std::pair<TKey, TData>(Foo(13), 7));
			Assert::IsTrue(list.ContainsKey(Foo(11)));
			Assert::IsTrue(list.ContainsKey(Foo(12)));
			Assert::IsTrue(list.ContainsKey(Foo(13)));

			Assert::AreEqual(10, list.At(Foo(11)));
			Assert::AreEqual(14, list.At(Foo(12)));
			Assert::AreEqual(7, list.At(Foo(13)));
			Assert::ExpectException<std::runtime_error>([&list] { list.At(Foo(14)); });

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));
			Assert::IsTrue(otherList.ContainsKey(11));
			Assert::IsTrue(otherList.ContainsKey(12));
			Assert::IsFalse(otherList.ContainsKey(13));

			Assert::AreEqual(10, otherList.At(11));
			Assert::AreEqual(14, otherList.At(12));
			Assert::ExpectException<std::runtime_error>([&otherList] { otherList.At(14); });
		}

		TEST_METHOD(AtConst)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			const Hashmap<TKey, TData> listConstEmpty = list;
			Assert::ExpectException<std::runtime_error>([&listConstEmpty] { listConstEmpty.At(Foo(3)); });

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			list.Insert(std::pair<TKey, TData>(Foo(13), 7));

			const Hashmap<TKey, TData> listConst = list;
			Assert::IsTrue(listConst.ContainsKey(Foo(11)));
			Assert::IsTrue(listConst.ContainsKey(Foo(12)));
			Assert::IsTrue(listConst.ContainsKey(Foo(13)));

			Assert::AreEqual(10, listConst.At(Foo(11)));
			Assert::AreEqual(14, listConst.At(Foo(12)));
			Assert::AreEqual(7, listConst.At(Foo(13)));
			Assert::ExpectException<std::runtime_error>([&listConst] { listConst.At(Foo(14)); });

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> otherListConst = otherList;
			Assert::IsTrue(otherListConst.ContainsKey(11));
			Assert::IsTrue(otherListConst.ContainsKey(12));
			Assert::IsFalse(otherListConst.ContainsKey(13));

			Assert::AreEqual(10, otherListConst.At(11));
			Assert::AreEqual(14, otherListConst.At(12));
			Assert::ExpectException<std::runtime_error>([&otherListConst] { otherListConst.At(14); });
		}
		
		TEST_METHOD(Insert)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			{
				auto tuple = list.Insert(temp);
				Assert::AreEqual(1_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());
				Assert::IsTrue(std::get<1>(tuple));

				auto it = std::get<0>(tuple);
				Assert::AreEqual(Foo(11), (*it).first);
				Assert::AreEqual(10, (*it).second);
			}
			
			{
				auto tuple = list.Insert(temp);
				Assert::AreEqual(1_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());
				Assert::IsFalse(std::get<1>(tuple));

				auto it = std::get<0>(tuple);
				Assert::AreEqual(Foo(11), (*it).first);
				Assert::AreEqual(10, (*it).second);

				tuple = list.Insert(std::pair<TKey, TData>(Foo(12), 14));
				Assert::AreEqual(2_z, list.Size());
				Assert::AreEqual(31_z, list.BucketSize());
				Assert::IsTrue(std::get<1>(tuple));

				it = std::get<0>(tuple);
				Assert::AreEqual(Foo(12), (*it).first);
				Assert::AreEqual(14, (*it).second);
			}

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			{
				auto tuple = otherList.Insert(temp2);
				Assert::AreEqual(1_z, otherList.Size());
				Assert::AreEqual(31_z, otherList.BucketSize());
				Assert::IsTrue(std::get<1>(tuple));

				auto it = std::get<0>(tuple);
				Assert::AreEqual(11, (*it).first);
				Assert::AreEqual(10, (*it).second);
			}

			{
				auto tuple = otherList.Insert(temp2);
				Assert::AreEqual(1_z, otherList.Size());
				Assert::AreEqual(31_z, otherList.BucketSize());
				Assert::IsFalse(std::get<1>(tuple));

				auto it = std::get<0>(tuple);
				Assert::AreEqual(11, (*it).first);
				Assert::AreEqual(10, (*it).second);

				tuple = otherList.Insert(std::pair<int, TData>(12, 14));
				Assert::AreEqual(2_z, otherList.Size());
				Assert::AreEqual(31_z, otherList.BucketSize());
				Assert::IsTrue(std::get<1>(tuple));

				it = std::get<0>(tuple);
				Assert::AreEqual(12, (*it).first);
				Assert::AreEqual(14, (*it).second);
			}

			Hashmap<TKey, TData> initList(79, [](const TKey&) { return 10; });
			Assert::AreEqual(0_z, initList.Size());
			Assert::AreEqual(79_z, initList.BucketSize());

			std::pair<TKey, TData> temp3(Foo(11), 10);
			{
				auto tuple = initList.Insert(temp3);
				Assert::AreEqual(1_z, initList.Size());
				Assert::AreEqual(79_z, initList.BucketSize());
				Assert::IsTrue(std::get<1>(tuple));

				auto it = std::get<0>(tuple);
				Assert::AreEqual(Foo(11), (*it).first);
				Assert::AreEqual(10, (*it).second);
			}

			{
				auto tuple = initList.Insert(temp3);
				Assert::AreEqual(1_z, initList.Size());
				Assert::AreEqual(79_z, initList.BucketSize());
				Assert::IsFalse(std::get<1>(tuple));

				auto it = std::get<0>(tuple);
				Assert::AreEqual(Foo(11), (*it).first);
				Assert::AreEqual(10, (*it).second);

				tuple = initList.Insert(std::pair<TKey, TData>(Foo(12), 14));
				Assert::AreEqual(2_z, initList.Size());
				Assert::AreEqual(79_z, initList.BucketSize());
				Assert::IsTrue(std::get<1>(tuple));

				it = std::get<0>(tuple);
				Assert::AreEqual(Foo(12), (*it).first);
				Assert::AreEqual(14, (*it).second);
			}
		}
		
		TEST_METHOD(ContainsKey)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			Assert::IsTrue(list.ContainsKey(Foo(11)));
			Assert::IsTrue(list.ContainsKey(Foo(12)));
			Assert::IsFalse(list.ContainsKey(Foo(13)));

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));
			Assert::IsTrue(otherList.ContainsKey(11));
			Assert::IsTrue(otherList.ContainsKey(12));
			Assert::IsFalse(otherList.ContainsKey(13));
		}

		TEST_METHOD(Find)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			auto it = list.Find(Foo(11));
			Assert::AreEqual(Foo(11), (*it).first);
			Assert::AreEqual(10, (*it).second);
			it = list.Find(Foo(40));
			Assert::ExpectException<std::runtime_error>([&it] { *it; });


			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));
			auto otherIt = otherList.Find(12);
			Assert::AreEqual(12, (*otherIt).first);
			Assert::AreEqual(14, (*otherIt).second);
			otherIt = otherList.Find(40);
			Assert::ExpectException<std::runtime_error>([&otherIt] { *otherIt; });	
		}

		TEST_METHOD(FindConst)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));

			const Hashmap<TKey, TData> listCopy(list);
			auto it = listCopy.Find(Foo(11));
			Assert::AreEqual(Foo(11), (*it).first);
			Assert::AreEqual(10, (*it).second);
			it = listCopy.Find(Foo(40));
			Assert::ExpectException<std::runtime_error>([&it] { *it; });


			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));

			const Hashmap<int, TData> otherListCopy(otherList);
			auto otherIt = otherListCopy.Find(12);
			Assert::AreEqual(12, (*otherIt).first);
			Assert::AreEqual(14, (*otherIt).second);
			otherIt = otherListCopy.Find(40);
			Assert::ExpectException<std::runtime_error>([&otherIt] { *otherIt; });
		}

		TEST_METHOD(Remove)
		{
			using TKey = Foo;
			using TData = int;

			Hashmap<TKey, TData> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(31_z, list.BucketSize());

			std::pair<TKey, TData> temp(Foo(11), 10);
			list.Insert(temp);
			list.Insert(std::pair<TKey, TData>(Foo(12), 14));
			Assert::IsTrue(list.ContainsKey(Foo(11)));
			Assert::IsTrue(list.ContainsKey(Foo(12)));
			Assert::AreEqual(2_z, list.Size());

			list.Remove(Foo(11));
			Assert::IsFalse(list.ContainsKey(Foo(11)));
			Assert::AreEqual(1_z, list.Size());

			list.Remove(Foo(14));
			Assert::AreEqual(1_z, list.Size());

			Hashmap<int, TData> otherList;
			std::pair<int, TData> temp2(11, 10);
			otherList.Insert(temp2);
			otherList.Insert(std::pair<int, TData>(12, 14));
			Assert::IsTrue(otherList.ContainsKey(11));
			Assert::IsTrue(otherList.ContainsKey(12));
			Assert::AreEqual(2_z, otherList.Size());

			otherList.Remove(12);
			Assert::IsFalse(otherList.ContainsKey(12));
			Assert::AreEqual(1_z, otherList.Size());

			otherList.Remove(14);
			Assert::AreEqual(1_z, otherList.Size());

			{
				Hashmap<TKey, TData> list2;
				list2.Insert(std::pair<TKey, TData>(Foo(12), 14));
				auto it2 = list2.begin();
				Assert::ExpectException<std::runtime_error>([&list, &it2] { list.Remove(it2); });
			}
		}

#pragma endregion Hashmap Tests


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashmapTests::sStartMemState;
}