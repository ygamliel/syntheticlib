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

#ifndef SYNTHETIC_PROCESS_MODULE_HPP
#define SYNTHETIC_PROCESS_MODULE_HPP

//C++ Header files:
#include <string>

//Synthetic Header files:
#include "SysObjectIterator.hpp"
#include "Types.hpp"

namespace Synthetic
{
	/**
	* Class representing a Windows thread
	*/
	class Module
	{
		//ModuleManager needs to access private data when module is manually mapped
		friend class ModuleManager;

	public:

		typedef ModuleIterator iterator;

		/**********************************************************************
		***********************************************************************
		************************ PUBLIC MEMBER FUNCTIONS **********************
		***********************************************************************
		**********************************************************************/

		/**
		* Default constructor.
		*/
		Module();

		/**
		* Optional constructor.
		* Calls read().
		* @param mod A reference to a (filled) MODULEENTRY32W structure.
		*/
		Module(const MODULEENTRY32W& mod);

		/**
		* Reads data from a MODULEENTRY32W structure.
		* @param mod A reference to a (filled) MODULEENTRY32W structure.
		*/
		void read(const MODULEENTRY32W& mod);

		/**
		* Returns the address the module was loaded.
		* @return ptr_t The modules loadaddress.
		*/
		ptr_t getBaseAddress() const;

		/**
		* Returns the modules size.
		* @return size_t The modules size.
		*/
		size_t getSize() const;

		/**
		* Returns the modules name.
		* @return std::wstring The modules name.
		*/
		std::wstring getName() const;

		/**
		* Returns the modules path.
		* @return std::wstring The modules path.
		*/
		std::wstring getPath() const;

		/**
		* Determines if the module was manually mapped, so it isn't included
		* in the module list.
		* @return bool true if module was manually mapped, false otherwise.
		*/
		bool isManuallyMapped() const;

		/**
		* Determines if information was loaded using read() or the
		* optional constructor.
		* @return bool true if module was loaded, false otherwise
		*/
		bool isLoaded() const;
	
	private:

		/**********************************************************************
		***********************************************************************
		*********************** PRIVATE MEMBER VARIABLES **********************
		***********************************************************************
		**********************************************************************/

		ptr_t baseAddress_;
		size_t size_;
		std::wstring moduleName_;
		std::wstring modulePath_;

		bool isManuallyMapped_;
	};
}

#endif //SYNTHETIC_PROCESS_MODULE_HPP

/******************
******* EOF *******
******************/