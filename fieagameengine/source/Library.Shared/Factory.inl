#include "Factory.h"

namespace Library
{
	template <typename AbstractProductT>
	Hashmap<const std::string, Factory<AbstractProductT>*> Factory<AbstractProductT>::mConcreteFactoryTable;

	template <typename AbstractProductT>
	inline const Factory<AbstractProductT>* Factory<AbstractProductT>::Find(const std::string& className)
	{
		auto it = mConcreteFactoryTable.Find(className);
		if (it == mConcreteFactoryTable.end()) 
		{
			return nullptr;
		}

		return (*it).second;
	}

	template<typename AbstractProductT>
	inline gsl::owner<AbstractProductT*> Factory<AbstractProductT>::Create(const std::string& className)
	{
		return mConcreteFactoryTable.At(className)->Create();
	}


	template<typename AbstractProductT>
	inline void Factory<AbstractProductT>::Add(Factory& concreteFactory)
	{
		mConcreteFactoryTable.Insert(std::make_pair( concreteFactory.ClassName(), &concreteFactory ));
	}

	template<typename AbstractProductT>
	inline void Factory<AbstractProductT>::Remove(Factory& concreteFactory)
	{
		mConcreteFactoryTable.Remove(concreteFactory.ClassName());
	}


}