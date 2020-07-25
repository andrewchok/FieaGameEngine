#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Sector.h"

namespace Library
{
#pragma region Shared Data

	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

	JsonTableParseHelper::SharedData::SharedData(Scope& scope) :
		mScopeSharedData(&scope)
	{
	}

	void JsonTableParseHelper::SharedData::Initialize()
	{
		// setup a pattern of Initializing
		JsonParseMaster::SharedData::Initialize();
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
	{
		Scope* scope = new Scope();
		return new SharedData(*scope);
	}

	Scope* JsonTableParseHelper::SharedData::GetSharedData()
	{
		return mScopeSharedData;
	}

	void JsonTableParseHelper::SharedData::SetSharedData(Scope& scopeSharedData)
	{
		mScopeSharedData = &scopeSharedData;
	}

#pragma endregion

#pragma region Json Test Parse Helper

	RTTI_DEFINITIONS(JsonTableParseHelper)

	void JsonTableParseHelper::Initialize()
	{
		// setup a pattern of Initializing
		IJsonParseHelper::Initialize();
	
		while (mContextStack.Size() > 0) mContextStack.Pop();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, const Json::Value& jsonValue, bool isArrayElement, std::size_t index)
	{
		JsonTableParseHelper::SharedData* tempSharedData = sharedData.As<JsonTableParseHelper::SharedData>();
		if (tempSharedData == nullptr) return false;

		if (jsonKey == "class")
		{
			assert(mContextStack.IsEmpty() == false);
			Context& contextFrame = mContextStack.Top();

			contextFrame.className = jsonValue.asString();
		}

		else if (jsonKey == "type")
		{
			assert(mContextStack.IsEmpty() == false);
			Context& contextFrame = mContextStack.Top();
			const auto& type = jsonValue.asString();

			contextFrame.type = Datum::StringToDatumTypesMap[type];
			Datum& datum = contextFrame.scope->Append(contextFrame.key);
			datum.SetType(contextFrame.type);
		}

		else if (jsonKey == "value")
		{
			assert(mContextStack.IsEmpty() == false);
			Context& contextFrame = mContextStack.Top();
			Datum* d = contextFrame.scope->Find(contextFrame.key);
			assert(d != nullptr);
			Datum& datum = *d;
			
			if (datum.Type() == Datum::DatumTypes::Table)
			{
				Scope* nestedScope;
				if (datum.Size() > index)
				{
					nestedScope = &(datum.operator[](index));
				}
				else if (contextFrame.className.empty() == false)
				{
					nestedScope = Factory<Scope>::Create(contextFrame.className);
					assert(nestedScope != nullptr);
					contextFrame.scope->Adopt(*nestedScope, contextFrame.key);
				}
				else
				{
					nestedScope = &(contextFrame.scope->AppendScope(contextFrame.key));
				}

				if (isArrayElement)
				{
					mContextStack.Push({ jsonKey, Datum::DatumTypes::Table, std::string(), nestedScope });
				}
				else
				{
					contextFrame.scope = nestedScope;
				}				
			}
			else
			{
				switch (datum.Type())
				{
				case Datum::DatumTypes::Integer:
					datum.Set(jsonValue.asInt(), index);
					break;
				case Datum::DatumTypes::Float:
					datum.Set(jsonValue.asFloat(), index);
					break;
				case Datum::DatumTypes::String:
					datum.Set(jsonValue.asString(), index);
					break;
				case Datum::DatumTypes::Vector:
				case Datum::DatumTypes::Matrix:
				default:
					datum.SetFromString(jsonValue.asString(), index);
					break;
				}
			}
		}
		
		else
		{
			Scope* scope = (mContextStack.IsEmpty() ? tempSharedData->GetSharedData() : mContextStack.Top().scope);
			assert(scope != nullptr);
			Context contextFrame({ jsonKey, Datum::DatumTypes::Unknown, std::string(), scope });
			mContextStack.Push(contextFrame);			
		}
			   
		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey)
	{
		// todo: do an "Is" check (slightly cheaper) dont need the pointer after
		JsonTableParseHelper::SharedData* tempSharedData = sharedData.As<JsonTableParseHelper::SharedData>();
		if (tempSharedData == nullptr) return false;

		if (mContextStack.Top().key == jsonKey)
		{
			mContextStack.Pop();
		}

		return true;
	}

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}
	   
#pragma endregion
}