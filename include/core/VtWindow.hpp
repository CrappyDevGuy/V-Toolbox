//_Author : CrappyDevGuy, M12D28Y2020_//

#pragma once

#include "core/VtInstance.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

struct VtWindowCreateInfo
{
	std::string name{"NULL"};
	glm::ivec2  extent{800, 600};

	std::vector<glm::ivec2> windowhints;

	VtInstance* pVtInstance{nullptr};
};

class VtWindow
{
	struct InternalData
	{
		std::string name;
		glm::ivec2  extent;
		bool 				resize;

		glm::ivec2 mousePosition;

	};

	public:
		VtWindow(VtWindowCreateInfo _createInfo);
		VtWindow(const VtWindow&) = delete;
		VtWindow() = default;
		~VtWindow();

		VtWindow& operator=(const VtWindow&) = delete;
		VtWindow& operator=(VtWindow&& other) noexcept;
		VtWindow(VtWindow&& other) noexcept;

		void update();

		inline void setResize(bool value) noexcept { m_data.resize = value;                  };
    inline bool hasBeenResized()      noexcept { return m_data.resize;                   };
    inline bool shouldClose()         noexcept { return glfwWindowShouldClose(m_window); };

    inline GLFWwindow*   getWindowInstance()  noexcept { return m_window;  };
    inline VkSurfaceKHR& getSurfaceInstance() noexcept { return m_surface; };

    inline glm::ivec2    getMousePosition()   noexcept { return m_data.mousePosition; };
    inline glm::ivec2    getExtent()          noexcept { return m_data.extent;        };

	private:
		VtWindow::InternalData m_data{};
		
    GLFWwindow*  m_window{nullptr};
    VkSurfaceKHR m_surface{nullptr};
    VtInstance*  m_pVtInstance{nullptr};
		
};
