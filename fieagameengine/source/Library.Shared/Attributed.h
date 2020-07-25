#pragma once

#include "Scope.h"

namespace Library
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:
		Attributed() = delete;
		Attributed(const Attributed& rhs);
		Attributed(Attributed&& rhs) noexcept;
		Attributed& operator=(const Attributed& rhs);
		Attributed& operator=(Attributed&& rhs) noexcept;
		virtual ~Attributed() = default;

#pragma region Equality:

		bool operator == (const Attributed& rhs) const;
		bool operator != (const Attributed& rhs) const;

		bool Equals(const RTTI* rhs) const override;
#pragma endregion Equality:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool IsAttribute(const std::string& name) const;
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool IsPrescribedAttribute(const std::string& name) const;
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);
		//Datum& AppendAuxiliaryAttribute(const std::string& name, const Datum& datum);
		Datum& AppendAuxiliaryAttribute(const HashTablePair& pair);
		//Datum& AppendAuxiliaryAttribute(PairType&& pair);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const OrderedVector& GetAttributes() const;
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		OrderedVector GetPrescribedAttributes() const;
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		OrderedVector GetAuxiliaryAttributes() const;


		using AuxiliaryAttributeFunction = std::function<void(const Scope::HashTablePair&)>;
		void ForEachAuxiliaryAttribute(AuxiliaryAttributeFunction func) const;
		
	protected:

		//todo: change std::typeId >>> RTTI:IdType typeId
		explicit Attributed(std::size_t typeId);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="typeId"></param>
		void Populate(std::size_t typeId);

		void UpdateExternalStorage(std::size_t typeId);
	};
}