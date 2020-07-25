#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include <fstream>

namespace Library
{
#pragma region Shared Data

	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

	JsonParseMaster::SharedData::SharedData(JsonParseMaster* jsonParseMaster) :
		mJsonParseMaster(jsonParseMaster)
	{
	}

	void JsonParseMaster::SharedData::Initialize()
	{
		mDepth = 0;
	}

	JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return mJsonParseMaster;
	}

	void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster& jsonParseMaster)
	{
		mJsonParseMaster = &jsonParseMaster;
	}

	void JsonParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		if (mDepth == 0) throw std::runtime_error("Decrementing 0");
		--mDepth;
	}

	const std::size_t JsonParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}


#pragma endregion Shared Data

#pragma region Json Parse Master

	JsonParseMaster::JsonParseMaster(SharedData& sharedData) :
		mSharedData(&sharedData)
	{
		sharedData.SetJsonParseMaster(*this);
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) noexcept :
		mListOfHelpers(std::move(rhs.mListOfHelpers)),
		mSharedData(rhs.mSharedData),
		mIsClone(rhs.mIsClone),
		mFileName(std::move(rhs.mFileName))
	{
		mSharedData->SetJsonParseMaster(*this);
		rhs.mSharedData = nullptr;
		rhs.mIsClone = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mListOfHelpers = std::move(rhs.mListOfHelpers);
			mSharedData = rhs.mSharedData;
			mIsClone = rhs.mIsClone;
			mFileName = std::move(rhs.mFileName);

			mSharedData->SetJsonParseMaster(*this);
			rhs.mSharedData = nullptr;
			rhs.mIsClone = false;
		}
		return *this;
	}

	JsonParseMaster::~JsonParseMaster()
	{
		if (mIsClone)
		{
			for (auto& helper : mListOfHelpers)
			{
				delete helper;
			}

			delete mSharedData;
		}
	}
	
	gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
	{
		gsl::owner<JsonParseMaster*> clone = new JsonParseMaster(*mSharedData->Create());

		clone->mListOfHelpers.Reserve(mListOfHelpers.Size());

		for (auto& helper : mListOfHelpers)
		{
			clone->mListOfHelpers.PushBack(helper->Create());
		}

		clone->mIsClone = true;
		
		return clone;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		if (mIsClone)
		{
			throw std::runtime_error("Cannot add Helper to a clone");
		}

		auto it = std::find_if(mListOfHelpers.begin(), mListOfHelpers.end(), [&helper](const IJsonParseHelper* iJsonParseHelper) 
		{
			return (helper.TypeIdInstance() == iJsonParseHelper->TypeIdInstance());
		});

		if (it != mListOfHelpers.end())
		{
			throw std::runtime_error("Helper being added already exists");
		}

		mListOfHelpers.PushBack(&helper);
	}

	void JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		// todo: allow removal of helper
		if (mIsClone)
		{
			throw std::runtime_error("Cannot remove Helper from a clone");
		}

		mListOfHelpers.Remove(&helper);
	}

	void JsonParseMaster::Parse(const std::string& jsonData)
	{
		std::stringstream strStream;
		strStream << jsonData;
		Parse(strStream);
	}

	void JsonParseMaster::Parse(std::istream& jsonInputStream)
	{
		Json::Value root;
		jsonInputStream >> root;

		mSharedData->IncrementDepth();
		ParseMembers(root);
		mSharedData->DecrementDepth();
	}

	void JsonParseMaster::Parse(const std::string& key, const Json::Value& jsonValue, bool isArrayElement, std::size_t index)
	{
		if (jsonValue.isObject())
		{
			mSharedData->IncrementDepth();
			
			for (IJsonParseHelper* helper : mListOfHelpers)
			{
				if (helper->StartHandler(*mSharedData, key, jsonValue, isArrayElement, index))
				{
					ParseMembers(jsonValue);
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}

			mSharedData->DecrementDepth();
		}
		else if (jsonValue.isArray())
		{
			std::size_t arrIndex = 0;
			// todo: check if it is an object or not if(value.isObject())
			for (; arrIndex < jsonValue.size(); ++arrIndex)
			{
				const auto& element = jsonValue[static_cast<int>(arrIndex)];
				if (element.isObject())
				{
					mSharedData->IncrementDepth();
					ParseMembers(element, true, arrIndex);
					mSharedData->DecrementDepth();
				}
				else
				{
					Parse(key, element, true, arrIndex);
				}
			}
		}
		else // is a Primitive
		{
			for (IJsonParseHelper* helper : mListOfHelpers)
			{
				if (helper->StartHandler(*mSharedData, key, jsonValue, isArrayElement, index))
				{
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
		}
	}

	void JsonParseMaster::ParseMembers(const Json::Value& jsonValue, bool isArrayElement, std::size_t index)
	{
		if (jsonValue.size() > 0)
		{
			const std::vector<std::string> keys = jsonValue.getMemberNames();

			for (const std::string& key : keys)
			{
				auto& value = jsonValue[key];
				Parse(key, value, isArrayElement, index);
			}
		}
	}

	void JsonParseMaster::ParseFromFile(const std::string& fileName)
	{
		Initialize();
		std::ifstream jsonFile(fileName);

		if (!(jsonFile.good()))
		{
			throw std::runtime_error("Bad File");
		}

		mFileName = fileName;

		Parse(jsonFile);
	}

	const std::string& JsonParseMaster::GetFileName() const
	{
		return mFileName;
	}

	JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	void JsonParseMaster::SetSharedData(SharedData& sharedData)
	{
		if (mIsClone)
		{
			throw std::runtime_error("Cannot change a Clone's Shared Data");
		}

		mSharedData = &sharedData;
		mSharedData->SetJsonParseMaster(*this);
	}

	void JsonParseMaster::Initialize()
	{
		if(mSharedData != nullptr) mSharedData->Initialize();

		for (auto& helper : mListOfHelpers)
		{
			helper->Initialize();
		}

		mFileName.clear();
	}

	bool JsonParseMaster::IsClone() const
	{
		return mIsClone;
	}

	const Vector<IJsonParseHelper*>& JsonParseMaster::GetListOfHelpers() const
	{
		return mListOfHelpers;
	}

#pragma endregion Json Parse Master

}
