//_Author : CrappyDevGuy, M12D08Y2020_//

#define VT_DEBUG_DEFINE
#include "core/io/VtLogHandler.hpp"

#include <iostream>
#include <iomanip>

int main()
{

	VtLogHandler::oStream("myproject", "main::main", "Starting");

	VtLogHandler::oStream("myproject", "main::main", "Ending");

	return 0;
}
