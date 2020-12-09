//_Author : CrappyDevGuy, M12D08Y2020_//

#define VT_DEBUG_DEFINE
#include "core/io/VtLogHandler.hpp"

#include "core/VtInstance.hpp"

#include <iostream>
#include <iomanip>

int main()
{
  VtLogHandler::oStream("myproject", "main", "Starting");

	VtInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.appName    = "MainInstance";
	instanceCreateInfo.appVersion = VK_MAKE_VERSION(0,0,1);

	VtInstance instance{instanceCreateInfo};


	VtLogHandler::oStream("myproject", "main", "Ending");
	return 0;
}
