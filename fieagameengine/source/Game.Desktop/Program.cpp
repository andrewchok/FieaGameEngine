#include "pch.h"
#include "SList.h"

using namespace Library;
int main()
{

	SList<int> list;
	list.PushBack(10);

	{
		// copy constructor
		SList<int> anotherList = list;
		//SList<int> anotherList(list);
	}

	{
		// move constructor
		SList<int> anotherList = std::move(list);
		//SList<int> anotherList(std::move(list));
	}

	{
		SList<int> anotherList{ std::move(list) };
		anotherList = std::move(list);
	}

	return 0;
}
