#pragma once

#include "RTTI.h"
#include "Factory.h"

namespace Library
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& rhs) const noexcept;
		bool operator!=(const Foo& rhs) const noexcept;

		int Data() const;
		void SetData(int data);

	private:
		int* mData;
	};

	ConcreteFactory(Foo, RTTI)
}

