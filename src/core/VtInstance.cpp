//_Author : CrappyDevGuy, M12D08Y2020_//

#include "core/VtInstance.hpp"

VtInstance& VtInstance::operator=(VtInstance&& other) noexcept
{
	std::swap(m_instance, other.m_instance);
	return *this;
}

VtInstance::VtInstance(VtInstance&& other) noexcept
:m_instance{std::move(other.m_instance)}
{}

VtInstance::VtInstance()
{
	
}

VtInstance::~VtInstance()
{
	
}
