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

//Synthetic Header Files:
#include "SysObjectIterator.hpp"

using namespace Synthetic;

template class SysObjectIterator<	PROCESSENTRY32W,
												Process32FirstW,
												Process32NextW,
												TH32CS_SNAPPROCESS>;

template class SysObjectIterator<	THREADENTRY32,
												Thread32First,
												Thread32Next,
												TH32CS_SNAPTHREAD>;

template class SysObjectIterator<	MODULEENTRY32W,
												Module32FirstW,
												Module32NextW,
												TH32CS_SNAPMODULE>;

template class SysObjectIterator<	HEAPLIST32,
												Heap32ListFirst,
												Heap32ListNext,
												TH32CS_SNAPHEAPLIST>;

/******************
******* EOF *******
******************/