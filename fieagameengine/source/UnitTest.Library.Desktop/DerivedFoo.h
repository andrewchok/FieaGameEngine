#pragma once

#include "Foo.h"
#include "Factory.h"

namespace Library
{
	class DerivedFoo final : public Foo
	{
	};

	ConcreteFactory(DerivedFoo, Foo)
}

