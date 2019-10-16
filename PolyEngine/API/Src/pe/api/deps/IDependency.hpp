#pragma once

#include <pe/Defines.hpp>

namespace pe::api::deps {

class API_DLLEXPORT IDependency : public ::pe::core::BaseObject<>
{   
public:

	virtual std::type_index getType() const = 0;
};

}