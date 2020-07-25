#pragma once
#include "RTTI.h"

namespace Library
{
	class Bar : public RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI)

	public:
		explicit Bar(int data = 0);
		Bar(const Bar& rhs);
		Bar(Bar&& rhs) noexcept;
		Bar& operator=(const Bar& rhs);
		Bar& operator=(Bar&& rhs) noexcept;
		virtual ~Bar();

		bool operator==(const Bar& rhs) const noexcept;
		bool operator!=(const Bar& rhs) const noexcept;

		int Data() const;
		void SetData(int data);

	private:
		int* mData;
	};

}
