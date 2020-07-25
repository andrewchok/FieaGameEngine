#pragma once

#include "IJsonParseHelper.h"

namespace Library
{
	/// <summary>
	/// Abstract Helper Class, where concrete helper classes will derive from
	/// </summary>
	class JsonTestParseHelper final : public IJsonParseHelper
	{
	public:
		/// <summary>
		/// Inherited SharedData Class
		/// </summary>
		class SharedData final : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)
		public:
			/// <summary>
			/// Initialize: set maxDepth to 0
			/// </summary>
			virtual void Initialize() override;
			/// <summary>
			/// create a new SharedData of this type and return it
			/// </summary>
			/// <returns>pointer to newly made SharedData</returns>
			virtual gsl::owner<SharedData*> Create() const override;

			std::size_t mMaxDepth{ 0 };
		};
		/// <summary>
		/// Constructor (default)
		/// </summary>
		JsonTestParseHelper() = default;
		/// <summary>
		/// Copy onstructor (Delete)
		/// </summary>
		JsonTestParseHelper(const JsonTestParseHelper& rhs) = delete;
		/// <summary>
		/// Copy Assignment (Delete)
		/// </summary>
		JsonTestParseHelper& operator=(const JsonTestParseHelper& rhs) = delete;
		/// <summary>
		/// Move Constructor (default)
		/// </summary>
		JsonTestParseHelper(JsonTestParseHelper&& rhs) noexcept = default;
		/// <summary>
		/// Move Assignment (default)
		/// </summary>
		JsonTestParseHelper& operator=(JsonTestParseHelper&& rhs) noexcept = default;

		/// <summary>
		/// virtual destructor (default)
		/// </summary>
		virtual ~JsonTestParseHelper() = default;

		/// <summary>
		/// Initialize: set isInit to true, and set all counters to 0
		/// </summary>
		virtual void Initialize() override;
		/// <summary>
		/// Given a shared data reference, a string for the Json key, 
		/// a reference to the Json::Value object, 
		/// and a bool indicating if the value is an array element, 
		/// attempt to handle the key/value pair. 
		/// If this routine does indeed handle the pair, return true, otherwise return false.
		/// </summary>
		/// <param name="sharedData">ParseMaster Shared Data</param>
		/// <param name="jsonKey">String</param>
		/// <param name="jsonValueRef">Json::Value</param>
		/// <param name="isArrayElement">bool</param>
		/// <param name="index">size_t</param>
		/// <returns>bool</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, const Json::Value& jsonValueRef, bool isArrayElement = false, std::size_t index = 0) override;
		/// <summary>
		/// Given a shared data reference, a string for the Json key, 
		/// attempt to complete the handling of the element pair. 
		/// If this routine does indeed handle the pair, 
		/// return true, otherwise return false. 
		/// </summary>
		/// <param name="sharedData">ParseMaster Shared Data</param>
		/// <param name="jsonKey">string</param>
		/// <returns>bool</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey) override;
		/// <summary>
		/// overridden implementations will create an instance of the helper
		/// </summary>
		/// <returns>instance of the helper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

		bool mIsInitialized = false;
		std::size_t mStartHandlerCount = 0;
		std::size_t mEndHandlerCount = 0;
		std::size_t mArrayCount = 0;
	};
}
