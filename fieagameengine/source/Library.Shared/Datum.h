#pragma once

#include <functional>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable: 4634)
#include <glm/glm.hpp>
#pragma warning(push)
#pragma warning(disable: 4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning(pop)
#pragma warning(pop)

#include "RTTI.h"
#include "Enum.h"
#include "DefaultIncrement.h"
#include "hashmap.h"


namespace Library
{
	class Scope;

	class Datum final
	{
		friend Scope;

	private:
		union DatumValue final
		{
		public:

			void* voidPtr{ nullptr };
			std::int32_t* i;
			std::float_t* f;
			glm::vec4* v;
			glm::mat4* m;
			Scope** t;
			std::string* s;
			RTTI** p;
		};

	public:
		using IncrementFunctor = std::function<size_t(const size_t, const size_t)>;
		using RTTIPtr = RTTI*;
		using ScopePtr = Scope*;

		enum class DatumTypes
		{
			Unknown,

			Integer,
			Float,
			Vector,
			Matrix,
			Table,
			String,
			Pointer,
			Reference,

			Max
		};

	#pragma region Constructors, Assignments & Destructor:

		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit Datum(DatumTypes dataType = DatumTypes::Unknown,
			bool isInternalStorage = true,
			size_t capacity = 0,
			IncrementFunctor incrementFunctor = DefaultIncrement<DatumValue>());

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Datum reference</param>
		Datum(const Datum& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Datum&&</param>
		/// <returns>Moved Datum</returns>
		Datum(Datum&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Datum reference</param>
		/// <returns>Copied Datum</returns>
		Datum& operator=(const Datum& rhs);
		Datum& operator=(std::int32_t rhs);
		Datum& operator=(std::float_t rhs);
		Datum& operator=(glm::vec4 rhs);
		Datum& operator=(glm::mat4 rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(RTTIPtr& rhs);
		Datum& operator=(Scope* rhs);

		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Datum&&</param>
		/// <returns>Moved Datum</returns>
		Datum& operator=(Datum&& rhs) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		~Datum();

		/// <summary>
		/// Equal Operator
		/// </summary>
		/// <param name="rhs">takes in a rhs</param>
		/// <returns>bool</returns>
		const bool operator == (const Datum& rhs) const;
		const bool operator == (std::int32_t rhs) const;
		const bool operator == (std::float_t rhs) const;
		const bool operator == (const glm::vec4& rhs) const;
		const bool operator == (const glm::mat4& rhs) const;
		//const bool operator == (const Scope& rhs) const;
		const bool operator == (const std::string& rhs) const;
		const bool operator == (const RTTIPtr& rhs) const;
		/// <summary>
		/// Not Equal Operator
		/// </summary>
		/// <param name="rhs">takes in a rhs</param>
		/// <returns>bool</returns>
		const bool operator != (const Datum& rhs) const;
		const bool operator != (std::int32_t rhs) const;
		const bool operator != (std::float_t rhs) const;
		const bool operator != (const glm::vec4& rhs) const;
		const bool operator != (const glm::mat4& rhs) const;
		//const bool operator != (const Scope& rhs) const;
		const bool operator != (const std::string& rhs) const;
		const bool operator != (const RTTIPtr& rhs) const;

	#pragma endregion Constructors, Assignments & Destructor:


	#pragma region Storage:

		/// <summary>
		/// Returns type enum associated with this object.
		/// </summary>
		/// <returns>DatumTypes enum</returns>
		DatumTypes Type() const;

		const bool IsExternal() const;

		/// <summary>
		/// Return a bool indicating whether the list contains any items
		/// </summary>
		/// <returns>true if empty and false if not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// return the number of items in the list as an unsigned int
		/// </summary>
		/// <returns>number of items in list as an unsigned int</returns>
		size_t Size() const;

		/// <summary>
		/// return the capacity of the list as an unsigned int
		/// </summary>
		/// <returns>capacity of list as an unsigned int</returns>
		size_t Capacity() const;

		/// <summary>
		/// Used to Allocate more space in memory, if given a smaller capacity than current
		/// it will increase the current capcity by one.
		/// </summary>
		/// <param name="capacity">Capacity to increase allocated memory to</param>
		void Reserve(const size_t capacity);

		/// <summary>
		/// Given integer number of values, set number of values and reserve memory if needed. 
		/// This allows shrinking or growing. 
		/// After a Resize call the Datum's Size and Capacity are the same.
		/// </summary>
		void Resize(const size_t size);

	#pragma endregion Storage:

	#pragma region Element Access:
		
		/// <summary>
		/// Gets a copy of Data at index
		/// </summary>
		template<typename T>
		T& Get(size_t index = 0);

		template<> Scope*& Get<Scope*>(size_t index);

		/// <summary>
		/// Gets a copy of Data at index (const)
		/// </summary>
		template<typename T>
		const T& Get(size_t index = 0) const;
		
		/// <summary>
		/// return the first item in the list, if list is empty throws exception
		/// </summary>
		/// <returns>the first item in the list</returns>
		/// <exception cref="runtime_error">List is empty.</exception>
		template<typename T>
		T& Front();
		/// <summary>
		/// return the first item in the list as a const, if list is empty throws exception
		/// </summary>
		/// <returns>the first item in the list as a const</returns>
		template<typename T>
		const T& Front() const;

		/// <summary>
		/// return the last item in the list, if list is empty throws exception
		/// </summary>
		/// <returns>the last item in the list</returns>
		template<typename T>
		T& Back();
		/// <summary>
		/// return the last item in the list as a const, if list is empty throws exception
		/// </summary>
		/// <returns>return the last item in the list as a const</returns>
		template<typename T>
		const T& Back() const;

		std::tuple<size_t, bool> Find(const std::int32_t& value);
		std::tuple<size_t, bool> Find(const std::float_t& value);
		std::tuple<size_t, bool> Find(const glm::vec4& value);
		std::tuple<size_t, bool> Find(const glm::mat4& value);
		std::tuple<size_t, bool> Find(const std::string& value);
		std::tuple<size_t, bool> Find(const RTTI& value);
		std::tuple<size_t, bool> Find(const Scope& value);

		const std::tuple<size_t, bool> Find(const std::int32_t& value) const;
		const std::tuple<size_t, bool> Find(const std::float_t& value) const;
		const std::tuple<size_t, bool> Find(const glm::vec4& value) const;
		const std::tuple<size_t, bool> Find(const glm::mat4& value) const;
		const std::tuple<size_t, bool> Find(const std::string& value) const;
		const std::tuple<size_t, bool> Find(const RTTI& value) const;
		const std::tuple<size_t, bool> Find(const Scope& value) const;

		Scope& operator[](std::size_t index);
		const Scope& operator[](std::size_t index) const;

	#pragma endregion Element Access:

	#pragma region Modifiers:

		// Setters
		void Set(const DatumTypes& dataType, std::size_t index = 0);
		void Set(const std::int32_t& value, std::size_t index = 0);
		void Set(const std::float_t& value, std::size_t index = 0);
		void Set(const glm::vec4& value, std::size_t index = 0);
		void Set(const glm::mat4& value, std::size_t index = 0);
		void Set(const std::string& value, std::size_t index = 0);
		void Set(RTTIPtr value, std::size_t index = 0);
		void Set(ScopePtr value, std::size_t index = 0);


		/// <summary>
		/// Sets the type of data this Datum stores
		/// </summary>
		/// <param name="dataType">DatumTypes enum value</param>
		/// <returns>bool whether the Type was set or not</returns>
		const bool SetType(const DatumTypes dataType);

		
		void SetStorage(const DatumTypes& dataType, std::size_t size);
		void SetStorage(std::int32_t* dataArr, std::size_t size);
		void SetStorage(std::float_t* dataArr, std::size_t size);
		void SetStorage(glm::vec4* dataArr, std::size_t size);
		void SetStorage(glm::mat4* dataArr, std::size_t size);
		void SetStorage(std::string* dataArr, std::size_t size);
		void SetStorage(RTTIPtr* dataArr, std::size_t size);
		void SetStorage(void* dataArr, std::size_t size);

		void SetFromString(const std::string& str, size_t index = 0);

		template<typename T>
		const std::string ToString(size_t index = 0);

		/// <summary>
		/// remove the last item from the list and destructs the item, memory is preserved
		/// </summary>
		void PopBack();

		/// <summary>
		/// append given item after the end of the list, if the Capacity has been reached, 
		/// Reserve is called to allocate more space
		/// </summary>
		/// <param name="data">item to be appended</param>
		void PushBack(const std::int32_t& value);
		void PushBack(const std::float_t& value);
		void PushBack(const glm::vec4& value);
		void PushBack(const glm::mat4& value);
		void PushBack(const std::string& value);
		void PushBack(RTTIPtr value);
		void PushBack(ScopePtr value);

		/// <summary>
		/// Remove the item associated with the given data and maintain list integrity. 
		/// Calling Remove on a non-existent item will not cause any harm.
		/// memory is preserved
		/// </summary>
		/// <param name="value">Value to search and remove from the list</param>
		template<typename T>
		void Remove(const T& value);

		template<typename T>
		void RemoveAt(size_t index);

		/// <summary>
		/// remove all items in the list, memory is preserved
		/// </summary>
		void Clear();

	#pragma endregion Modifiers:

	private:
	#pragma region Helpers:
		
		/// <summary>
		/// Helper function to check if the data types match, returns a bool
		/// </summary>
		template<typename T>
		const bool TypeCheck() const;

		/// <summary>
		/// Helper function to initalize the Data Type when it is unknown
		/// </summary>
		template<typename T>
		void InitialSetType();

		template<typename T>
		void PushBackHelper(const T& value);

		void InitPushBack(DatumTypes type);
		
		template<typename T>
		std::tuple<size_t, bool> FindHelper(const T& value);

		//template<typename T>
		//const std::tuple<size_t, bool> FindHelper(const T& value) const;

		/// <summary>
		/// Helper function to get the Data Reference at a certain index
		/// </summary>
		template<typename T>
		T& At(size_t index = 0);
		
		/// <summary>
		/// Helper function to get the Data Reference at a certain index (const)
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		template<typename T> 
		const T& At(size_t index = 0) const;

		template<typename T>
		T*& DataPointer();
		template<>
		Scope**& DataPointer<Scope*>();
		template<>
		RTTI**& DataPointer<RTTI*>();

		
		glm::vec4 ParseStringToVector(const std::string& str);

		glm::mat4 ParseStringToMatrix(const std::string& str);

	#pragma region Helpers:

		size_t mCapacity{ 0 };
		size_t mSize{ 0 };
		bool mIsInternalStorage{ true };

		DatumTypes mDataType{ DatumTypes::Unknown };
		DatumValue mData;

		IncrementFunctor mIncrementFunctor;

		static const size_t DatumSizes[static_cast<size_t>(DatumTypes::Max)];

	public:
		static const Hashmap<const std::string, Datum::DatumTypes> StringToDatumTypesMap;
		static const Hashmap<Datum::DatumTypes, const std::string> DatumTypesToStringMap;
	};
}

#include "Datum.inl"