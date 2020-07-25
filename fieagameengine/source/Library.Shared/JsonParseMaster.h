#pragma once

#pragma warning(push)
#pragma warning(disable: 26812)
#include "json/json.h"
#pragma warning(pop)
#include <gsl/gsl>
#include "vector.h"
#include "RTTI.h"

namespace Library
{
	/// <summary>
	/// forward declarion of Interface Json Parse Helper
	/// </summary>
	class IJsonParseHelper;

	/// <summary>
	/// class to wrap JsonCpp functionality in a C++ class
	/// </summary>
	class JsonParseMaster final
	{
	public:
		/// <summary>
		/// This class will represent data that all helpers share with each other and with the master parser. 
		/// This is a base class.
		/// </summary>
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)

			friend JsonParseMaster;

		public:
			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="jsonParseMaster">pointer to JsonParseMaster that its linked to</param>
			explicit SharedData(JsonParseMaster* jsonParseMaster = nullptr);
			/// <summary>
			/// Copy Constructor (default)
			/// </summary>
			SharedData(const SharedData&) = default;
			/// <summary>
			/// Move Constructor (default)
			/// </summary>
			SharedData(SharedData&&) noexcept = default;
			/// <summary>
			/// Copy Assignment(default)
			/// </summary>
			SharedData& operator=(const SharedData&) = default;
			/// <summary>
			/// Move Assignment (default)
			/// </summary>
			SharedData& operator=(SharedData&&) noexcept = default;
			/// <summary>
			/// Virtual Destructor (default)
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Used to Initialize the SharedData, not pure virtual because might not need it
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// (pure virtual) overridden implementations will create an instance of the shared data.
			/// </summary>
			/// <returns>instance of SharedData</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// return the address of the JsonParseMaster associated with this object.
			/// </summary>
			/// <returns>address of the JsonParseMaster</returns>
			JsonParseMaster* GetJsonParseMaster() const;

			/// <summary>
			/// return the current nesting depth.
			/// </summary>
			/// <returns>size_t of current Depth</returns>
			const std::size_t Depth() const;

		protected:
			std::size_t mDepth{ 0 };

		private:
			/// <summary>
			/// given the address of an JsonParseMaster, assign it to a private member.
			/// </summary>
			/// <param name="jsonParseMaster">address of an JsonParseMaster</param>
			void SetJsonParseMaster(JsonParseMaster& jsonParseMaster);

			/// <summary>
			/// increment a “nesting depth” counter. This gets incremented upon the start of each element.
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// decrement a “nesting depth” counter. This gets decremented upon the end of each element.
			/// </summary>
			void DecrementDepth();

			JsonParseMaster* mJsonParseMaster{ nullptr };
		};

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="sharedData">SharedData Ref</param>
		explicit JsonParseMaster(SharedData& sharedData);

		/// <summary>
		/// Copy COnstructor (deleted)
		/// </summary>
		JsonParseMaster(const JsonParseMaster& rhs) = delete;
		/// <summary>
		/// Copy Assignment (deleted)
		/// </summary>
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = delete;

		/// <summary>
		/// move Constructor
		/// </summary>
		/// <param name="rhs">rvalue JsonParseMaster to move</param>
		/// <returns>JsonParseMaster with Moved Memory</returns>
		JsonParseMaster(JsonParseMaster&& rhs) noexcept;
		/// <summary>
		/// Move Assignment 
		/// </summary>
		/// <param name="rhs">rvalue JsonParseMaster to move</param>
		/// <returns>JsonParseMaster with Moved Memory</returns>
		JsonParseMaster& operator=(JsonParseMaster&& rhs) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		///  duplicate this object and return its address
		/// </summary>
		/// <returns>address of duplicated object</returns>
		gsl::owner<JsonParseMaster*> Clone() const;

		/// <summary>
		/// given a reference to an IJsonParseHelper object, add it to the list.
		/// </summary>
		/// <param name="helper">IJsonParseHelper object</param>
		void AddHelper(IJsonParseHelper& helper);
		/// <summary>
		/// given a reference to an IJsonParseHelper object, remove it from the list.
		/// </summary>
		/// <param name="helper">IJsonParseHelper object</param>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Parse a string of Json data.
		/// </summary>
		/// <param name="jsonData">a string of Json data.</param>
		void Parse(const std::string& jsonData);
		/// <summary>
		/// Parse an input stream of Json data (std::istream).
		/// </summary>
		/// <param name="jsonInputStream">an input stream of Json data (std::istream).</param>
		void Parse(std::istream& jsonInputStream);
		/// <summary>
		/// given a filename, read in the file and parse it.
		/// </summary>
		/// <param name="fileName">string of filename</param>
		void ParseFromFile(const std::string& fileName);

		/// <summary>
		/// return the path for the file being parsed, passed into ParseFromFile.
		/// </summary>
		/// <returns>name of file</returns>
		const std::string& GetFileName() const;

		/// <summary>
		///  return the address of the SharedData associated with this object.
		/// </summary>
		/// <returns>address of the SharedData</returns>
		SharedData* GetSharedData() const;
		/// <summary>
		/// given the address of a SharedData object, associate it with this object
		/// </summary>
		/// <param name="sharedData">address of a SharedData object</param>
		void SetSharedData(SharedData& sharedData);
		
		/// <summary>
		/// Used to Initialize SharedData, Helpers, and clear filename
		/// </summary>
		void Initialize();

		/// <summary>
		/// Returns a bool if this is a clone
		/// </summary>
		/// <returns>bool</returns>
		bool IsClone() const;

		/// <summary>
		/// returns a const Vector Ref of the list of helpers
		/// </summary>
		/// <returns>a const Vector Ref of the list of helpers</returns>
		const Vector<IJsonParseHelper*>& GetListOfHelpers() const;

	private:
		/// <summary>
		/// Retrieves the members from a Json::Value object and Parse's each of them.
		/// </summary>
		/// <param name="jsonValue">Json::Value object</param>
		void ParseMembers(const Json::Value& jsonValue, bool isArrayElement = false, std::size_t index = 0);
		/// <summary>
		/// Given a key/value pair (string/Json::Value) and a bool indicating if the value is an element of an array, trigger the chain of responsibility. 
		/// </summary>
		/// <param name="key">string</param>
		/// <param name="jsonValue">Json::Value object</param>
		/// <param name="isArrayElement">bool</param>
		/// <param name="index">size_t</param>
		void Parse(const std::string& key, const Json::Value& jsonValue, bool isArrayElement = false, std::size_t index = 0);

		Vector<IJsonParseHelper*> mListOfHelpers;
		SharedData* mSharedData{ nullptr };
		bool mIsClone{ false };
		std::string mFileName;
	};
}