#include <pe/core/rtti/RTTI2.hpp>

namespace pe::core::rtti
{
	namespace RTTI2
	{
		TypeManager& TypeManager::get()
		{
			static TypeManager instance{};
			return instance;
		}
	}
}
