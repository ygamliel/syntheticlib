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

//C++ header files:
#include <string>
#include <algorithm>

//Synthetic header Files:
#include "ModuleManager.hpp"
#include "Allocator.hpp"
#include "WinException.hpp"
#include "ThreadManager.hpp"
#include "SmartType.hpp"

using namespace std;
using namespace Synthetic;

/**********************************************************************
***********************************************************************
************************ PUBLIC MEMBER FUNCTIONS **********************
***********************************************************************
**********************************************************************/

ModuleManager::ModuleManager(Process& proc) : proc_(proc)
{ }

const vector<Module>& ModuleManager::getManuallyMappedList() const
{
	return manuallyMappedModules_;
}

Module ModuleManager::getModuleByName(wstring moduleName) const
{
	//Convert given name to lowercase
	transform(	moduleName.begin(), moduleName.end(),
					moduleName.begin(), towlower);

	//Iterate modulelist
	for(Module::iterator it(proc_.getId()); it != Module::iterator(); ++it)
	{
		wstring curName(it->szModule);

		//Convert  to lowercase
		transform(	curName.begin(), curName.end(),
						curName.begin(), towlower);

		if(curName == moduleName)
			return Module(*it);
	}

	//Return invalid module in case nothing got found
	Module fail;
	fail.baseAddress_ = 0;
	return fail;
}

Module ModuleManager::getModuleByPath(wstring modulePath) const
{
	//Convert given path to lowercase
	transform(	modulePath.begin(), modulePath.end(),
					modulePath.begin(), towlower);

	//Iterate modulelist
	for(Module::iterator it(proc_.getId()); it != Module::iterator(); ++it)
	{
		wstring curPath(it->szExePath);

		//Convert  to lowercase
		transform(	curPath.begin(), curPath.end(),
						curPath.begin(), towlower);

		if(curPath == modulePath)
			return Module(*it);
	}

	//Return invalid module in case nothing got found
	Module fail;
	fail.baseAddress_ = 0;
	return fail;
}

Module ModuleManager::injectModule(const wstring& dllPath) const
{
	//Write path into targets memory
	ScopedAllocator allocator(proc_);
	ptr_t mem = allocator.allocate<wchar_t>(dllPath.length() + 1);
	proc_.writeString<wchar_t>(mem, dllPath);

	//Fetch kernel32.dll
	Module kernel32 = getModuleByName(L"kernel32.dll");

	//Call LoadLibraryW and validate return value
	dword_t ec = callModuleExport(kernel32, "LoadLibraryW", mem);
	if(!ec)
	{
		throw std::runtime_error(	"Process::inject() Error : "\
											"LoadLibraryW() in remote process failed");
	}

	return getModuleByPath(dllPath);
}

void ModuleManager::ejectModule(Module& mod)
{
	//Check if we have to eject
	if(!mod.getBaseAddress())
		return;

	//Fetch kernel32.dll
	Module kernel32 = getModuleByName(L"kernel32.dll");

	//Call FreeLibrary and validate return value
	dword_t ec = callModuleExport(kernel32, "FreeLibrary", mod.getBaseAddress());
	if(!ec)
	{
		throw std::runtime_error(	"Process::eject() Error : "\
											"FreeLibrary() in remote process failed");
	}

	//Invalidate module
	mod.baseAddress_ = 0;
	mod.isManuallyMapped_ = false;
	mod.moduleName_ = L"";
	mod.modulePath_ = L"";
	mod.size_ = 0;
}

dword_t ModuleManager::callModuleExport(	const Module& mod,
														const string& exportName,
														ptr_t param) const
{
	//Fetch export address
	ptr_t exportAddress = getModuleExportAddress(mod, exportName);

	//Create a thread
	ThreadManager threads(proc_);
	Thread thread = threads.createThread(	exportAddress,
														param,
														false,
														INFINITE);
	return thread.getExitCode();
}

ptr_t ModuleManager::getModuleExportAddress(	const Module& mod,
																const string& exportName) const
{
	//Load module as data for local read
	SmartModule module = LoadLibraryExW(	mod.getPath().c_str(),
														NULL,
														DONT_RESOLVE_DLL_REFERENCES);

	if(!module)
	{
		throw WinException(	"Module::getExportAddress()",
									"LoadLibraryExW()",
									GetLastError());
	}

	//Retrieve exported address
	FARPROC exportAddressTemp = GetProcAddress(module, exportName.c_str());
	ptr_t exportAddress = reinterpret_cast<ptr_t>(exportAddressTemp);
	if (!exportAddress)
	{
		throw WinException(	"Module::getExportAddress()",
									"GetProcAddress()",
									GetLastError());
	}

	//Calculate offset, add it to the module address and return
	ptr_t offset = exportAddress - reinterpret_cast<ptr_t>(module.get());
	return mod.getBaseAddress() + offset;
}

/******************
******* EOF *******
******************/