#include "pch.h"
#include "Foo.h"

namespace Library
{
	RTTI_DEFINITIONS(Foo)

	Foo::Foo(int data) :
		mData(new int(data))
	{
	}

	Foo::Foo(const Foo& rhs) :
		mData(new int(*rhs.mData))
	{
	}

	Foo::Foo(Foo&& rhs) noexcept :
		mData(rhs.mData)
	{
		rhs.mData = nullptr;
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)
		{
			*mData = *rhs.mData;
		}

		return *this;
	}

	Foo& Foo::operator=(Foo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			delete mData;
			mData = rhs.mData;
			rhs.mData = nullptr;
		}

		return *this;
	}

	Foo::~Foo()
	{
		delete mData;
	}

	bool Foo::operator==(const Foo& rhs) const noexcept
	{
		return *mData == *rhs.mData;
	}

	bool Foo::operator!=(const Foo& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	int Foo::Data() const
	{
		return *mData;
	}

	void Foo::SetData(int data)
	{
		*mData = data;
	}
}