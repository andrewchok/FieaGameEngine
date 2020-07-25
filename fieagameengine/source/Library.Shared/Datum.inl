#include "Datum.h"

namespace Library
{

#pragma region Datum


	template<typename T>
	inline std::tuple<size_t, bool> Datum::FindHelper(const T& value)
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}
		TypeCheck<T>();

		for (size_t i = 0; i < mSize; ++i)
		{
			if (value == DataPointer<T>()[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}


	#pragma region Storage:


	inline void Datum::Resize(const size_t size)
	{
		if (mSize != size && !mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot alter size nor capacity.");
		}
		if (mDataType == DatumTypes::Unknown)
		{
			throw std::runtime_error("Data type not set.");
		}

		if (size < mSize)
		{
			size_t removingElements = mSize - size;
			for (size_t i = 0; i < removingElements; ++i)
			{
				PopBack();
			}
		}
		else if (size > mSize)
		{
			if (size > mCapacity)
			{
				Reserve(size);
			}

			size_t appendingElements = size - mSize;
			for (size_t i = 0; i < appendingElements; ++i)
			{
				// default construct elements into vector
				if (mDataType == DatumTypes::Integer) new (mData.i + mSize) std::int32_t();
				else if (mDataType == DatumTypes::Float) new (mData.f + mSize) std::float_t();
				else if (mDataType == DatumTypes::Vector) new (mData.v + mSize) glm::vec4();
				else if (mDataType == DatumTypes::Matrix) new (mData.m + mSize) glm::mat4();
				else if (mDataType == DatumTypes::Table) mData.t[mSize] = nullptr;
				else if (mDataType == DatumTypes::String) new (mData.s + mSize) std::string();
				else if (mDataType == DatumTypes::Pointer) mData.p[mSize] = nullptr;
				else throw std::runtime_error("Type not supported.");
				++mSize;
			}
		}
		else return;

		mCapacity = size;

		void* data = (realloc(mData.voidPtr, mCapacity * DatumSizes[static_cast<size_t>(mDataType)]));
		assert(data != nullptr);
		mData.voidPtr = data;
	}

	#pragma endregion Storage:

	#pragma region Element Access:

	template<typename T>
	inline T& Datum::Get(size_t index)
	{
		return At<T>(index);
	}

	template<>
	Scope*& Datum::Get<Scope*>(size_t index)
	{
		if (mDataType != DatumTypes::Table)
		{
			throw std::runtime_error("type mismatch");
		}
		if (mSize <= index)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return mData.t[index];
	}

	template<typename T>
	inline const T& Datum::Get(size_t index) const
	{
		return At<T>(index);
	}

	template<typename T>
	inline T& Datum::Front()
	{
		return At<T>();
	}

	template<typename T>
	inline const T& Datum::Front() const
	{
		return At<T>();
	}

	template<typename T>
	inline T& Datum::Back()
	{
		return At<T>(mSize - 1);
	}

	template<typename T>
	inline const T& Datum::Back() const
	{
		return At<T>(mSize - 1);
	}

	#pragma endregion Element Access:

	#pragma region Modifiers:
	
	template<typename T>
	inline void Datum::PushBackHelper(const T& value)
	{
		if (!mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot change size nor capacity.");
		}
		InitialSetType<T>();
		TypeCheck<T>();

		if (mSize == mCapacity)
		{
			Reserve( mCapacity + std::max( mIncrementFunctor( mSize, mCapacity ), size_t(1) ) );
		}

		if constexpr (std::is_same<T, std::int32_t>::value) new (mData.i + mSize) T(value);
		else if constexpr (std::is_same<T, std::float_t>::value) new (mData.f + mSize) T(value);
		else if constexpr (std::is_same<T, glm::vec4>::value) new (mData.v + mSize) T(value);
		else if constexpr (std::is_same<T, glm::mat4>::value) new (mData.m + mSize) T(value);
		else if constexpr (std::is_same<T, Scope>::value) new (mData.t + mSize) T(value);
		else if constexpr (std::is_same<T, std::string>::value) new (mData.s + mSize) T(value);
		else if constexpr (std::is_same<T, RTTIPtr>::value) new (mData.p + mSize) T(value);
		else throw std::runtime_error("Type not supported.");
		++mSize;
	}

	template<typename T>
	inline void Datum::Remove(const T& value)
	{
		if (!mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot change size nor capacity.");
		}
		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}
		TypeCheck<T>();

		auto [ index, isFound ] = Find(value);
		if (isFound) RemoveAt<T>(index);
	}

	template<typename T>
	inline void Datum::RemoveAt(size_t index)
	{
		if (!mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot change size nor capacity.");
		}
		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}
		TypeCheck<T>();

		if (index < mSize)
		{
			if (mDataType == DatumTypes::String)
			{
				At<std::string>(index).std::string::~string();				
			}

			if (mSize != 1 && index + 1 != mSize)
			{
				memmove(&(At<T>(index)), &(At<T>(index + 1)), (mSize - index) * DatumSizes[static_cast<size_t>(mDataType)]);
			}

			--mSize;
		}
	}

	template<typename T>
	inline const std::string Datum::ToString(size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Mismatch data types.");
		}

		if (std::is_same<T, std::int32_t>::value) return std::to_string(At<std::int32_t>(index));
		else if (std::is_same<T, std::float_t>::value)
		{
			std::ostringstream ss;
			ss << At<std::float_t>(index);
			std::string s(ss.str());
			return s;
		}
		else if (std::is_same<T, glm::vec4>::value) return glm::to_string(At<glm::vec4>(index));
		else if (std::is_same<T, glm::mat4>::value) return glm::to_string(At<glm::mat4>(index));
		else if (std::is_same<T, std::string>::value) return At<std::string>(index);
		else if (std::is_same<T, RTTIPtr>::value) throw std::runtime_error("RTTI pointer cannot be made to string.");

		throw std::runtime_error("Data Type is uninitialized or not supported.");
	}

	#pragma endregion Modifiers:

