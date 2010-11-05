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
#include "Module.hpp"

using namespace std;
using namespace Synthetic;

/**********************************************************************
***********************************************************************
************************ PUBLIC MEMBER FUNCTIONS **********************
***********************************************************************
**********************************************************************/

Module::Module(const MODULEENTRY32W& mod)
{
	read(mod);
}

Module::Module()
{ }

void Module::read(const MODULEENTRY32W& mod)
{
	baseAddress_ = reinterpret_cast<address_t>(mod.modBaseAddr);
	size_ = mod.modBaseSize;
	moduleName_.assign(mod.szModule);
	modulePath_.assign(mod.szExePath);
	isManuallyMapped_ = false;
}

address_t Module::getBaseAddress() const
{
	return baseAddress_;
}

size_t Module::getSize() const
{
	return size_;
}

wstring Module::getName() const
{
	return moduleName_;
}

wstring Module::getPath() const
{
	return modulePath_;
}

bool Module::isManuallyMapped() const
{
	return isManuallyMapped_;
}

/******************
******* EOF *******
******************/