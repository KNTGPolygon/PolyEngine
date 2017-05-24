#pragma once

#include <Core.hpp>
#include "Entity.hpp"

namespace Poly {

	enum class eComponentBaseFlags
	{
		NONE = 0x00,
		ABOUT_TO_BE_CREATED = 0x01,
		NEWLY_CREATED = 0x02,
		ABOUT_TO_BE_REMOVED = 0x04,
	};

	class ENGINE_DLLEXPORT ComponentBase : public BaseObject<>
	{
	friend class World;
	public:
		//////////////////////////////
		/// Getter for a component of a specified type that shares UniqueID with this one.
		/// @tparam T is a type of a component to get.
		/// @return pointer to a component of a specified type or a nullptr, if it does not exist.
		//////////////////////////////
		template<typename T>
		T* GetSibling()
		{
			HEAVY_ASSERTE(Owner, "Component was not properly initialized.");
			return Owner->GetComponent<T>();
		}

		void SetFlags(const EnumFlags<eComponentBaseFlags>& rhs) { Flags = rhs; }
		void ClearFlags() { Flags = eComponentBaseFlags::NONE; }
		const EnumFlags<eComponentBaseFlags>& GetFlags() { return Flags; }

	private:
		Entity* Owner = nullptr;

		EnumFlags<eComponentBaseFlags> Flags = eComponentBaseFlags::ABOUT_TO_BE_CREATED;
	};
}
