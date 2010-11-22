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

//Synthetic header files:
#include "Auxiliary.hpp"
#include "WinException.hpp"

using namespace Synthetic;

HANDLE Aux::duplicateHandleLocal(HANDLE source)
{
	HANDLE dest;
	BOOL ec = DuplicateHandle(	GetCurrentProcess(),
										source,
										GetCurrentProcess(),
										&dest,
										0,
										FALSE,
										DUPLICATE_SAME_ACCESS);
	if(!ec)
	{
		DWORD errorCode = GetLastError();
		throw WinException(	"duplicateHandleLocal()",
									"DuplicateHandle()",
									errorCode);
	}

	return dest;
}