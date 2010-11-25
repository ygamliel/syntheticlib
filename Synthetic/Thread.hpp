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

#ifndef SYNTHETIC_PROCESS_THREAD_HPP
#define SYNTHETIC_PROCESS_THREAD_HPP

//Windows Header Files:
#include <Windows.h>

//Synthetic Header Files:
#include "SysObjectIterator.hpp"
#include "Types.hpp"

namespace Synthetic
{
	//Enumerations

	/**
	* Defines thread priority types\n
	* Windows Server 2008 and Windows Vista specific flags are left\n
	* out to stay compatible to older versions of Windows.\n
	*/
	enum ThreadPriority
	{
		IDLE = THREAD_PRIORITY_IDLE,
		LOWEST = THREAD_PRIORITY_LOWEST,
		LOWER = THREAD_PRIORITY_BELOW_NORMAL,
		NORMAL = THREAD_PRIORITY_NORMAL,
		HIGHER = THREAD_PRIORITY_ABOVE_NORMAL,
		HIGHEST = THREAD_PRIORITY_HIGHEST,
		CRITICAL = THREAD_PRIORITY_TIME_CRITICAL
	};

	enum ThreadCode
	{
		ACTIVE = STILL_ACTIVE
	};

	/**
	* Class representing a Windows thread
	*/
	class Thread
	{
	public:

		typedef ThreadIterator iterator;

		/**********************************************************************
		***********************************************************************
		************************ PUBLIC MEMBER FUNCTIONS **********************
		***********************************************************************
		**********************************************************************/

		/**
		* Default constructor
		*/
		Thread();

		/**
		* Optional constructor
		* Opens a thread by an id
		* @param id The Id of the thread you want to attach
		*/
		Thread(tid_t id);

		/**
		* Copy constructor
		* @param proc Another Thread, which should be copied
		*/
		Thread(const Thread& thread);

		/**
		* Destructor
		* Calls close()
		*/
		~Thread();

		/**
		* Opens a thread by a threadid
		* If a thread is already opened, it will get closed and 
		* the new thread opened.
		* @param id The id of the thread you want to attach.
		*/
		void open(tid_t id);

		/**
		* Closes the current threadhandle
		*/
		void close();

		/**
		* Retrieves the Id.
		* @return tid_t The attached thread's Id.
		*/
		tid_t getId() const;

		/**
		* Retrieves the low level threadhandle for use in WinAPI functions.
		* Note that the handle becomes invalid when the destructor/close() is
		* called.
		* Copy the handle by using DuplicateHandle if you want to avoid this,
		* check a WinAPI reference for that.
		* @return handle_t A handle associated to the thread.
		*/
		handle_t getHandle() const;

		/**
		*Retrieves the priority value for the specified thread. This value,
		*together with the priority class of the thread's process,
		*determines the thread's base-priority level.
		*@return ThreadPriority The thread's priority level.
		*@see ThreadPriority
		*/
		ThreadPriority getPriority() const;

		/**
		* Sets the priority value for the specified thread. This value,
		* together with the priority class of the thread's process,
		* determines the thread's base priority level.
		* @param priority The priority value for the thread.
		* @see ThreadPriority
		*/
		void setPriority(ThreadPriority priority) const;

		/**
		* Terminates a thread.
		* @param exitCode The exit code for the thread.
		*/
		void terminate(unsigned long exitCode) const;

		/**
		* Retrieves the termination status of the thread.
		* @return unsigned long The thread termination status or ACTIVE.
		*/
		unsigned long getExitCode() const;

		/**
		* Suspends the thread.
		* @return unsigned long The thread's previous suspend count.
		*/
		unsigned long suspend() const;

		/**
		* Decrements a thread's suspend count. When the suspend count is decremented
		* to zero, the execution of the thread is resumed.
		* @return unsigned long The thread's previous suspend count.
		*/
		unsigned long resume() const;

		/**
		* Retrieves the context of the thread.
		* @param contextFlags Specifies which portions of the thread's
		* context are retrieved.
		* @return context_t The appropriate context of the thread.
		*/
		CONTEXT getContext(unsigned long contextFlags) const;

		/**
		* Sets the context for the thread.
		* @param newContext The context to be set in the thread.
		* @param unsigned long Specifies which portions of the thread's
		* context are retrieved.
		*/
		void setContext(	CONTEXT& newContext,
								unsigned long contextFlags) const;

		/**
		* Waits until the thread is in the signaled state or
		* the time-out interval elapses.
		* @param milliSeconds The time-out interval, in milliseconds. 
		* If INFINITE, the function will return only when the object is signaled.
		*/
		void wait(unsigned long milliSeconds) const;

	private:

		/**********************************************************************
		***********************************************************************
		*********************** PRIVATE MEMBER VARIABLES **********************
		***********************************************************************
		**********************************************************************/

		tid_t id_;
		handle_t handle_;
	};
}

#endif //SYNTHETIC_PROCESS_THREAD_HPP

/******************
******* EOF *******
******************/