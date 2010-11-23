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

#ifndef SYNTHETIC_PROCESS_SYSTEM_HPP
#define SYNTHETIC_PROCESS_SYSTEM_HPP

#if defined(linux)
	#define SYNTHETIC_ISLINUX
#elif defined(WIN32)
	#define SYNTHETIC_ISWINDOWS
	#define SYNTHETIC_ISWIN32
#else
	#error [Synthetic] Unsupported platform
#endif

#if defined(_M_X64) || defined(__LP64__)
	#define SYNTHETIC_ISX64
#elif defined(_M_IX86) || defined(__i386__)
	#define SYNTHETIC_ISX86
#else
	#error [Synthetic] Unsupported platform and/or compiler
#endif

#if defined(SYNTHETIC_ISX64)
	#define SYNTHETIC_IS64BIT
	#define SYNTHETIC_PLATFORMBITS 64
#elif defined(SYNTHETIC_ISX86)
	#define SYNTHETIC_IS32BIT
	#define SYNTHETIC_PLATFORMBITS 32
#else
	#error [Synthetic] Unsupported platform and/or compiler
#endif

#endif //SYNTHETIC_PROCESS_SYSTEM_HPP

/******************
******* EOF *******
******************/