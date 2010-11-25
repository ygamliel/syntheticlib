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

#ifndef SYNTHETIC_PROCESS_THREADMANAGER_HPP
#define SYNTHETIC_PROCESS_THREADMANAGER_HPP

//C++ header files:
#include <vector>

//Synthetic header Files:
#include "Process.hpp"
#include "Thread.hpp"
#include "SysObjectIterator.hpp"

namespace Synthetic
{
	/**
	* Auxiliary class to offering access to a process' threads\n
	* Becomes invalid as soon as Process reference becomes invalid\n
	*/
	class ThreadManager
	{
	public:

		/**********************************************************************
		***********************************************************************
		************************ PUBLIC MEMBER FUNCTIONS **********************
		***********************************************************************
		**********************************************************************/

		/**
		*Default Constructor
		*@param proc A reference to a process object which threads
		*should be managed. Has to be valid the whole lifetime
		*/
		explicit ThreadManager(Process& proc);

		/**
		* Returns all threads running in the specified thread
		* @param dest A reference to a vector to store all objects
		* @return Number of found threads
		*/
		 size_t getAllThreads(std::vector<Thread>& dest) const;

		/**
		*Creates a new thread
		*@param procedure The procedure's address to be executed by the thread and
		*represents the starting address of the thread in the remote process.
		*@param param (optional) An integer value to be passed
		*to the thread function.
		*@param suspended (optional) Set to true if the new thread should
		*start in a suspended state
		*@param waitingTime (optional) The time the calling thread will wait for
		*the execution of the new thread in milliseconds
		*@return Thread A thread object
		*/
		Thread createThread(	ptr_t procedure,
									ptr_t param = 0,
									bool suspended = false,
									dword_t waitingTime = 0) const;

	private:

		/**********************************************************************
		***********************************************************************
		*********************** PRIVATE MEMBER VARIABLES **********************
		***********************************************************************
		**********************************************************************/

		Process& proc_;
};

}

#endif //SYNTHETIC_PROCESS_THREADMANAGER_HPP

/******************
******* EOF *******
******************/