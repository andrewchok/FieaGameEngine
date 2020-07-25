#include "pch.h"
#include "Datum.h"
#include "Scope.h"

namespace Library
{
	const size_t Datum::DatumSizes[static_cast<size_t>(DatumTypes::Max)] =
	{
		size_t(0),
		sizeof(std::int32_t),
		sizeof(std::float_t),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(ScopePtr),
		sizeof(std::string),
		sizeof(RTTIPtr),
		size_t(0)
	};

	const Hashmap<const std::string, Datum::DatumTypes> Datum::StringToDatumTypesMap =
	{
		{ "integer", Datum::DatumTypes::Integer },
		{ "float", Datum::DatumTypes::Float },
		{ "vector", Datum::DatumTypes::Vector },
		{ "matrix", Datum::DatumTypes::Matrix },
		{ "table", Datum::DatumTypes::Table },
		{ "string", Datum::DatumTypes::String }
	};

	const Hashmap<Datum::DatumTypes, const std::string> Datum::DatumTypesToStringMap =
	{
		{ Datum::DatumTypes::Integer, "integer" },
		{ Datum::DatumTypes::Float, "float" },
		{ Datum::DatumTypes::Vector, "vector" },
		{ Datum::DatumTypes::Matrix, "matrix" },
		{ Datum::DatumTypes::Table, "table" },
		{ Datum::DatumTypes::String, "string" }
	};
	#pragma region Constructors, Assignments & Destructor:
	
	Datum::Datum(DatumTypes dataType, bool isInternalStorage, size_t capacity, IncrementFunctor incrementFunctor) :
		mDataType(dataType), mIsInternalStorage(isInternalStorage), mIncrementFunctor(incrementFunctor)
	{
		if(dataType != DatumTypes::Unknown && mIsInternalStorage && capacity > 0) Reserve(capacity);
	}

	Datum::Datum(const Datum& rhs)
	{		
		operator=(rhs);
	}

	Datum::Datum(Datum&& rhs) noexcept :
		mCapacity(rhs.mCapacity), mSize(rhs.mSize), mData(rhs.mData),
		mIsInternalStorage(rhs.mIsInternalStorage), mDataType(rhs.mDataType),
		mIncrementFunctor(std::move(rhs.mIncrementFunctor))
	{
		rhs.mCapacity = 0;
		rhs.mSize = 0;
		rhs.mDataType = DatumTypes::Unknown;
		rhs.mIsInternalStorage = true;
		rhs.mData.voidPtr = nullptr;
		rhs.mIncrementFunctor = DefaultIncrement<DatumValue>();
	}

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			if (mIsInternalStorage) Clear();

			mDataType = rhs.mDataType;
			mIsInternalStorage = rhs.mIsInternalStorage;

