#pragma once

#include "RTTI.h"
#include "vector.h"
#include "hashmap.h"
#include "Datum.h"
#include <gsl/gsl>

namespace Library
{
	class Scope : public Library::RTTI
	{
		RTTI_DECLARATIONS(Scope, Library::RTTI)		

	public:
		using HashTable = Hashmap<std::string, Datum>;
		using HashTablePair = HashTable::PairType;
		using OrderedVector = Vector<HashTablePair*>;

	#pragma region Constructors, Assignments & Destructor:

		/// <summary>
		/// Default Constructor
		/// </summary>
		explicit Scope(std::size_t capacity = 0);
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Takes in a const Scope reference</param>
		Scope(const Scope& rhs);
		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Takes in a Scope&&</param>
		/// <returns>Moved Scope</returns>
		Scope(Scope&& rhs) noexcept;
		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Takes in a const Scope reference</param>
		/// <returns>Copied Scope</returns>
		Scope& operator=(const Scope& rhs);
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Takes in a Scope&&</param>
		/// <returns>Moved Scope</returns>
		Scope& operator=(Scope&& rhs) noexcept;
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Scope();

	#pragma endregion Constructors, Assignments & Destructor:

	#pragma region Equality:

		/// <summary>
		/// Equal Operator
		/// </summary>
		/// <param name="rhs">takes in a rhs</param>
		/// <returns>bool</returns>
		bool operator == (const Scope& rhs) const;
		/// <summary>
		/// Not Equal Operator
		/// </summary>
		/// <param name="rhs">takes in a rhs</param>
		/// <returns>bool</returns>
		bool operator != (const Scope& rhs) const;
		
		bool Equals(const RTTI* rhs) const override;
	#pragma endregion Equality:

	#pragma region Element Access:
		
		/// <summary>
		/// which takes an unsigned integer and which returns a reference to a Datum at the given index.
		/// The index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">index of Vector array</param>
		/// <returns>Datum reference at index</returns>
		Datum& operator [] (const size_t index);
		/// <summary>
		/// which takes an unsigned integer and which returns a reference to a Datum at the given index.
		/// The index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">index of Vector array</param>
		/// <returns>const Datum reference at index</returns>
		const Datum& operator [] (const size_t index) const;
		/// <summary>
		/// which takes a constant string and which wraps Append, for syntactic convenience.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>Datum reference</returns>
		Datum& operator [] (const std::string& key);

		const Datum& operator[](const std::string& key) const;

		/// <summary>
		/// which takes a constant string and returns the address of a Datum. 
		/// This should return the address of the Datum associated with the given name in this Scope, 
		/// if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>return the address of the Datum if exists, otherwise nullptr</returns>
		virtual Datum* Find(const std::string& key);
		/// <summary>
		/// which takes a constant string and returns the address of a Datum. 
		/// This should return the address of the Datum associated with the given name in this Scope, 
		/// if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>return the const address of the Datum if exists, otherwise nullptr</returns>
		virtual const Datum* Find(const std::string& key) const;

		/// <summary>
		/// which takes the constant address of a Scope and returns the Datum pointer and index at which the Scope was found.
		/// </summary>
		/// <param name="scope">address of Parent Scope</param>
		/// <returns>returns the Datum pointer and index at which the Scope was found</returns>
		std::tuple<Datum*, size_t> FindContainedScope(const Scope& scope);

		/// <summary>
		/// which takes a constant string, 
		/// and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, 
		/// if it exists, and nullptr otherwise. 
		/// The Scope pointer variable, if provided, shall contain the address of the Scope object which contains the match.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>returns the address of a Datum</returns>
		Datum* Search(const std::string& key);
		/// <summary>
		/// which takes a constant string, 
		/// and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, 
		/// if it exists, and nullptr otherwise. 
		/// The Scope pointer variable, if provided, shall contain the address of the Scope object which contains the match.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>returns the const address of a Datum</returns>
		const Datum* Search(const std::string& key) const;
		/// <summary>
		/// which takes a constant string and the address of a Scope*& pointer variable, 
		/// and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, 
		/// if it exists, and nullptr otherwise. 
		/// The Scope pointer variable, if provided, shall contain the address of the Scope object which contains the match.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>returns the address of a Datum</returns>
		Datum* Search(const std::string& key, Scope*& foundScope);
		/// <summary>
		/// which takes a constant string and the address of a Scope*& pointer variable, 
		/// and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, 
		/// if it exists, and nullptr otherwise. 
		/// The Scope pointer variable, if provided, shall contain the address of the Scope object which contains the match.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>returns the const address of a Datum</returns>
		const Datum* Search(const std::string& key, Scope*& foundScope) const;

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
	#pragma endregion Element Access:
		
	#pragma region Modifers:

		/// <summary>
		/// takes a constant string and returns a reference to a Datum.  
		/// Note that the order in which values are appended must be preserved. 
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>
		/// return a reference to a Datum with the associated name. 
		/// If it already exists, return that one, otherwise create one.
		/// </returns>
		Datum& Append(const std::string& key);
		Datum& Append(const std::string& key, const Datum& datum);
		/// <summary>
		/// This should return a reference to a Scope with the associated name.  
		/// If a Datum already exists at that key reuse it (and append to it a new Scope), 
		/// otherwise create a new Datum. 
		/// Note that creating one implicitly means that a Datum will be created within this scope, 
		/// that its type is ”Table”, and that its value is assigned to the address of the created Scope. 
		/// Also, the created Scope shall refer to its parent, such that Search operates as specified.
		/// </summary>
		/// <param name="key">constant string key</param>
		/// <returns>returns a reference to Scope.</returns>
		Scope& AppendScope(const std::string& key);

		/// <summary>
		/// which takes a reference to a Scope (the child to adopt), 
		/// a string (the name of key for the Datum to use for storing the child).
		/// </summary>
		/// <param name="child">reference to a Scope</param>
		/// <param name="key">a string </param>
		void Adopt(Scope& child, const std::string& key);
		/// <summary>
		///  which returns the address of the Scope which contains this one
		/// </summary>
		/// <returns>address of Parent Scope</returns>
		Scope* GetParent() const;

		
		/// <summary>
		/// remove all items in the list, memory is preserved
		/// </summary>
		void Clear();

	#pragma endregion Modifers:

	#pragma region Helpers:

		virtual gsl::owner<Scope*> Clone() const;

	private:
		/// <summary>
		/// Helper method to Find a pair using the given key, locally
		/// </summary>
		/// <param name="key">std::string key used to find</param>
		/// <returns>Pointer to Datum if found, nullptr if not found</returns>
		const Datum* FindHelper(const std::string& key) const;
		/// <summary>
		/// Helper Seacrch to look for Pair with key, hierarchically upwards
		/// also returns the scope that closest that contains key
		/// </summary>
		/// <param name="key">std::string key used to search</param>
		/// <param name="foundScope">Scope*& of the scope</param>
		/// <returns>Pointer to Datum if found, nullptr if not found</returns>
		const Datum* SearchHelper(const std::string& key, Scope** foundScope = nullptr) const;
				
	#pragma endregion Helpers:
	public:
		/// <summary>
		/// Removes self from parent if is a child of one and tells parent to do the same
		/// </summary>
		/// <returns>returns pointer to scope of self</returns>
		Scope * OrphanSelf();

	protected:
		const OrderedVector& GetOrderedVector() const;


	private:

		Scope* mParent{ nullptr };

		OrderedVector mOrderedVector;
		HashTable mHashTable;
	};
}

#include "Scope.inl"