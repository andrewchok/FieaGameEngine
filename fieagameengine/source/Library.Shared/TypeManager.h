#pragma once
#include "hashmap.h"
#include "Datum.h"
#include "Signature.h"

namespace Library
{
	class TypeManager final
	{
	public:
		TypeManager(const TypeManager& rhs) = delete;
		TypeManager(TypeManager&& rhs) = delete;
		TypeManager& operator=(const TypeManager& rhs) = delete;
		TypeManager& operator=(TypeManager&& rhs) = delete;
		~TypeManager() = default;
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="typeId"></param>
		/// <param name="signatures"></param>
		static void RegisterType(const std::size_t& typeId, const Vector<Signature>& signatures);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="typeId"></param>
		/// <param name="signatures"></param>
		static void RegisterType(const std::size_t& typeId, Vector<Signature>&& signatures);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="typeId"></param>
		static void RemoveType(std::size_t& typeId);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="typeId"></param>
		/// <returns></returns>
		static const Vector<Signature>& GetSignatures(const std::size_t& typeId);

		/// <summary>
		/// 
		/// </summary>
		void Clear();

	private:
		TypeManager() = default;

		// todo: make size_t >>> RTTI:IdType
		static Hashmap<std::size_t, Vector<Signature>> mHashSignatureTable;
	};
}