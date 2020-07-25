#include "pch.h"
#include "JsonTestParseHelper.h"

namespace Library
{
#pragma region Shared Data

	RTTI_DEFINITIONS(JsonTestParseHelper::SharedData)

	void JsonTestParseHelper::SharedData::Initialize()
	{
		// setup a pattern of Initializing
		JsonParseMaster::SharedData::Initialize();
		mMaxDepth = 0;
	}

	gsl::owner<JsonTestParseHelper::SharedData*> JsonTestParseHelper::SharedData::Create() const
	{
		return new SharedData();
	}

#pragma endregion

#pragma region Json Test Parse Helper

	void JsonTestParseHelper::Initialize()
	{
		// setup a pattern of Initializing
		IJsonParseHelper::Initialize();
		mIsInitialized = true;
		mStartHandlerCount = 0;
		mEndHandlerCount = 0;
		mArrayCount = 0;
	}

	bool JsonTestParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& /*jsonKey*/, const Json::Value& /*jsonObjectRef*/, bool isArrayElement, std::size_t /*index*/)
	{
		JsonTestParseHelper::SharedData* tempSharedData = sharedData.As<JsonTestParseHelper::SharedData>();
		// todo:: remove assert and just return false, 
		assert(tempSharedData != nullptr);

		++mStartHandlerCount;

		if (isArrayElement) ++mArrayCount;

		if (tempSharedData->Depth() > tempSharedData->mMaxDepth)
		{
			tempSharedData->mMaxDepth = tempSharedData->Depth();
		}

		return true;
	}

	bool JsonTestParseHelper::EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& /*jsonKey*/)
	{
		// todo: do an "Is" check (slightly cheaper) dont need the pointer after
		JsonTestParseHelper::SharedData* tempSharedData = sharedData.As<JsonTestParseHelper::SharedData>();

		if (tempSharedData == nullptr) return false;

		++mEndHandlerCount;

		return true;
	}

	gsl::owner<IJsonParseHelper*> JsonTestParseHelper::Create() const
	{
		return new JsonTestParseHelper;
	}

#pragma endregion


}
