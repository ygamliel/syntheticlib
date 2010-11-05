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
#include "Thread.hpp"
#include "WinException.hpp"

using namespace Synthetic;

/**********************************************************************
***********************************************************************
************************ PUBLIC MEMBER FUNCTIONS **********************
***********************************************************************
**********************************************************************/

Thread::Thread() : handle_(NULL), id_(0)
{ }

Thread::Thread(ThreadId id) : handle_(NULL), id_(id)
{
	open(id);
}

Thread::Thread(const Thread& thread) : handle_(NULL), id_(thread.id_)
{
	close();

	//Duplicate handle to avoid it getting invalid if the
	//original objects destructor gets called
	if(thread.handle_)
	{
		if(!DuplicateHandle(	GetCurrentProcess(),
									thread.handle_,
									GetCurrentProcess(),
									&handle_,
									NULL,
									FALSE,
									DUPLICATE_SAME_ACCESS))
		{
			throw WinException(	"Thread::Thread()",
										"DuplicateHandle()",
										GetLastError());
		}
	}
}

Thread::~Thread()
{
	close();
}


void Thread::open(ThreadId id)
{
	if(!id)
		return;

	close();

	//Windows Server 2008 and Windows Vista specific access rights are left
	//out to stay compatible to older versions of Windows.
	DWORD desiredAccess =	SYNCHRONIZE |
									THREAD_DIRECT_IMPERSONATION |
									THREAD_GET_CONTEXT |
									THREAD_IMPERSONATE |
									THREAD_QUERY_INFORMATION  |
									THREAD_SET_CONTEXT |
									THREAD_SET_INFORMATION |
									THREAD_SET_THREAD_TOKEN |
									THREAD_SUSPEND_RESUME |
									THREAD_TERMINATE;

	handle_ = OpenThread(desiredAccess, FALSE, id);
	if(!handle_)
	{
		throw WinException(	"Thread::open()",
									"OpenThread()",
									GetLastError());
	}
}

void Thread::close()
{
	if(handle_ != NULL && handle_ != INVALID_HANDLE_VALUE)
		CloseHandle(handle_);

	handle_ = NULL;
}

ThreadId Thread::getId() const
{
	return id_;
}

HANDLE Thread::getHandle() const
{
	return handle_;
}

ThreadPriority Thread::getPriority() const
{
	int priority = GetThreadPriority(getHandle());
	if(priority == THREAD_PRIORITY_ERROR_RETURN)
	{
		throw WinException(	"Thread::getPriority()",
									"GetThreadPriority()",
									GetLastError());
	}

	return static_cast<ThreadPriority>(priority);
}

void Thread::setPriority(ThreadPriority priority) const
{
	BOOL ec = SetThreadPriority(getHandle(), priority);
	if(!ec)
	{
		throw WinException(	"Thread::setPriority()",
									"SetThreadPriority()",
									GetLastError());
	}
}

void Thread::terminate(unsigned long exitCode) const
{
	BOOL ec = TerminateThread(getHandle(), exitCode);
	if(!ec)
	{
		throw WinException(	"Thread::terminate()",
									"TerminateThread()",
									GetLastError());
	}
}

unsigned long Thread::getExitCode()  const
{
	DWORD exitCode;
	BOOL ec = GetExitCodeThread(getHandle(), &exitCode);
	if(!ec)
	{
		throw WinException(	"Thread::getExitCode()",
									"GetExitCodeThread()",
									GetLastError());
	}

	return exitCode;
}

unsigned long Thread::suspend() const
{
	DWORD suspendedCount = SuspendThread(getHandle());
	if(suspendedCount == -1)
	{
		throw WinException(	"Thread::suspend()",
									"SuspendThread()",
									GetLastError());
	}

	return suspendedCount;
}

unsigned long Thread::resume() const
{
	DWORD suspendedCount = ResumeThread(getHandle());
	if(suspendedCount == -1)
	{
		throw WinException(	"Thread::resume()",
									"ResumeThread()",
									GetLastError());
	}

	return suspendedCount;
}

context_t Thread::getContext(unsigned long contextFlags) const
{
	CONTEXT threadContext;
	ZeroMemory(&threadContext, sizeof(CONTEXT));
	threadContext.ContextFlags = contextFlags;

	BOOL ec = GetThreadContext(getHandle(), &threadContext);
	if(!ec)
	{
		throw WinException(	"Thread::getContext()",
									"GetThreadContext()",
									GetLastError());
	}

	return threadContext;
}

void Thread::setContext(context_t& newContext, unsigned long contextFlags) const
{
	newContext.ContextFlags = contextFlags;

	BOOL ec = SetThreadContext(getHandle(), &newContext);
	if(!ec)
	{
		throw WinException(	"Thread::setContext()",
									"SetThreadContext()",
									GetLastError());
	}
}

void Thread::wait(unsigned long milliSeconds) const
{
	DWORD returnEvent = WaitForSingleObject(getHandle(), milliSeconds);
	if(returnEvent != WAIT_OBJECT_0)
	{
		throw WinException(	"Thread::wait()",
									"WaitForSingleObject()",
									GetLastError());
	}
}

/******************
******* EOF *******
******************/