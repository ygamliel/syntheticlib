/*
	This file is part of the Synthetic library.
	Synthetic is a little library for writing custom gamecheats etc

	Synthetic is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Synthetic is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Synthetic.  If not, see <http://www.gnu.org/licenses/>.

	Copyright (C) [2010] [Ethon <Ethon@list.ru>]
*/

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
	#pragma once
#endif

#ifndef SYNTHETIC_PROCESS_OS_HPP
#define SYNTHETIC_PROCESS_OS_HPP

#include "System.hpp"

#if defined(SYNTHETIC_ISWINDOWS)
	
	//Include platform-independent header files for Windows here

	#if defined(SYNTHETIC_IS32BIT)

		//Include 32bit header files for Windows here

	#elif defined(SYNTHETIC_IS64 BIT)

		//Include 64bit header files for Windows here

	#endif

#elif defined(SYNTHETIC_ISLINUX)

	//Include platform-independent header files for Linux here

	#if defined(SYNTHETIC_IS32BIT)

		//Include 32bit header files for Linux here

	#elif defined(SYNTHETIC_IS64 BIT)

		//Include 64bit header files for Linux here

	#endif

#endif


#endif //SYNTHETIC_PROCESS_OS_HPP