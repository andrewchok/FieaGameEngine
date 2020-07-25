#pragma once
#include "IJsonParseHelper.h"
#include "Stack.h"
#include "Scope.h"

namespace Library
{
	/// <summary>
	/// Abstract Helper Class, where concrete helper classes will derive from
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)
	public:
		/// <summary>
		/// Inherited SharedData Class
		/// </summary>
		class SharedData final : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)
		public:
			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="jsonParseMaster">pointer to JsonParseMaster that its linked to</param>
			explicit SharedData(Scope& scope);
			/// <summary>
			/// Copy Constructor (deleted)
			/// </summary>
			SharedData(const SharedData&) = delete;
			/// <summary>
			/// Move Constructor (deleted)
			/// </summary>
			SharedData(SharedData&&) noexcept = delete;
			/// <summary>
			/// Copy Assignment(deleted)
			/// </summary>
			SharedData& operator=(const SharedData&) = delete;
			/// <summary>
			/// Move Assignment (deleted)
			/// </summary>
			SharedData& operator=(SharedData&&) noexcept = delete;
			/// <summary>
			/// Virtual Destructor (default)
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Initialize:
			/// </summary>
			virtual void Initialize() override;
			/// <summary>
			/// create a new SharedData of this type and return it
			/// </summary>
			/// <returns>pointer to newly made SharedData</returns>
			virtual gsl::owner<SharedData*> Create() const override;

			Scope* GetSharedData();
			void SetSharedData(Scope& scopeSharedData);

		private:
			Scope* mScopeSharedData{ nullptr };
		};
		/// <summary>
		/// Constructor (default)
		/// </summary>
		JsonTableParseHelper() = default;
		/// <summary>
		/// Copy onstructor (Delete)
		/// </summary>
		JsonTableParseHelper(const JsonTableParseHelper& rhs) = delete;
		/// <summary>
		/// Copy Assignment (Delete)
		/// </summary>
		JsonTableParseHelper& operator=(const JsonTableParseHelper& rhs) = delete;
		/// <summary>
		/// Move Constructor (default)
		/// </summary>
		JsonTableParseHelper(JsonTableParseHelper&& rhs) noexcept = default;
		/// <summary>
		/// Move Assignment (default)
		/// </summary>
		JsonTableParseHelper& operator=(JsonTableParseHelper&& rhs) noexcept = default;
		/// <summary>
		/// virtual destructor (default)
		/// </summary>
		virtual ~JsonTableParseHelper() = default;

		/// <summary>
		/// Initialize: 
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
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, const Json::Value& jsonValue, bool isArrayElement = false, std::size_t index = 0) override;
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

	private:

		struct Context
		{
			const std::string key{ "" };
			Datum::DatumTypes type{ Datum::DatumTypes::Unknown };
			std::string className{ "" };
			Scope* scope{ nullptr };
		};

		Stack<Context> mContextStack;

	};

}
