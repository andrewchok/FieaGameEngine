#include "pch.h"
#include "CppUnitTest.h"
#include "vector.h"
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
	TEST_CLASS(VectorTests)
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
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::Iterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { ++itEmpty; });

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			auto it = list.begin();

			Assert::AreEqual(Foo(data), *it);

			auto preInc = ++it;
			Assert::AreEqual(Foo(20), *preInc);
			Assert::AreEqual(Foo(20), *it);

			++it;
			++preInc;
			Assert::AreEqual(Foo(30), *preInc);
			Assert::AreEqual(Foo(30), *it);

			++it;
			++preInc;
			Assert::ExpectException<std::runtime_error>([&preInc] { *preInc; });
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { ++it; });
		}

		TEST_METHOD(PostIncrement)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::Iterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { itEmpty++; });

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

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
		
		TEST_METHOD(PreDecrement)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::Iterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { --itEmpty; });

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			auto it = list.end();

			Assert::AreEqual(Foo(30), *(--it));

			auto preDec = --it;
			Assert::AreEqual(Foo(20), *preDec);
			Assert::AreEqual(Foo(20), *it);

			--it;
			--preDec;
			Assert::AreEqual(Foo(data), *preDec);
			Assert::AreEqual(Foo(data), *it);

			Assert::ExpectException<std::runtime_error>([&it] { --it; });
		}

		TEST_METHOD(PostDecrement)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::Iterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { itEmpty--; });

			const int data = 10;
			list.PushBack(Foo(30));
			list.PushBack(Foo(20));
			list.PushBack(Foo(data));
			Assert::AreEqual(3_z, list.Size());

			auto it = list.end();
			Assert::AreEqual(Foo(data), *(--it));

			auto postDec = it--;
			Assert::AreEqual(Foo(data), *postDec);
			Assert::AreEqual(Foo(20), *it);

			it--;
			postDec--;
			Assert::AreEqual(Foo(20), *postDec);
			Assert::AreEqual(Foo(30), *it);

			Assert::ExpectException<std::runtime_error>([&it] { it--; });
		}

		TEST_METHOD(IteratorEquality)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			auto it = list.begin();
			auto itCopy(list.begin());

			Assert::IsTrue(it == itCopy);
			Assert::IsTrue(it <= itCopy);
			Assert::IsTrue(it >= itCopy);
			Assert::IsFalse(it != itCopy);
			Assert::IsFalse(it < itCopy);
			Assert::IsFalse(it > itCopy);

			itCopy++;
			Assert::IsFalse(it == itCopy);
			Assert::IsTrue(it <= itCopy);
			Assert::IsFalse(it >= itCopy);
			Assert::IsTrue(it != itCopy);
			Assert::IsTrue(it < itCopy);
			Assert::IsFalse(it > itCopy);

			Vector<Foo> listCopy(list);
			auto itCopy2 = listCopy.begin();
			Assert::IsFalse(it == itCopy2);
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it <= itCopy2; });
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it >= itCopy2; });
			Assert::IsTrue(it != itCopy2);
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it < itCopy2; });
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it > itCopy2; });


			Vector<Foo> listEmpty;
			auto itEmpty = listEmpty.begin();
			Assert::IsFalse(it == itEmpty);
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it <= itEmpty; });
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it >= itEmpty; });
			Assert::IsTrue(it != itEmpty);
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it < itEmpty; });
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it > itEmpty; });

			it++;
			itCopy--;
			Assert::IsFalse(it == itCopy);
			Assert::IsFalse(it <= itCopy);
			Assert::IsTrue(it >= itCopy);
			Assert::IsTrue(it != itCopy);
			Assert::IsFalse(it < itCopy);
			Assert::IsTrue(it > itCopy);
		}
		
		TEST_METHOD(IteratorArithmetic)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			list.PushBack(Foo(40));
			list.PushBack(Foo(50));
			list.PushBack(Foo(60));
			Assert::AreEqual(6_z, list.Size());

			auto it = list.begin();

			it = it + 2;
			Assert::AreEqual(Foo(30), *it);

			it = it + 3;
			Assert::AreEqual(Foo(60), *it);

			it = it - 2;
			Assert::AreEqual(Foo(40), *it);

			it = it - 3;
			Assert::AreEqual(Foo(data), *it);

			Assert::ExpectException<std::runtime_error>([&it] { it = it + 10; });
			Assert::ExpectException<std::runtime_error>([&it] { it = it - 10; });
		}

