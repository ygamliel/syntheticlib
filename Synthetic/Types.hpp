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

//Synthetic Header files:
#include "Os.hpp"

namespace Synthetic
{
	//Signed integer types
	typedef char		int8_t;
	typedef short		int16_t;
	typedef int			int32_t;
	typedef long long	int64_t;

	//Unsigned integer types
	typedef unsigned char		uint8_t;
	typedef unsigned short		uint16_t;
	typedef unsigned int			uint32_t;
	typedef unsigned long long	uint64_t;

	//Other integer types
	typedef uint8_t	byte_t;
	typedef uint16_t	word_t;
	typedef uint32_t	dword_t;
	typedef uint64_t	qword_t;

	//Pointer types
	//typedef Os::ulongptr_t ulongptr_t;
	typedef dword_t			ptr32_t;
	typedef qword_t			ptr64_t;
	//typedef ulongptr_t		ptr_t;
	typedef DWORD_PTR			ptr_t;
	typedef ptr_t				address_t;
}

#endif //SYNTHETIC_PROCESS_TYPES_HPP

/******************
******* EOF *******
******************/