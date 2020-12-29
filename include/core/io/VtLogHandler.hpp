//_Author : CrappyDevGuy, M12D09Y2020_//

//#define VT_DEBUG_DEFINE

#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>

namespace VtLogHandler
{
	constexpr static std::uint8_t max_label = 24;
	constexpr static std::uint8_t max_nspace = 15;

	#ifndef VT_DEBUG_DEFINE
		inline void oStreamDebug(std::string s, std::string src, std::string msg) {}
	#else
	  inline void oStreamDebug(std::string s, std::string src, std::string msg) noexcept
	  {
	  	std::string a{"[" + s + "]"};
	   	std::string b{"[" + src + "]"};
	   	std::cout << "\033[37;45m[DEBUG]\033" << std::setw((max_label-23)+a.size()) << "\033[0m" << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
	  }
	#endif

  inline void oStream(std::string s, std::string src, std::string msg) noexcept
  {
  	std::string a{"[" + s + "]"};
   	std::string b{"[" + src + "]"};
   	std::cout << "\033[37;0m[INFO]\033[0m" << std::setw((max_label-16)+a.size()) << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }

	inline void oStreamError(std::string s, std::string src, std::string msg) noexcept
  {
  	std::string a{"[" + s + "]"};
   	std::string b{"[" + src + "]"};
   	std::cout << "\033[37;41m[ERROR]\033" << std::setw((max_label-23)+a.size()) << "\033[0m" << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }
  
  inline void oStreamWarning(std::string s, std::string src, std::string msg) noexcept
  {
    std::string a{"[" + s + "]"};
    std::string b{"[" + src + "]"};
    std::cout << "\033[30;43m[WARNING]\033" << std::setw((max_label-25)+a.size()) << "\033[0m" << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
  }
    
  inline void oStreamFatalError(std::string s, std::string src, std::string msg)
  {
  	std::string a{"[" + s + "]"};
   	std::string b{"[" + src + "]"};
   	std::cout << "\033[37;41m[FATAL_ERROR]\033" << std::setw((max_label-29)+a.size()) << "\033[0m" << a << std::setw((max_nspace-a.size())+b.size()) << b << " : " << msg << '\n'; 
   	throw std::runtime_error(msg);
  }

	
}
