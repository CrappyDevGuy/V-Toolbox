//_Author : CrappyDevGuy, M12D09Y2020_//

#pragma once

#include <string>
#include <iostream>
#include <iomanip>

namespace VtLogHandler
{
	constexpr static std::uint8_t max_label = 25;
	constexpr static std::uint8_t max_nspace = 15;

	#ifndef VT_DEBUG_DEFINE
		inline void oStreamDebug(std::string projectnspace, std::string src, std::string msg) const {}
	#else
	  inline void oStreamDebug(std::string s, std::string src, std::string msg)
	  {
	  	std::string a{"[" + s + "]"};
	   	std::string b{"[" + src + "]"};
	   	std::cout << "\033[35;1m[DEBUG]\033[0m" << std::setw((max_label-18)+a.size()) << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
	  }
	#endif

  inline void oStream(std::string s, std::string src, std::string msg)
  {
  	std::string a{"[" + s + "]"};
   	std::string b{"[" + src + "]"};
   	std::cout << "\033[37;1m[INFO]\033[0m" << std::setw((max_label-17)+a.size()) << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }

	inline void oStreamError(std::string s, std::string src, std::string msg)
  {
  	std::string a{"[" + s + "]"};
   	std::string b{"[" + src + "]"};
   	std::cout << "\033[31;1m[ERROR]\033[0m" << std::setw((max_label-18)+a.size()) << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }
  
  inline void oStreamWarning(std::string s, std::string src, std::string msg)
  {
    std::string a{"[" + s + "]"};
    std::string b{"[" + src + "]"};
    std::cout << "\033[33;1m[WARNING]\033[0m" << std::setw((max_label-20)+a.size()) << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }
    
  inline void oStreamFatalError(std::string s, std::string src, std::string msg)
  {
  	std::string a{"[" + s + "]"};
   	std::string b{"[" + src + "]"};
   	std::cout << "\033[37;41m[FATAL_ERROR]\033[0m" << std::setw((max_label-24)+a.size()) << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }

	
}
