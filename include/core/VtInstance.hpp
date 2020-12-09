//_Author : CrappyDevGuy, M12D08Y2020_//

#pragma once

#include "volk.h"
#include <utility>


class VtInstance
{

	public:
		VtInstance();
		VtInstance(const VtInstance&) = delete;
		~VtInstance();

		VtInstance& operator=(const VtInstance&) = delete;
		VtInstance& operator=(VtInstance&& other) noexcept;
		VtInstance(VtInstance&& other) noexcept;
			
	private:
		VkInstance m_instance = nullptr;
};