#pragma endregion Datum

#pragma region Helpers:

	template<typename T>
	inline const bool Datum::TypeCheck() const
	{
		bool typeMatch = false;

		if (mDataType == DatumTypes::Unknown) typeMatch = false;
		else if constexpr (std::is_same<T, std::int32_t>::value) { if (mDataType == DatumTypes::Integer) typeMatch = true; }
		else if constexpr (std::is_same<T, std::float_t>::value) { if (mDataType == DatumTypes::Float) typeMatch = true; }
		else if constexpr (std::is_same<T, glm::vec4>::value) { if (mDataType == DatumTypes::Vector) typeMatch = true; }
		else if constexpr (std::is_same<T, glm::mat4>::value) { if (mDataType == DatumTypes::Matrix) typeMatch = true; }
		else if constexpr (std::is_same<T, std::string>::value) { if (mDataType == DatumTypes::String) typeMatch = true; }
		else if constexpr (std::is_same<T, RTTIPtr>::value) { if (mDataType == DatumTypes::Pointer) typeMatch = true; }
		else if constexpr (std::is_same<T, ScopePtr>::value) { if (mDataType == DatumTypes::Table) typeMatch = true; }
		else throw std::runtime_error("Type not supported.");

		return typeMatch;
	}

	template<typename T>
	inline void Datum::InitialSetType()
	{
		if (mDataType == DatumTypes::Unknown)
		{
			if (std::is_same<T, std::int32_t>::value) SetType(DatumTypes::Integer);
			else if (std::is_same<T, std::float_t>::value) SetType(DatumTypes::Float);
			else if (std::is_same<T, glm::vec4>::value) SetType(DatumTypes::Vector);
			else if (std::is_same<T, glm::mat4>::value) SetType(DatumTypes::Matrix);
			else if (std::is_same<T, ScopePtr>::value) SetType(DatumTypes::Table);
			else if (std::is_same<T, std::string>::value) SetType(DatumTypes::String);
			else if (std::is_same<T, RTTIPtr>::value) SetType(DatumTypes::Pointer);
			else throw std::runtime_error("Type not supported.");
		}
	}

	template<typename T>
	inline T& Datum::At(size_t index)
	{
		if (mDataType == DatumTypes::Unknown)
		{
			throw std::runtime_error("No Type is set yet");
		}

		TypeCheck<T>();

		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if constexpr (std::is_same<T, std::int32_t>::value) return mData.i[index];
		else if constexpr (std::is_same<T, std::float_t>::value) return mData.f[index];
		else if constexpr (std::is_same<T, glm::vec4>::value) return mData.v[index];
		else if constexpr (std::is_same<T, glm::mat4>::value) return mData.m[index];
		else if constexpr (std::is_same<T, ScopePtr>::value) return mData.t[index];
		else if constexpr (std::is_same<T, std::string>::value) return mData.s[index];
		else if constexpr (std::is_same<T, RTTIPtr>::value) return mData.p[index];
		else throw std::runtime_error("Type not supported.");
	}

	template<typename T>
	inline const T& Datum::At(size_t index) const
	{
		if (mDataType == DatumTypes::Unknown)
		{
			throw std::runtime_error("No Type is set yet");
		}

		TypeCheck<T>();

		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if constexpr (std::is_same<T, std::int32_t>::value) return mData.i[index];
		else if constexpr (std::is_same<T, std::float_t>::value) return mData.f[index];
		else if constexpr (std::is_same<T, glm::vec4>::value) return mData.v[index];
		else if constexpr (std::is_same<T, glm::mat4>::value) return mData.m[index];
		else if constexpr (std::is_same<T, ScopePtr>::value) return mData.t[index];
		else if constexpr (std::is_same<T, std::string>::value) return mData.s[index];
		else if constexpr (std::is_same<T, RTTIPtr>::value) return mData.p[index];
		else throw std::runtime_error("Invalid Data Type");
	}

	template<typename T>
	inline T*& Datum::DataPointer()
	{
		if constexpr (std::is_same<T, std::int32_t>::value) return mData.i;
		else if constexpr (std::is_same < T, std::float_t> ::value) return mData.f;
		else if constexpr (std::is_same<T, glm::vec4>::value) return mData.v;
		else if constexpr (std::is_same<T, glm::mat4>::value) return mData.m;
		else if constexpr (std::is_same<T, std::string>::value) return mData.s;
		else throw std::runtime_error("Invalid Data Type");
	}

	template<>
	inline Scope**& Datum::DataPointer<Scope*>()
	{
		return mData.t;
	}

	template<>
	inline RTTI**& Datum::DataPointer<RTTI*>()
	{
		return mData.p;
	}


#pragma endregion Helpers:
}