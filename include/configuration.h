#pragma once
//This file os only needed for building the library


//Single define for window platform
#if defined(_WIN32)
	#if defined(_WIN64)
		#define AND_WINDOWS
	#else
		#error "Build for x86 is not supported!"
	#endif 
#endif 

//Chech if the api is avaliable in the platform
#if defined(AND_WINDOWS)
	#if !defined(AND_OPENGL)
		#error "A graphics api must be defined! (AND_OPENGL)"
	#endif
#endif