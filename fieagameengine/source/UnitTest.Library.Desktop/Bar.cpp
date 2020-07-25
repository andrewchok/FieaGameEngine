#include "pch.h"
#include "Bar.h"

namespace Library
{
	RTTI_DEFINITIONS(Bar)

	Bar::Bar(int data) :
		mData(new int(data))
	{
	}

	Bar::Bar(const Bar& rhs) :
		mData(new int(*rhs.mData))
	{
	}

	Bar::Bar(Bar&& rhs) noexcept :
		mData(rhs.mData)
	{
		rhs.mData = nullptr;
	}

	Bar& Bar::operator=(const Bar& rhs)
	{
		if (this != &rhs)
		{
			*mData = *rhs.mData;
		}

		return *this;
	}

	Bar& Bar::operator=(Bar&& rhs) noexcept
	{
		if (this != &rhs)
		{
			delete mData;
			mData = rhs.mData;
			rhs.mData = nullptr;
		}

		return *this;
	}

	Bar::~Bar()
	{
		delete mData;
	}

	bool Bar::operator==(const Bar& rhs) const noexcept
	{
		return *mData == *rhs.mData;
	}

	bool Bar::operator!=(const Bar& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	int Bar::Data() const
	{
		return *mData;
	}

	void Bar::SetData(int data)
	{
		*mData = data;
	}
}