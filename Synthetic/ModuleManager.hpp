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

#ifndef SYNTHETIC_PROCESS_MODULEMANAGER_HPP
#define SYNTHETIC_PROCESS_MODULEMANAGER_HPP

//Synthetic Header Files:
#include "Process.hpp"
#include "Module.hpp"

namespace Synthetic
{
	/**
	* Auxiliary class to offering access to a process' modules.\n
	* Becomes invalid as soon as Process reference becomes invalid.n
	*/
	class ModuleManager
	{
	public:

		/**********************************************************************
		***********************************************************************
		************************ PUBLIC MEMBER FUNCTIONS **********************
		***********************************************************************
		**********************************************************************/

		/**
		* Default Constructor.
		* @param proc A reference to a process object which modules.
		* should be managed. Has to be valid the whole lifetime.
		*/
		ModuleManager(Process& proc);

		/**
		* Retrieves all loaded modules.
		* @param dest Reference to vector to hold all found objects.
		* @return size_t Number of found modules.
		*/
		size_t getAllModules(std::vector<Module>& dest) const;

		/**
		* Retrieves all manually mapped Modules.
		* @return const std::vector<Module>& A read-only reference to
		* the internal list containing manually mapped modules.
		*/
		const std::vector<Module>& getManuallyMappedList() const;

		/**
		* Attempts to return a module by its name.
		* @param moduleName The modules name.
		* @return Module A module object. If nothing was found, baseaddress
		* will be set to 0.
		*/
		Module getModuleByName(std::wstring moduleName) const;

		/**
		* Attempts to return a module by its path.
		* @param modulePath The modules path.
		* @return Module A module object. If nothing was found, baseaddress will
		*be set to 0.
		*/
		Module getModuleByPath(std::wstring modulePath) const;

		/**
		* Injects a module into the process.
		* @param dllPath The modules path.
		* @return Module The injected module.
		*/
		Module injectModule(const std::wstring& dllPath) const;

		/**
		* Ejects a module from process.
		* @param mod The module to eject.
		*/
		void ejectModule(Module& mod);

		/**
		* Calls a loaded modules export.
		* @param mod The module which has the export.
		* @param exportName The name of the export to be called.
		* @param param A pointer to be passed to the export.
		* @return The exports return value.
		*/
		dword_t callModuleExport(	const Module& mod,
											const std::string& exportName,
											address_t param) const;

		/**
		* Retrieve an exports address.
		* @param mod Module which should be queried.
		* @param exportName Name of the export to search for.
		* @return address_t Address of the found export.
		*/
		address_t getModuleExportAddress(	const Module& mod,
														const std::string& exportName) const;


	private:

		/**********************************************************************
		***********************************************************************
		*********************** PRIVATE MEMBER VARIABLES **********************
		***********************************************************************
		**********************************************************************/

		std::vector<Module> manuallyMappedModules_;
		Process& proc_;
	};

}

#endif //SYNTHETIC_PROCESS_MODULEMANAGER_HPP

/******************
******* EOF *******
******************/