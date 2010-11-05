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

#ifndef SYNTHETIC_PROCESS_TYPES_HPP
#define SYNTHETIC_PROCESS_TYPES_HPP

//Windows Header Files:
#include <Windows.h>

//C++ Header Files:
#include <string>
#include <vector>

namespace Synthetic
{
	//Signed integer types
	typedef char		int8_t;
	typedef short		int16_t;
	typedef int			int32_t;
	typedef __int64	int64_t;

	//Unsigned integer types
	typedef unsigned char		uint8_t;
	typedef unsigned short		uint16_t;
	typedef unsigned int			uint32_t;
	typedef unsigned __int64	uint64_t;

	//Other integer types
	typedef uint8_t	byte_t;
	typedef uint16_t	word_t;
	typedef uint32_t	dword_t;
	typedef uint64_t	qword_t;

	//Id types
	typedef DWORD	ProcessId;
	typedef LONG	ThreadId;

	//Address types
	typedef DWORD_PTR ptr_t;
	typedef ptr_t		address_t;

	//List types
	typedef std::vector<ProcessId> ProcessList;

	//Typedefs to remove the fugly "everything in capitals" of the WinAPI :)
	typedef CONTEXT	context_t;
	typedef HANDLE		handle_t;
}

#endif //SYNTHETIC_PROCESS_TYPES_HPP

/******************
******* EOF *******
******************/