#pragma endregion Iterator Tests

#pragma region ConstIterator Tests

		TEST_METHOD(PreIncrementConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::ConstIterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { ++itEmpty; });

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			const Vector<Foo> listCopy(list);

			auto it = listCopy.begin();

			Assert::AreEqual(Foo(data), *it);

			auto preInc = ++it;
			Assert::AreEqual(Foo(20), *preInc);
			Assert::AreEqual(Foo(20), *it);

			++it;
			++preInc;
			Assert::AreEqual(Foo(30), *preInc);
			Assert::AreEqual(Foo(30), *it);

			++it;
			++preInc;
			Assert::ExpectException<std::runtime_error>([&preInc] { *preInc; });
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Assert::ExpectException<std::runtime_error>([&it] { ++it; });
		}

		TEST_METHOD(PostIncrementConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::ConstIterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { itEmpty++; });

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			const Vector<Foo> listCopy(list);

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

		TEST_METHOD(PreDecrementConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::ConstIterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { --itEmpty; });

			const int data = 10;
			list.PushBack(Foo(30));
			list.PushBack(Foo(20));
			list.PushBack(Foo(data));
			Assert::AreEqual(3_z, list.Size());

			const Vector<Foo> listCopy(list);

			auto it = listCopy.end();

			Assert::AreEqual(Foo(data), *(--it));

			auto preDec = --it;
			Assert::AreEqual(Foo(20), *preDec);
			Assert::AreEqual(Foo(20), *it);

			--it;
			--preDec;
			Assert::AreEqual(Foo(30), *preDec);
			Assert::AreEqual(Foo(30), *it);

			Assert::ExpectException<std::runtime_error>([&it] { --it; });
		}

		TEST_METHOD(PostDecrementConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Vector<Foo> listEmptyCopy(list);
			auto itEmpty = Vector<Foo>::ConstIterator();
			Assert::ExpectException<std::runtime_error>([&itEmpty] { itEmpty--; });

			const int data = 10;
			list.PushBack(Foo(30));
			list.PushBack(Foo(20));
			list.PushBack(Foo(data));
			Assert::AreEqual(3_z, list.Size());

			const Vector<Foo> listCopy(list);

			auto it = listCopy.end();
			Assert::AreEqual(Foo(data), *(--it));

			auto postDec = it--;
			Assert::AreEqual(Foo(data), *postDec);
			Assert::AreEqual(Foo(20), *it);

			it--;
			postDec--;
			Assert::AreEqual(Foo(20), *postDec);
			Assert::AreEqual(Foo(30), *it);

			Assert::ExpectException<std::runtime_error>([&it] { it--; });
		}

		TEST_METHOD(IteratorEqualityConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			const Vector<Foo> listConst(list);

			auto it = listConst.begin();
			auto itCopy(listConst.begin());

			Assert::IsTrue(it == itCopy);
			Assert::IsTrue(it <= itCopy);
			Assert::IsTrue(it >= itCopy);
			Assert::IsFalse(it != itCopy);
			Assert::IsFalse(it < itCopy);
			Assert::IsFalse(it > itCopy);

			itCopy++;
			Assert::IsFalse(it == itCopy);
			Assert::IsTrue(it <= itCopy);
			Assert::IsFalse(it >= itCopy);
			Assert::IsTrue(it != itCopy);
			Assert::IsTrue(it < itCopy);
			Assert::IsFalse(it > itCopy);

			const Vector<Foo> listCopy(listConst);
			auto itCopy2 = listCopy.begin();
			Assert::IsFalse(it == itCopy2);
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it <= itCopy2; });
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it >= itCopy2; });
			Assert::IsTrue(it != itCopy2);
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it < itCopy2; });
			Assert::ExpectException<std::runtime_error>([&it, &itCopy2] { it > itCopy2; });

			const Vector<Foo> listEmpty;
			auto itEmpty = listEmpty.begin();
			Assert::IsFalse(it == itEmpty);
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it <= itEmpty; });
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it >= itEmpty; });
			Assert::IsTrue(it != itEmpty);
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it < itEmpty; });
			Assert::ExpectException<std::runtime_error>([&it, &itEmpty] { it > itEmpty; });

			it++;
			itCopy--;
			Assert::IsFalse(it == itCopy);
			Assert::IsFalse(it <= itCopy);
			Assert::IsTrue(it >= itCopy);
			Assert::IsTrue(it != itCopy);
			Assert::IsFalse(it < itCopy);
			Assert::IsTrue(it > itCopy);
		}

		TEST_METHOD(IteratorArithmeticConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			list.PushBack(Foo(40));
			list.PushBack(Foo(50));
			list.PushBack(Foo(60));
			Assert::AreEqual(6_z, list.Size());

			const Vector<Foo> listConst(list);
			auto it = listConst.begin();

			it = it + 2;
			Assert::AreEqual(Foo(30), *it);

			it = it + 3;
			Assert::AreEqual(Foo(60), *it);

			it = it - 2;
			Assert::AreEqual(Foo(40), *it);

			it = it - 3;
			Assert::AreEqual(Foo(data), *it);

			Assert::ExpectException<std::runtime_error>([&it] { it = it + 10; });
			Assert::ExpectException<std::runtime_error>([&it] { it = it - 10; });
		}

