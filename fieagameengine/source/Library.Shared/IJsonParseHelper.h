#pragma once

#include "RTTI.h"
#include "JsonParseMaster.h"


namespace Library
{

	/// <summary>
	/// Abstract Helper Class, where concrete helper classes will derive from
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)

	public:
		/// <summary>
		/// Default Constructor, Defaulted
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// Copy onstructor (Delete)
		/// </summary>
		IJsonParseHelper(const IJsonParseHelper& rhs) = delete;
		/// <summary>
		/// Copy Assignment (Delete)
		/// </summary>
		IJsonParseHelper& operator=(const IJsonParseHelper& rhs) = delete;

		/// <summary>
		/// Move Constructor, defaulted
		/// </summary>
		/// <param name="rhs">IJsonParseHelper to ber moved</param>
		/// <returns>reference to the moved IJsonParseHelper is returned</returns>
		IJsonParseHelper(IJsonParseHelper&& rhs) noexcept = default;
		/// <summary>
		/// Move Assignment Operator, Defaulted
		/// </summary>
		/// <param name="rhs">IJsonParseHelper to ber moved</param>
		/// <returns>reference to the moved IJsonParseHelper is returned</returns>
		IJsonParseHelper& operator=(IJsonParseHelper&& rhs) noexcept = default;
		/// <summary>
		/// virtual destructor (default)
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Initialize this helper. 
		/// This will get called just before each file is parsed.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Given a shared data reference, a string for the Json key, 
		/// a reference to the Json::Value object, 
		/// and a bool indicating if the value is an array element, 
		/// attempt to handle the key/value pair. 
		/// If this routine does indeed handle the pair, 
		/// return true, otherwise return false.
		/// </summary>
		/// <param name="sharedData">shared data reference</param>
		/// <param name="jsonKey">a string for the Json key</param>
		/// <param name="jsonObjectRef">a reference to the Json::Value object</param>
		/// <param name="isArrayElement">is an array element</param>
		/// <returns>a bool</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, const Json::Value& jsonValueRef, bool isArrayElement = false, std::size_t index = 0) = 0;
		//Increment Depth of shared data

		/// <summary>
		/// Given a shared data reference, a string for the Json key, 
		/// attempt to complete the handling of the element pair. 
		/// If this routine does indeed handle the pair, 
		/// return true, otherwise return false. 
		/// </summary>
		/// <param name="sharedData">shared data reference</param>
		/// <param name="jsonKey">string for the Json key</param>
		/// <returns>a bool</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey) = 0;
		//Decrement Depth of shared data

		/// <summary>
		/// overridden implementations will create an instance of the helper. This is a so-called “virtual constructor”.
		/// </summary>
		/// <returns>instance of the helper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;

	};
}
