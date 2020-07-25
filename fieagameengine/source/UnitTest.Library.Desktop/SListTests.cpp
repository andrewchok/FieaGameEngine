#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"

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
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
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
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
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
	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			size_t size = list.Size();
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(std::size_t(0), size);
			Assert::AreEqual(std::size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });

			const SList<Foo> fooList;
			size = fooList.Size();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(std::size_t(0), size);
			Assert::AreEqual(std::size_t(0), fooList.Size());
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Front(); });
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Back(); });
		}

		TEST_METHOD(DeepCopy)
		{
			SList<Foo> list;
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			{
				SList<Foo> listCopy(list);
				while (list.Size() != 0)
				{
					Assert::AreEqual(list.Front(), listCopy.Front());
					Assert::AreEqual(list.Size(), listCopy.Size());
					list.PopFront();
					listCopy.PopFront();
				}
			}

			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			{
				SList<Foo> listCopy(list);
				while (list.Size() != 0)
				{
					Assert::AreEqual(list.Back(), listCopy.Back());
					Assert::AreEqual(list.Size(), listCopy.Size());
					list.PopBack();
					listCopy.PopBack();
				}
			}

			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			{
				SList<Foo> listCopy;
				listCopy = list;
				while (list.Size() != 0)
				{
					Assert::AreEqual(list.Front(), listCopy.Front());
					Assert::AreEqual(list.Size(), listCopy.Size());
					list.PopFront();
					listCopy.PopFront();
				}
			}

			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			{
				SList<Foo> listCopy;
				listCopy = list;
				while (list.Size() != 0)
				{
					Assert::AreEqual(list.Back(), listCopy.Back());
					Assert::AreEqual(list.Size(), listCopy.Size());
					list.PopBack();
					listCopy.PopBack();
				}
			}
			list.PushBack(Foo(10)); 
			list.PushBack(Foo(10)); 
			list.PushBack(Foo(10));

			{
				SList<Foo> listCopy;
				listCopy = list;
				Assert::AreNotSame(list.Front(), listCopy.Back());
				Assert::AreEqual(list.Front(), listCopy.Back());
				while (list.Size() != 0)
				{
					Assert::AreEqual(list.Front(), listCopy.Front());
					Assert::AreEqual(list.Size(), listCopy.Size());
					list.PopFront();
					listCopy.PopFront();
				}
			}
		}

		TEST_METHOD(ShallowCopy)
		{
			SList<Foo> list;
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			{
				SList<Foo> listCopy(std::move(list));
				Assert::IsTrue(list.IsEmpty());
				Assert::IsFalse(listCopy.IsEmpty());
			}


			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			{
				SList<Foo> listCopy;
				listCopy = std::move(list);
				Assert::IsTrue(list.IsEmpty());
				Assert::IsFalse(listCopy.IsEmpty());
			}
		}

		TEST_METHOD(IsEmpty)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			const int data = 10;
			list.PushBack(Foo(data));
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			Assert::AreEqual(std::size_t(1), list.Size());
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Front)
		{
			SList<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });

			auto fooExpression = [&list] { list.Front(); };
			Assert::ExpectException<std::runtime_error>(fooExpression);	

			const int data = 99;
			list.PushBack(Foo(data)); 
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(Foo(data), list.Front());

			
		}

		TEST_METHOD(FrontConst)
		{
			{
				const SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			}

			{
				SList<Foo> list;
				const int data = 99;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));

				const SList<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
			}
		}

		TEST_METHOD(Back)
		{
			SList<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });

			const int data = 99;
			list.PushBack(Foo(30));
			list.PushBack(Foo(20));
			list.PushBack(Foo(data));
			Assert::AreEqual(Foo(data), list.Back());
		}

		TEST_METHOD(BackConst)
		{
			{
				const SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
			}

			{
				SList<Foo> list;
				const int data = 99;
				list.PushBack(Foo(30));
				list.PushBack(Foo(20));
				list.PushBack(Foo(data));

				const SList<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Back());
			}
		}

		TEST_METHOD(PushFront)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });

			Foo data = Foo(10);
			list.PushFront(data);
			Assert::AreEqual(data, list.Front());
			Assert::AreEqual(data, list.Back());
			Assert::AreEqual(std::size_t(1), list.Size());

			list.PushFront(Foo(20));
			Assert::AreEqual(Foo(20), list.Front());
			Assert::AreEqual(Foo(10), list.Back());
			Assert::AreEqual(std::size_t(2), list.Size());

			//add move semantics to check front
		}

		TEST_METHOD(PushBack)
		{
			SList<Foo> list;
			Foo data = Foo(10);
			Assert::
			Assert::AreEqual(std::size_t(0), list.Size());
			Assert::IsTrue(list.IsEmpty());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });

			list.PushBack(data);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(data, list.Front());
			Assert::AreEqual(data, list.Back());
			Assert::AreEqual(std::size_t(1), list.Size());

			list.PushBack(Foo(20));
			Assert::AreEqual(data, list.Front());
			Assert::AreEqual(Foo(20), list.Back());
			Assert::AreEqual(std::size_t(2), list.Size());
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;

			list.PopFront();

			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			Assert::AreEqual(std::size_t(1), list.Size());

			list.PopFront();
			Assert::IsTrue(list.IsEmpty());

			const int data2 = 20;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data2));
			Assert::AreEqual(std::size_t(2), list.Size());
			Assert::AreEqual(Foo(data), list.Front());

			list.PopFront();
			Assert::AreEqual(std::size_t(1), list.Size());
			Assert::AreEqual(Foo(data2), list.Front());
		}
		
		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			list.PopBack();
			Assert::AreEqual(std::size_t(2), list.Size());
			Assert::AreEqual(Foo(data), list.Front());
			Assert::AreEqual(Foo(20), list.Back());

			list.PopBack();
			Assert::AreEqual(std::size_t(1), list.Size());
			Assert::AreEqual(list.Back(), list.Front());

			list.PopBack();
			Assert::AreEqual(std::size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
		}

		// assignment 3 tests: Iterator Add-On

		TEST_METHOD(Begin)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());
			
			Assert::AreEqual(Foo(data), *(list.begin()));
		}

		TEST_METHOD(BeginConst)
		{
			{
				const SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.begin()); });
			}

			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(std::size_t(3), list.Size());

				const SList<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::AreEqual(Foo(data), *(listCopy.begin()));
			}
		}

		TEST_METHOD(CBegin)
		{
			{
				const SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cbegin()); });
			}

			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(std::size_t(3), list.Size());

				const SList<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::AreEqual(Foo(data), *(listCopy.cbegin()));
			}
		}

		TEST_METHOD(End)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
		}

		TEST_METHOD(EndConst)
		{
			{
				const SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			}

			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(std::size_t(3), list.Size());

				const SList<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			}
		}

		TEST_METHOD(CEnd)
		{
			{
				const SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			}

			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(std::size_t(3), list.Size());

				const SList<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			}
		}

		TEST_METHOD(PreIncrement)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			auto it = list.begin();
			
			Assert::AreEqual(Foo(data), *it);

			auto preInc = ++it;
			Assert::AreEqual(Foo(20), *preInc);
			Assert::AreEqual(Foo(20), *it);

			it++;
			preInc++;
			Assert::AreEqual(Foo(30), *preInc);
			Assert::AreEqual(Foo(30), *it);

			it++;
			preInc++;
			Assert::ExpectException<std::runtime_error>([&preInc] { *preInc; });
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { it++; });
		}

		TEST_METHOD(PreIncrementConst)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			const SList<Foo> listCopy(list);

			auto it = listCopy.begin();

			Assert::AreEqual(Foo(data), *it);

			auto preInc = ++it;
			Assert::AreEqual(Foo(20), *preInc);
			Assert::AreEqual(Foo(20), *it);

			it++;
			preInc++;
			Assert::AreEqual(Foo(30), *preInc);
			Assert::AreEqual(Foo(30), *it);

			it++;
			preInc++;
			Assert::ExpectException<std::runtime_error>([&preInc] { *preInc; });
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { it++; });
		}

		TEST_METHOD(PostIncrement)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			auto it = list.begin();
			Assert::AreEqual(Foo(data), *it);

			auto postInc = it++;
			Assert::AreEqual(Foo(data), *postInc);
			Assert::AreEqual(Foo(20), *it);

			it++;
			postInc++;
			Assert::AreEqual(Foo(20), *postInc);
			Assert::AreEqual(Foo(30), *it);

			it++;
			postInc++;
			Assert::AreEqual(Foo(30), *postInc);
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { it++; });
		}

		TEST_METHOD(PostIncrementConst)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			const SList<Foo> listCopy(list);

			auto it = listCopy.begin();
			Assert::AreEqual(Foo(data), *it);

			auto postInc = it++;
			Assert::AreEqual(Foo(data), *postInc);
			Assert::AreEqual(Foo(20), *it);

			it++;
			postInc++;
			Assert::AreEqual(Foo(20), *postInc);
			Assert::AreEqual(Foo(30), *it);

			it++;
			postInc++;
			Assert::AreEqual(Foo(30), *postInc);
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { it++; });
		}

		TEST_METHOD(IteratorEquality)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());
			
			auto it = list.begin();
			auto itCopy = list.begin();

			Assert::IsTrue(it == itCopy);
			Assert::IsFalse(it != itCopy);

			itCopy++;
			Assert::IsFalse(it == itCopy);
			Assert::IsTrue(it != itCopy);


			SList<Foo> listCopy(list);
			auto itCopy2 = listCopy.begin();
			Assert::IsFalse(it == itCopy2);
			Assert::IsTrue(it != itCopy2);


			SList<Foo> listEmpty;
			auto itEmpty = listEmpty.begin();
			Assert::IsFalse(it == itEmpty);
			Assert::IsTrue(it != itEmpty);
		}

		TEST_METHOD(IteratorEqualityConst)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			const SList<Foo> listConst(list);

			auto it = listConst.begin();
			auto itCopy = listConst.begin();

			Assert::IsTrue(it == itCopy);
			Assert::IsFalse(it != itCopy);

			itCopy++;
			Assert::IsFalse(it == itCopy);
			Assert::IsTrue(it != itCopy);


			const SList<Foo> listCopy(listConst);
			auto itCopy2 = listCopy.begin();
			Assert::IsFalse(it == itCopy2);
			Assert::IsTrue(it != itCopy2);


			const SList<Foo> listEmpty;
			auto itEmpty = listEmpty.begin();
			Assert::IsFalse(it == itEmpty);
			Assert::IsTrue(it != itEmpty);
		}

		TEST_METHOD(InsertAfter)
		{
			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				auto it = list.begin();
				it = list.InsertAfter(Foo(data), it);
				Assert::AreEqual(std::size_t(1), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(data), list.Back());

				it = list.InsertAfter(Foo(20), it);
				Assert::AreEqual(std::size_t(2), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(20), list.Back());

				Assert::AreEqual(Foo(30), *(list.InsertAfter(Foo(30), it)));
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(30), list.Back());
			}

			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				auto it = list.begin();
				list.InsertAfter(Foo(data), it); // list = {Foo(10)}
				Assert::AreEqual(std::size_t(1), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(data), list.Back());

				it = list.begin();
				list.InsertAfter(Foo(20), it); // list = {Foo(10), Foo(20)}
				Assert::AreEqual(std::size_t(2), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(20), list.Back());

				it = list.begin();
				it = list.InsertAfter(Foo(30), it); // list = {Foo(10), Foo(30), Foo(20)}
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(20), list.Back());
				Assert::AreEqual(Foo(30), *it);


				list.InsertAfter(Foo(40), it); // list = {Foo(10), Foo(30), Foo(40) Foo(20)}
				Assert::AreEqual(std::size_t(4), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(20), list.Back());

				list.PopFront();
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(30), list.Front());

				list.PopFront();
				Assert::AreEqual(std::size_t(2), list.Size());
				Assert::AreEqual(Foo(40), list.Front());
			}

			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				auto it = list.end();
				it = list.InsertAfter(Foo(data), it); // list = {Foo(10)}
				Assert::AreEqual(std::size_t(1), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(data), list.Back());
				Assert::AreEqual(Foo(data), *it);

				it = list.InsertAfter(Foo(20), it); // list = {Foo(10), Foo(20)}
				Assert::AreEqual(std::size_t(2), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(20), list.Back());
				Assert::AreEqual(Foo(20), *it);

				it = list.end();
				it = list.InsertAfter(Foo(30), it); // list = {Foo(10), Foo(20), Foo(30)}
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(30), list.Back());
				Assert::AreEqual(Foo(30), *it);
			}
		}

		TEST_METHOD(Find)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			auto it = list.Find(Foo(data));
			Assert::AreEqual(Foo(data), *it);

			it = list.Find(Foo(20));
			Assert::AreEqual(Foo(20), *it);

			it = list.Find(Foo(30));
			Assert::AreEqual(Foo(30), *it);
		}

		TEST_METHOD(FindConst)
		{
			SList<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			const SList<Foo> listConst(list);

			auto it = listConst.Find(Foo(data));
			Assert::AreEqual(Foo(data), *it);

			it = listConst.Find(Foo(20));
			Assert::AreEqual(Foo(20), *it);

			it = listConst.Find(Foo(30));
			Assert::AreEqual(Foo(30), *it);
		}

		TEST_METHOD(Remove)
		{
			// front remove
			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t(0), list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				list.PushBack(Foo(40));
				Assert::AreEqual(std::size_t(4), list.Size());

				list.Remove(Foo(data));
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(20), list.Front());
			}

			// middle remove
			{
				SList<Foo> list;
				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				list.PushBack(Foo(40));

				list.Remove(Foo(20));
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(data), list.Front());

				list.PopFront();
				Assert::AreEqual(std::size_t(2), list.Size());
				Assert::AreEqual(Foo(30), list.Front());
			}

			// back remove
			{
				SList<Foo> list;
				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				list.PushBack(Foo(40));

				list.Remove(Foo(40));
				Assert::AreEqual(std::size_t(3), list.Size());
				Assert::AreEqual(Foo(30), list.Back());
			}

			// No remove
			{
				SList<Foo> list;
				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				list.PushBack(Foo(40));

				list.Remove(Foo(50));
				Assert::AreEqual(std::size_t(4), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(40), list.Back());
			}

			// Single item no remove
			{
				SList<Foo> list;
				const int data = 10;
				list.PushBack(Foo(data));

				list.Remove(Foo(50));
				Assert::AreEqual(std::size_t(1), list.Size());
				Assert::AreEqual(Foo(data), list.Front());
				Assert::AreEqual(Foo(data), list.Back());
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTests::sStartMemState;
}
