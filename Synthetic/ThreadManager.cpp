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
#include "ThreadManager.hpp"
#include "SmartType.hpp"

using namespace std;
using namespace Synthetic;

/**********************************************************************
***********************************************************************
************************ PUBLIC MEMBER FUNCTIONS **********************
***********************************************************************
**********************************************************************/

ThreadManager::ThreadManager(Process& proc) : proc_(proc)
{ }

size_t ThreadManager::getAllThreads(std::vector<Thread>& dest) const
{
	size_t previousSize = dest.size();

	for(Thread::iterator it(proc_.getId()); it != Thread::iterator(); ++it)
		dest.push_back(Thread(it->th32ThreadID));

	return dest.size() - previousSize;
}

Thread ThreadManager::createThread(	address_t procedure, address_t param,
												bool suspended, dword_t waitingTime) const
{
	//Prepare Arguments
	LPTHREAD_START_ROUTINE procAddress;
	procAddress = reinterpret_cast<LPTHREAD_START_ROUTINE>(procedure);

	//Create the thread
	DWORD creationFlags = suspended ? CREATE_SUSPENDED : 0;
	DWORD id;

	SmartHandle createdThread;
	createdThread = CreateRemoteThread(	proc_.getHandle(),
													NULL,
													0,
													procAddress,
													reinterpret_cast<LPVOID>(param),
													creationFlags,
													&id);

	if(!createdThread)
	{
		throw WinException(	"ThreadManager::createThread<>()",
									"CreateRemoteThread()",
									GetLastError());
	}

	Thread newThread(id);

	//Wait if desired and return
	if(waitingTime)
		newThread.wait(waitingTime);

	return newThread;
}