#pragma endregion ConstIterator Tests

#pragma region Vector Tests
		TEST_METHOD(VectorConstructor)
		{
			Vector<Foo> list;
			size_t size = list.Size();
			size_t capacity = list.Capacity();
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(0_z, size);
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(0_z, capacity);
			Assert::AreEqual(0_z, list.Capacity());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });

			const Vector<Foo> fooList;
			size = fooList.Size();
			capacity = fooList.Capacity();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(0_z, size);
			Assert::AreEqual(0_z, fooList.Size());
			Assert::AreEqual(0_z, capacity);
			Assert::AreEqual(0_z, fooList.Capacity());
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Front(); });
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Back(); });

			Vector<Foo> ListIncrement(10, [](const size_t, const size_t) { return 100; });
			Assert::IsTrue(ListIncrement.IsEmpty());
			Assert::AreEqual(0_z, ListIncrement.Size());
			Assert::AreEqual(10_z, ListIncrement.Capacity());
			Assert::ExpectException<std::runtime_error>([&ListIncrement] { ListIncrement.Front(); });
			Assert::ExpectException<std::runtime_error>([&ListIncrement] { ListIncrement.Back(); });

			ListIncrement.Resize(10);
			Assert::IsFalse(ListIncrement.IsEmpty());
			Assert::AreEqual(10_z, ListIncrement.Size());
			Assert::AreEqual(10_z, ListIncrement.Capacity());

			ListIncrement.PushBack(Foo(10));
			Assert::AreEqual(11_z, ListIncrement.Size());
			Assert::AreEqual(110_z, ListIncrement.Capacity());
		}

		TEST_METHOD(DeepCopy)
		{
			{
				Vector<Foo> list;
				list.PushBack(Foo(10));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));

				Vector<Foo> listCopy(list);

				auto it = list.begin();
				auto itCopy = listCopy.begin();

				while (it != list.end())
				{
					Assert::AreEqual(*it, *itCopy);
					++it;
					++itCopy;
				}
			}

			{
				Vector<Foo> list;
				list.PushBack(Foo(10));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Vector<Foo> listCopy;
				listCopy = list;

				auto it = list.begin();
				auto itCopy = listCopy.begin();

				while (it != list.end())
				{
					Assert::AreEqual(*it, *itCopy);
					++it;
					++itCopy;
				}
			}

			{
				Vector<Foo> list;
				list.PushBack(Foo(10));
				list.PushBack(Foo(10));
				list.PushBack(Foo(10));
				Vector<Foo> listCopy;
				listCopy = list;

				Assert::AreNotSame(list.Front(), listCopy.Back());
				Assert::AreEqual(list.Front(), listCopy.Back());
			}
		}

		TEST_METHOD(ShallowCopy)
		{
			{
				Vector<Foo> list;
				list.PushBack(Foo(10));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));

				Vector<Foo> listCopy(std::move(list));

				Assert::IsTrue(list.IsEmpty());
				Assert::IsFalse(listCopy.IsEmpty());
			}

			{
				Vector<Foo> list;
				list.PushBack(Foo(10));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));

				Vector<Foo> listCopy;
				listCopy = std::move(list);

				Assert::IsTrue(list.IsEmpty());
				Assert::IsFalse(listCopy.IsEmpty());
			}
		}

		TEST_METHOD(IsEmpty)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			const int data = 10;
			list.PushBack(Foo(data));
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			Assert::AreEqual(1_z, list.Size());
		}

		TEST_METHOD(Capacity)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Capacity());

			const int data = 10;
			list.PushBack(Foo(data));
			Assert::AreEqual(1_z, list.Capacity());
		}

		TEST_METHOD(Clear)
		{
			Vector<Foo> list;
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Front)
		{
			Vector<Foo> list;
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
				const Vector<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			}

			{
				Vector<Foo> list;
				const int data = 99;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));

				const Vector<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
			}
		}

		TEST_METHOD(Back)
		{
			Vector<Foo> list;
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
				const Vector<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
			}

			{
				Vector<Foo> list;
				const int data = 99;
				list.PushBack(Foo(30));
				list.PushBack(Foo(20));
				list.PushBack(Foo(data));

				const Vector<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Back());
			}
		}

		TEST_METHOD(PushBack)
		{
			Vector<Foo> list;
			Foo data = Foo(10);
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(0_z, list.Capacity());
			Assert::IsTrue(list.IsEmpty());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });

			list.PushBack(data);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(data, list.Front());
			Assert::AreEqual(data, list.Back());
			Assert::AreEqual(1_z, list.Size());
			Assert::AreEqual(1_z, list.Capacity());

			auto capacity = list.Capacity();
			list.PushBack(Foo(20));
			Assert::AreEqual(data, list.Front());
			Assert::AreEqual(Foo(20), list.Back());
			Assert::AreEqual(2_z, list.Size());
			Assert::IsTrue(capacity < list.Capacity());
		}

		TEST_METHOD(PopBack)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			auto capacity = list.Capacity();
			list.PopBack();
			Assert::AreEqual(2_z, list.Size());
			Assert::AreEqual(capacity, list.Capacity());
			Assert::AreEqual(Foo(data), list.Front());
			Assert::AreEqual(Foo(20), list.Back());

			list.PopBack();
			Assert::AreEqual(1_z, list.Size());
			Assert::AreEqual(capacity, list.Capacity());
			Assert::AreEqual(list.Back(), list.Front());

			list.PopBack();
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(capacity, list.Capacity());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
		}
		
		TEST_METHOD(Begin)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			Assert::AreEqual(Foo(data), *(list.begin()));
		}

		TEST_METHOD(BeginConst)
		{
			{
				const Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.begin()); });
			}

			{
				Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(3_z, list.Size());

				const Vector<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::AreEqual(Foo(data), *(listCopy.begin()));
			}
		}

		TEST_METHOD(CBegin)
		{
			{
				const Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cbegin()); });
			}

			{
				Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(3_z, list.Size());

				const Vector<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::AreEqual(Foo(data), *(listCopy.cbegin()));
			}
		}

		TEST_METHOD(End)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
		}

		TEST_METHOD(EndConst)
		{
			{
				const Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			}

			{
				Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(3_z, list.Size());

				const Vector<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			}
		}

		TEST_METHOD(CEnd)
		{
			{
				const Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			}

			{
				Vector<Foo> list;
				Assert::AreEqual(0_z, list.Size());

				const int data = 10;
				list.PushBack(Foo(data));
				list.PushBack(Foo(20));
				list.PushBack(Foo(30));
				Assert::AreEqual(3_z, list.Size());

				const Vector<Foo> listCopy(list);

				Assert::AreEqual(Foo(data), listCopy.Front());
				Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			}
		}
		
		TEST_METHOD(Reserve)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(0_z, list.Capacity());

			list.Reserve(10);			
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(10_z, list.Capacity());

			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());
			Assert::AreEqual(10_z, list.Capacity());

			list.Reserve(5);
			Assert::AreEqual(3_z, list.Size());
			Assert::AreEqual(10_z, list.Capacity());
		}

		TEST_METHOD(Resize)
		{
			Vector<Foo> list;
			Assert::AreEqual(0_z, list.Size());
			Assert::AreEqual(0_z, list.Capacity());

			list.Resize(5);
			Assert::AreEqual(5_z, list.Size());
			Assert::AreEqual(5_z, list.Capacity());


			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(8_z, list.Size());
			Assert::AreEqual(10_z, list.Capacity());

			list.Resize(10);
			Assert::AreEqual(10_z, list.Size());
			Assert::AreEqual(10_z, list.Capacity());

			list.Resize(1);
			Assert::AreEqual(1_z, list.Size());
			Assert::AreEqual(10_z, list.Capacity());
		}

		TEST_METHOD(BracketIndex)
		{
			Vector<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { list[3]; });
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			Assert::AreEqual(Foo(10), list[0]);
			Assert::AreEqual(Foo(20), list[1]);
			Assert::AreEqual(Foo(30), list[2]);

			Assert::ExpectException<std::runtime_error>([&list] { list[3]; });
		}

		TEST_METHOD(BracketIndexConst)
		{
			Vector<Foo> list;
			const Vector<Foo> listConstEmpty = list;
			Assert::ExpectException<std::runtime_error>([&listConstEmpty] { listConstEmpty[3]; });
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			const Vector<Foo> listConst = list;
			Assert::AreEqual(3_z, listConst.Size());

			Assert::AreEqual(Foo(10), listConst[0]);
			Assert::AreEqual(Foo(20), listConst[1]);
			Assert::AreEqual(Foo(30), listConst[2]);

			Assert::ExpectException<std::runtime_error>([&listConst] { listConst[3]; });
		}

		TEST_METHOD(At)
		{
			Vector<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { list.At(3); });
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			Assert::AreEqual(Foo(10), list.At(0));
			Assert::AreEqual(Foo(20), list.At(1));
			Assert::AreEqual(Foo(30), list.At(2));

			Assert::ExpectException<std::runtime_error>([&list] { list.At(3); });
		}

		TEST_METHOD(AtConst)
		{
			Vector<Foo> list;
			const Vector<Foo> listConstEmpty = list;
			Assert::ExpectException<std::runtime_error>([&listConstEmpty] { listConstEmpty.At(3); });
			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));

			const Vector<Foo> listConst = list;
			Assert::AreEqual(3_z, listConst.Size());

			Assert::AreEqual(Foo(10), listConst.At(0));
			Assert::AreEqual(Foo(20), listConst.At(1));
			Assert::AreEqual(Foo(30), listConst.At(2));

			Assert::ExpectException<std::runtime_error>([&listConst] { listConst.At(3); });
		}

		TEST_METHOD(Dereference)
		{
			Vector<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { *(list.begin()); });
			Assert::ExpectException<std::runtime_error>([&list] { *(list.cbegin()); });
			Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			Assert::ExpectException<std::runtime_error>([&list] { (list.begin())->Data(); });
			Assert::ExpectException<std::runtime_error>([&list] { (list.cbegin())->Data(); });
			Assert::ExpectException<std::runtime_error>([&list] { (list.end())->Data(); });
			Assert::ExpectException<std::runtime_error>([&list] { (list.cend())->Data(); });
			Assert::AreEqual(0_z, list.Size());

			list.PushBack(Foo(10));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(3_z, list.Size());

			Assert::ExpectException<std::runtime_error>([&list] { *(list.end()); });
			Assert::ExpectException<std::runtime_error>([&list] { *(list.cend()); });
			Assert::ExpectException<std::runtime_error>([&list] { (list.end())->Data(); });
			Assert::ExpectException<std::runtime_error>([&list] { (list.cend())->Data(); });

			Assert::AreEqual(10, list.begin()->Data());
			Assert::AreEqual(10, list.cbegin()->Data());
		}

		TEST_METHOD(Find)
		{
			Vector<Foo> list;
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

			it = list.Find(Foo(40));
			Assert::ExpectException<std::runtime_error>([&it] { *it; });
		}

		TEST_METHOD(FindConst)
		{
			Vector<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			Assert::AreEqual(std::size_t(3), list.Size());

			const Vector<Foo> listConst(list);

			auto it = listConst.Find(Foo(data));
			Assert::AreEqual(Foo(data), *it);

			it = listConst.Find(Foo(20));
			Assert::AreEqual(Foo(20), *it);

			it = listConst.Find(Foo(30));
			Assert::AreEqual(Foo(30), *it);

			it = list.Find(Foo(40));
			Assert::ExpectException<std::runtime_error>([&it] { *it; });
		}

		TEST_METHOD(Remove)
		{
			Vector<Foo> list;
			Assert::AreEqual(std::size_t(0), list.Size());

			const int data = 10;
			list.PushBack(Foo(data));
			list.PushBack(Foo(20));
			list.PushBack(Foo(30));
			list.PushBack(Foo(40));
			list.PushBack(Foo(50));
			list.PushBack(Foo(60));
			Assert::AreEqual(6_z, list.Size());

			size_t capacity = list.Capacity();

			list.Remove(Foo(30));
			Assert::AreEqual(5_z, list.Size());
			Assert::AreEqual(capacity, list.Capacity());

			auto it = list.begin();
			Assert::AreEqual(Foo(data), *(it + 0));
			Assert::AreEqual(Foo(20), *(it + 1));
			Assert::AreEqual(Foo(40), *(it + 2));
			Assert::AreEqual(Foo(50), *(it + 3));
			Assert::AreEqual(Foo(60), *(it + 4));
			Assert::ExpectException<std::runtime_error>([&it] { *(it + 5); });

			auto itFirst = it + 1;
			auto itLast = it + 3;
			list.Remove(itFirst, itLast);
			Assert::AreEqual(2_z, list.Size());
			Assert::AreEqual(capacity, list.Capacity());

			Assert::AreEqual(Foo(data), *(it + 0));
			Assert::AreEqual(Foo(60), *(it + 1));
			Assert::ExpectException<std::runtime_error>([&it] { *(it + 2); });

			list.Remove(it);
			Assert::AreEqual(1_z, list.Size());
			Assert::AreEqual(capacity, list.Capacity());

			Assert::AreEqual(Foo(60), *(it + 0));
			Assert::ExpectException<std::runtime_error>([&it] { *(it + 1); });

			Vector<Foo> otherList;
			otherList.PushBack(Foo(11));
			otherList.PushBack(Foo(21));
			otherList.PushBack(Foo(31));
			otherList.PushBack(Foo(41));
			otherList.PushBack(Foo(51));
			otherList.PushBack(Foo(61));

			auto otherIt = otherList.begin();

			auto otherItFirst = otherIt + 1;
			auto otherItLast = otherIt + 3;

			Assert::ExpectException<std::runtime_error>([&otherList, &otherItFirst, &otherItLast] { otherList.Remove(otherItLast, otherItFirst); });
			Assert::ExpectException<std::runtime_error>([&list, &otherIt] { list.Remove(otherIt); });
			Assert::ExpectException<std::runtime_error>([&list, &otherIt, &it] { list.Remove(it, otherIt); });

		}

#pragma endregion Vector Tests


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTests::sStartMemState;
}