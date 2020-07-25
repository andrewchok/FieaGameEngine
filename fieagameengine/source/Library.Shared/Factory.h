#pragma once

#include "hashmap.h"
#include <gsl/gsl>

namespace Library
{
	/// <summary>
	/// Templated Class using the Factory Design Pattern
	/// </summary>
	template<typename AbstractProductT>
	class Factory
	{
	public:
		/// <summary>
		/// Defaul Constructor
		/// </summary>
		Factory() = default;
		/// <summary>
		/// Copy Constructor (deleted)
		/// </summary>
		Factory(const Factory& rhs) = delete;
		/// <summary>
		/// Move Constructor (deleted)
		/// </summary>
		Factory(Factory&& rhs) noexcept = delete;
		/// <summary>
		/// Copy Assignment (deleted)
		/// </summary>
		Factory& operator=(const Factory& rhs) = delete;
		/// <summary>
		/// Move Assignment  (deleted)
		/// </summary>
		Factory& operator=(Factory&& rhs) noexcept = delete;
		/// <summary>
		/// Virtual Destructor
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// (static) Given a class name (string), return the associated concrete factory
		/// </summary>
		/// <param name="className">class name (string)</param>
		/// <returns>associated concrete factory</returns>
		static const Factory<AbstractProductT>* Find(const std::string& className);

		/// <summary>
		/// (static) Given a class name (string), return a new object of that type.
		/// </summary>
		/// <param name="className">class name (string)</param>
		/// <returns>new object of that type</returns>
		static gsl::owner<AbstractProductT*> Create(const std::string& className);
		

	protected:
		/// <summary>
		/// Return the address of a concrete product associated with this concrete factory class.
		/// </summary>
		/// <returns>address of a concrete product</returns>
		virtual gsl::owner<AbstractProductT*> Create() = 0;

		/// <summary>
		/// Return a string representing the name of the class the factory instantiates.
		/// </summary>
		/// <returns>name of the class</returns>
		virtual const std::string ClassName() const = 0;

		/// <summary>
		/// Given a reference to a concrete factory, add it to the list of factories for this abstract factory
		/// </summary>
		/// <param name="concreteFactory">reference to a concrete factory</param>
		static void Add(Factory& concreteFactory);

		/// <summary>
		/// Given a reference to a concrete factory, remove it from the list of factories for this abstract factory. 
		/// </summary>
		/// <param name="concreteFactory">reference to a concrete factory</param>
		static void Remove(Factory& concreteFactory);
		
	private:
		static Hashmap<const std::string, Factory<AbstractProductT>*> mConcreteFactoryTable;
	};

	/// <summary>
	/// perform everything needed to create a concrete factory for a given product in a Macro
	/// </summary>
#define ConcreteFactory(ConcreteProductT, AbstractProductT)								\
	class ConcreteProductT##Factory final : public Factory<AbstractProductT>			\
	{																					\
	public:																				\
		ConcreteProductT##Factory()														\
		{																				\
			Add(*this);																	\
		}																				\
		~ConcreteProductT##Factory()													\
		{																				\
			Remove(*this);																\
		}																				\
		virtual gsl::owner<AbstractProductT*> Create() override							\
		{																				\
			return new ConcreteProductT();												\
		}																				\
		virtual const std::string ClassName() const override							\
		{																				\
			return (#ConcreteProductT);													\
		}																				\
	};
}

#include "Factory.inl"