			if (mIsInternalStorage)
			{
				Reserve(rhs.mCapacity);
				for (size_t i = 0; i < rhs.mSize; ++i)
				{
					switch (mDataType)
					{
					case DatumTypes::Integer:
					{
						std::int32_t temp = rhs.At<int32_t>(i);
						PushBack(temp);	
					}
						break;

					case DatumTypes::Float:
					{
						std::float_t temp = rhs.At<std::float_t>(i);
						PushBack(temp);
					}
						break;

					case DatumTypes::Vector:
					{
						glm::vec4 temp = rhs.At<glm::vec4>(i);
						PushBack(temp);
					}
						break;

					case DatumTypes::Matrix:
					{
						glm::mat4 temp = rhs.At<glm::mat4>(i);
						PushBack(temp);
					}
						break;

					case DatumTypes::String:
					{
						std::string temp = rhs.At<std::string>(i);
						PushBack(temp);
					}
						break;

					case DatumTypes::Table:
					{
						ScopePtr temp = rhs.At<ScopePtr>(i);
						PushBack(temp);
					}
					break;

					case DatumTypes::Pointer:
					{
						RTTIPtr temp = rhs.At<RTTIPtr>(i);
						PushBack(temp);
					}
						break;

					default:
						break;
					}
				}				
			}
			else
			{
				mData = rhs.mData;
				mCapacity = rhs.mCapacity;
				mSize = rhs.mSize;
			}
		}
		return *this;
	}

	Datum& Datum::operator=(std::int32_t rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(std::float_t rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(glm::vec4 rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(glm::mat4 rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(RTTIPtr& rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(Scope* rhs)
	{
		Set(rhs);
		Resize(1);
		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			if (mIsInternalStorage && mCapacity > 0)
			{
				if (mDataType == DatumTypes::String)
				{
					free(mData.s);
				}
				else
				{
					free(mData.voidPtr);
				}
			}

			mCapacity = rhs.mCapacity;
			mSize = rhs.mSize;
			mDataType = rhs.mDataType;
			mIsInternalStorage = rhs.mIsInternalStorage;
			mData = rhs.mData;
			mIncrementFunctor = rhs.mIncrementFunctor;

			rhs.mCapacity = 0;
			rhs.mSize = 0;
			rhs.mDataType = DatumTypes::Unknown;
			rhs.mIsInternalStorage = true;
			rhs.mData.voidPtr = nullptr;
			rhs.mIncrementFunctor = DefaultIncrement<DatumValue>();
		}

		return *this;
	}

	Datum::~Datum()
	{
		Clear();

		if (mIsInternalStorage && mCapacity > 0)
		{
			if (mDataType == DatumTypes::String)
			{
				free(mData.s);
			}
			else
			{
				free(mData.voidPtr);
			}
			mSize = 0;
			mCapacity = 0;
			mDataType = DatumTypes::Unknown;
			mIsInternalStorage = true;
		}
	}


	#pragma endregion Constructors, Assignments & Destructor:

	#pragma region Storage:

	Datum::DatumTypes Datum::Type() const
	{
		return mDataType;
	}

	const bool Datum::IsExternal() const
	{
		return !mIsInternalStorage;
	}

	bool Datum::IsEmpty() const
	{
		return mSize == 0;
	}

	size_t Datum::Size() const
	{
		return mSize;
	}

	size_t Datum::Capacity() const
	{
		return mCapacity;
	}

	void Datum::Reserve(const size_t capacity)
	{
		if (!mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot change size nor capacity.");
		}

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::runtime_error("No Data Type set, cannot reserve any memory.");
		}

		if (capacity > mCapacity)
		{
			mCapacity = capacity;

			void* data = (realloc(mData.voidPtr, mCapacity * DatumSizes[static_cast<size_t>(mDataType)]));
			assert(data != nullptr);
			mData.voidPtr = data;
		}
	}

	#pragma endregion Storage:

	#pragma region Element Access:

	Scope& Datum::operator[](std::size_t index)
	{
		return *Get<ScopePtr>(index);
	}

	const Scope& Datum::operator[](std::size_t index) const
	{
		return *Get<ScopePtr>(index);
	}

	#pragma region Find
	std::tuple<size_t, bool> Datum::Find(const std::int32_t& value)
	{
		return FindHelper<std::int32_t>(value);
	}

	std::tuple<size_t, bool> Datum::Find(const std::float_t& value)
	{
		return FindHelper<std::float_t>(value);
	}

	std::tuple<size_t, bool> Datum::Find(const glm::vec4& value)
	{
		return FindHelper<glm::vec4>(value);
	}

	std::tuple<size_t, bool> Datum::Find(const glm::mat4& value)
	{
		return FindHelper<glm::mat4>(value);
	}

	std::tuple<size_t, bool> Datum::Find(const std::string& value)
	{
		return FindHelper<std::string>(value);
	}

	std::tuple<size_t, bool> Datum::Find(const RTTI& value)
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (&value == mData.p[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	std::tuple<size_t, bool> Datum::Find(const Scope& value)
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (&value == mData.t[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const std::int32_t& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (value == mData.i[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const std::float_t& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (value == mData.f[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const glm::vec4& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (value == mData.v[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const glm::mat4& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (value == mData.m[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const std::string& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (value == mData.s[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const RTTI& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (&value == mData.p[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	const std::tuple<size_t, bool> Datum::Find(const Scope& value) const
	{
		bool isFound = false;
		size_t index = 0;

		if (mDataType == DatumTypes::Unknown)
		{
			throw std::exception("Datum Type not set.");
		}

		if (mDataType != DatumTypes::Pointer)
		{
			throw std::exception("Wrong Datum Type.");
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			if (&value == mData.t[i])
			{
				isFound = true;
				index = i;
				break;
			}
		}

		return { isFound, mSize };
	}
	#pragma endregion Find


	#pragma endregion Element Access:

	#pragma region Modifiers:

	void Datum::Clear()
	{
		if (mIsInternalStorage && mCapacity > 0)
		{
			if (mDataType == DatumTypes::String)
			{
				for (std::size_t i = 0; i < mSize; ++i)
				{
					mData.s[i].std::string::~string();
				}
			}
			mSize = 0;
		}
	}

	const bool Datum::SetType(const DatumTypes dataType)
	{
		bool wasTypeSet = false;

		if (mDataType == DatumTypes::Unknown || mDataType == dataType)
		{
			mDataType = dataType;
			wasTypeSet = true;
		}

		return wasTypeSet;
	}

	#pragma region Set Storage
	void Datum::SetStorage(const DatumTypes& dataType, std::size_t size)
	{
		if (mDataType != DatumTypes::Unknown && dataType != dataType)
		{
			throw std::runtime_error("Datum types are different");
		}

		if (mIsInternalStorage)
		{
			Clear();
		}

		mDataType = dataType;
		if (mDataType != DatumTypes::Table)
		{
			mIsInternalStorage = false;
		}
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(std::int32_t* dataArr, std::size_t size)
	{
		SetStorage(DatumTypes::Integer, size);
		mData.i = dataArr;
	}

	void Datum::SetStorage(std::float_t* dataArr, std::size_t size)
	{
		SetStorage(DatumTypes::Float, size);
		mData.f = dataArr;
	}

	void Datum::SetStorage(glm::vec4* dataArr, std::size_t size)
	{
		SetStorage(DatumTypes::Vector, size);
		mData.v = dataArr;
	}

	void Datum::SetStorage(glm::mat4* dataArr, std::size_t size)
	{
		SetStorage(DatumTypes::Matrix, size);
		mData.m = dataArr;
	}
	
	void Datum::SetStorage(std::string* dataArr, std::size_t size)
	{
		SetStorage(DatumTypes::String, size);
		mData.s = dataArr;
	}

	void Datum::SetStorage(RTTIPtr* dataArr, std::size_t size)
	{
		SetStorage(DatumTypes::Pointer, size);
		mData.p = dataArr;
	}

	void Datum::SetStorage(void* dataArr, std::size_t size)
	{
		if (dataArr == nullptr)
		{
			throw std::exception("Data Array is nullptr");
		}

		if (mIsInternalStorage && mSize > 0)
		{
			throw std::runtime_error("Data Type is internal, cannot set");
		}

		mIsInternalStorage = false;
		mSize = mCapacity = size;
		mData.voidPtr = dataArr;
	}
	#pragma endregion Set Storage

	void Datum::SetFromString(const std::string& str, size_t index)
	{
		switch (mDataType)
		{
		case DatumTypes::Integer:
			Set(std::stoi(str), index);
			break;

		case DatumTypes::Float:
			Set(std::stof(str), index);
			break;

		case DatumTypes::Vector:
			Set(ParseStringToVector(str), index);
			break;

		case DatumTypes::Matrix:
			Set(ParseStringToMatrix(str), index);
			break;

		case DatumTypes::String:
			Set(str, index);
			break;

		default:
			throw std::runtime_error("Type not supported.");

		}
	}

	glm::vec4 Datum::ParseStringToVector(const std::string& str)
	{
		float r, g, b, a;
		sscanf_s(str.c_str(), "vec4( %f, %f, %f, %f )", &r, &g, &b, &a);
		return glm::vec4(r,g,b,a);
	}

	glm::mat4 Datum::ParseStringToMatrix(const std::string& str)
	{
		float r1, g1, b1, a1;
		float r2, g2, b2, a2;
		float r3, g3, b3, a3;
		float r4, g4, b4, a4;
		sscanf_s(str.c_str(), "mat4x4((%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f))", &r1, &g1, &b1, &a1, &r2, &g2, &b2, &a2, &r3, &g3, &b3, &a3, &r4, &g4, &b4, &a4);
		glm::mat4 mat;
		mat[0] = glm::vec4(glm::vec3(r1, g1, b1), a1);
		mat[1] = glm::vec4(glm::vec3(r2, g2, b2), a2);
		mat[2] = glm::vec4(glm::vec3(r3, g3, b3), a3);
		mat[3] = glm::vec4(glm::vec3(r4, g4, b4), a4);

		return mat;
	}

	#pragma endregion Modifiers:

	const bool Datum::operator!=(const Datum& rhs) const
	{
		return !(operator==(rhs));
	}

	const bool Datum::operator!=(std::int32_t rhs) const
	{
		return !(operator==(rhs));
	}

	const bool Datum::operator!=(std::float_t rhs) const
	{
		return !(operator==(rhs));
	}

	const bool Datum::operator!=(const glm::vec4& rhs) const
	{
		return !(operator==(rhs));
	}

	const bool Datum::operator!=(const glm::mat4& rhs) const
	{
		return !(operator==(rhs));
	}

	//const bool Datum::operator!=(const Scope& rhs) const
	//{
	//	return !(operator==(rhs));
	//}

	const bool Datum::operator!=(const std::string& rhs) const
	{
		return !(operator==(rhs));
	}

	const bool Datum::operator!=(const RTTIPtr& rhs) const
	{
		return !(operator==(rhs));
	}

	const bool Datum::operator==(const Datum& rhs) const
	{
		bool areEqual = true;
		if (mDataType != rhs.mDataType)
		{
			switch (mDataType)
			{
			case DatumTypes::String:
				for (size_t i = 0; i < mSize; ++i)
				{
					if (mData.s[i] != rhs.mData.s[i])
					{
						areEqual = false;
						break;
					}
				}
				break;
			case DatumTypes::Pointer:
			case DatumTypes::Table:
				for (uint32_t i = 0; i < mSize; ++i)
				{
					if (mData.p[i] != rhs.mData.p[i] && mData.p[i] != nullptr)
					{
						if (mData.p[i]->Equals(rhs.mData.p[i]) == false)
						{
							areEqual = false;
							break;
						}
					}
				}
				break;
			case DatumTypes::Integer:
			case DatumTypes::Float:
			case DatumTypes::Vector:
			case DatumTypes::Matrix:
				if (memcmp(mData.voidPtr, rhs.mData.voidPtr, DatumSizes[static_cast<size_t>(mDataType)] * mSize) != 0)
				{
					areEqual = false;
				}

				break;
			default:
				throw std::runtime_error("Type not supported");
				break;
			}
		}
		else areEqual = false;
		
		return areEqual;
	}

	const bool Datum::operator==(std::int32_t rhs) const
	{
		return mSize > 0 && Get<std::int32_t>() == rhs;
	}

	const bool Datum::operator==(std::float_t rhs) const
	{
		return mSize > 0 && Get<std::float_t>() == rhs;
	}

	const bool Datum::operator==(const glm::vec4& rhs) const
	{
		return mSize > 0 && Get<glm::vec4>() == rhs;
	}

	const bool Datum::operator==(const glm::mat4& rhs) const
	{
		return mSize > 0 && Get<glm::mat4>() == rhs;
	}

	//const bool Datum::operator==(const Scope& rhs) const
	//{
	//	return mSize > 0 && Get<ScopePtr>()->Equals(&rhs);
	//}

	const bool Datum::operator==(const std::string& rhs) const
	{
		return mSize > 0 && Get<std::string>().compare(rhs);
	}
	const bool Datum::operator==(const RTTIPtr& rhs) const
	{
		return mSize > 0 && Get<RTTIPtr>()->Equals(rhs);
	}


#pragma region Setters
	void Datum::Set(const DatumTypes& dataType, std::size_t index)
	{
		if (mDataType == DatumTypes::Unknown)
		{
			mDataType = dataType;
			mIsInternalStorage = true;
		}

		if (mDataType != dataType)
		{
			throw std::runtime_error("type mismatch.");
		}

		if (index >= mSize)
		{
			if (!mIsInternalStorage)
			{
				throw std::runtime_error("Invalid, datum is External storage");
			}
			Resize(index + 1);
		}
	}

	void Datum::Set(const std::int32_t& value, std::size_t index)
	{
		Set(DatumTypes::Integer, index);
		mData.i[index] = value;
	}

	void Datum::Set(const std::float_t& value, std::size_t index)
	{
		Set(DatumTypes::Float, index);
		mData.f[index] = value;
	}

	void Datum::Set(const glm::vec4& value, std::size_t index)
	{
		Set(DatumTypes::Vector, index);
		mData.v[index] = value;
	}

	void Datum::Set(const glm::mat4& value, std::size_t index)
	{
		Set(DatumTypes::Matrix, index);
		mData.m[index] = value;
	}

	void Datum::Set(const std::string& value, std::size_t index)
	{
		Set(DatumTypes::String, index);
		mData.s[index] = value;
	}

	void Datum::Set(RTTIPtr value, std::size_t index)
	{
		Set(DatumTypes::Pointer, index);
		mData.p[index] = value;
	}

	void Datum::Set(ScopePtr value, std::size_t index)
	{
		Set(DatumTypes::Table, index);
		mData.t[index] = value;
	}
#pragma endregion
		

	void Datum::PopBack()
	{
		if (!mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot change size nor capacity.");
		}
		if (mDataType == DatumTypes::Unknown)
		{
			throw std::runtime_error("No Type Set");
		}

		if (!IsEmpty())
		{
			switch (mDataType)
			{
			case DatumTypes::Integer:
				break;
			case DatumTypes::Float:
				break;
			case DatumTypes::Vector:
				At<glm::vec4>(mSize - 1).glm::vec4::~vec4();
				break;
			case DatumTypes::Matrix:
				At<glm::mat4>(mSize - 1).glm::mat4x4::~mat4x4();
				break;
			case DatumTypes::String:
				At<std::string>(mSize - 1).std::string::~string();
				break;
			case DatumTypes::Pointer:
				At<RTTIPtr>(mSize - 1) = nullptr;
				break;
			default:
				throw std::exception("Data type is not valid.");
			}
		}
		--mSize;
	}

	void Datum::PushBack(const std::int32_t& value)
	{
		InitPushBack(DatumTypes::Integer);
		new (mData.i + mSize++)std::int32_t(value);
	}

	void Datum::PushBack(const std::float_t& value)
	{
		InitPushBack(DatumTypes::Float);
		new (mData.f + mSize++)std::float_t(value);
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		InitPushBack(DatumTypes::Vector);
		new (mData.v + mSize++)glm::vec4(value);
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		InitPushBack(DatumTypes::Matrix);
		new (mData.m + mSize++)glm::mat4(value);
	}

	void Datum::PushBack(const std::string& value)
	{
		InitPushBack(DatumTypes::String);
		new (mData.s + mSize++)std::string(value);
	}

	void Datum::PushBack(RTTIPtr value)
	{
		InitPushBack(DatumTypes::Pointer);
		new (mData.p + mSize++)RTTI*(value);
	}

	void Datum::PushBack(ScopePtr value)
	{
		InitPushBack(DatumTypes::Table);
		new (mData.t + mSize++)Scope*(value);
	}

	void Datum::InitPushBack(Datum::DatumTypes type)
	{
		if (!mIsInternalStorage)
		{
			throw std::runtime_error("Is External Storage, cannot change size nor capacity.");
		}

		if (mDataType == DatumTypes::Unknown)
		{
			mDataType = type;
		}
		else if (mDataType != type)
		{
			throw std::runtime_error("Type mismatch.");
		}

		if (mSize == mCapacity)
		{
			Reserve(mCapacity + std::max(mIncrementFunctor(mSize, mCapacity), size_t(1)));
		}
	}
}
