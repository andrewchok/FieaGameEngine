#pragma once
#include "Datum.h"

namespace Library
{
	struct Signature final
	{
		std::string mName;
		Datum::DatumTypes mType;
		std::size_t mSize;
		std::size_t mOffset;
	};
}