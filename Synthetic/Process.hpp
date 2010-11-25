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

#ifndef SYNTHETIC_PROCESS_PROCESS_HPP
#define SYNTHETIC_PROCESS_PROCESS_HPP

//Windows Header Files:
#include <windows.h>

//C++ Header Files:
#include <string>
#include <vector>

//Synthetic Header Files:
#include "Types.hpp"
#include "SysObjectIterator.hpp"

namespace Synthetic
{
	//Enumerations

	/**
	* All relevant calling conventions
	*/
	enum CallingConvention
	{
		STDCALL_CONVENTION,
		CDECL_CONVENTION,
		THISCALL_CONVENTION,
		GCCTHISCALL_CONVENTION
	};

	/**
	* Interface to a Windows process
	*/
	class Process
	{
	public:

		typedef ProcessIterator iterator;

		/**********************************************************************
		***********************************************************************
		************************* PUBLIC FREE FUNCTIONS ***********************
		***********************************************************************
		**********************************************************************/

		/**
		*Retrieves the PID of the process the currently active window
		*is associated to
		*@return The found process' PID
		*/
		static pid_t getProcessByForegroundWindow();

		/**
		*Retrieves the PID of the first found window with a given name
		*Note that all other windows with same name will be ignored
		*@param windowName Case-insensitive window name
		*@return The found process' PID
		*/
		static pid_t getProcessByWindowName(const std::wstring& windowName);

		/**
		*Retrieves the PID of the first found process with a given name
		*Note that all other processes with same name will be ignored
		*If you don't like that behaviour, use getProcessListByName()
		*@param processName Case-insensitive process name
		*@return The found process' PID
		*/
		static pid_t getProcessByName(std::wstring processName);

		/**
		*Retrieves the PID of the process associated to a windowhandle
		*@param windowHandle A WinAPI window handle
		*@return A found process' PID or zero in case of error
		*/
		static pid_t getProcessByWindowHandle(HWND windowHandle);

		/**
		*Retrieves the PID of the current process
		*@return The current process' PID
		*/
		static pid_t getCurrentProcess();

		/**
		*Retrieves the PIDs of all running processes by a name
		*@param processName Case-insensitive string specifying the name
		*@param dest Reference to a vector to hold all found PIDs
		*@return size_t Number of found processes
		*/
		static size_t getProcessListByName(	std::wstring processName,
														std::vector<pid_t>& dest);

		/**
		*Retrieves the PIDs of all running processes
		*@param dest Reference to a vector to hold all found PIDs
		*@return size_t Number of found processes
		*/
		static size_t getProcessList(std::vector<pid_t>& dest);

		/**********************************************************************
		***********************************************************************
		************************ PUBLIC MEMBER FUNCTIONS **********************
		***********************************************************************
		**********************************************************************/

		/**
		* Default constructor.
		* Initializes the current process for work with other processes.
		*/
		Process();

		/**
		* Optional constructor.
		* Initializes the current process for work with other processes and opens
		* the targeted process.
		* @param pid_t The PID of the process you want to attach.
		*/
		Process(pid_t pid);

		/**
		* Copy constructor.
		* Initializes the current process for work with other processes and opens
		* the targeted process.
		* @param proc Another Process, which should be copied.
		*/
		Process(const Process& proc);

		/**
		* Default destructor.
		* Calls close().
		*/
		~Process();

		/**
		* []-operator to read pointers from the remote process.
		* @param address A valid 32/64-bit memoryaddress.
		* @return ptr_t The memoryread's content.
		*/
		ptr_t operator[](ptr_t address) const;

		/**
		* Retrieves the low level processhandle for use in WinAPI functions.
		* Note that the handle becomes invalid when the destructor/close() is
		* called.
		* Copy the handle by using DuplicateHandle if you want to avoid this,
		* check a WinAPI reference for that.
		* @return HANDLE A handle associated to the process.
		*/
		HANDLE getHandle() const;

		/**
		* Retrieves the PID.
		* @return pid_t The attached process' PID.
		*/
		pid_t getId() const;

		/**
		* Creates a new process and opens it.
		* @param applicationName The name of the executeable to be executed.
		* The string can specify the full path and file name of the module to
		* execute or it can specify a partial name.
		* @param commandLine (optional) Commandline-Arguments to be passed to the
		* created process. Can be ignored or simply replaced by an empty string
		* in case you don't need it.
		* @param directory (optional) The working directory of the created
		* process, if ignored or if an empty string is passed, the new process
		* will have the same current drive and directory as the calling process.
		* @param suspended (optional) If set to true, the new process will be
		* created with a suspended mainthread. Ignore or set to false if this
		* is not wanted.
		* @param waitingTime (optional) The time in ms the function waits for
		* the newly created process.
		* @return pid_t The new process' PID
		*/
		pid_t createProcessAndOpen(	const std::wstring& applicationName,
													const std::wstring& commandLine = L"",
													const std::wstring directory = L"",
													bool suspended = false,
													dword_t waitingTime = 0);

		/**
		* Opens a process by a PID.
		* If a process is already opened, it will get closed and 
		* the new process opened.
		* @param pid_t The PID of the process you want to attach.
		*/
		void open(pid_t pid);

		/**
		* Closes handles to the current process.
		*/
		void close();

		/**
		* Terminates the attached process and calls close().
		* @param exitCode (optional) An integer value which will be returned as
		* exit code by the process. If ignored, zero will be returned.
		*/
		void terminate(dword_t exitCode = 0);

		/**
		* Reads data from an address.
		* @param source The data's address.
		* @param dest Pointer to a buffer for read data.
		* @param amount Amount of bytes to read.
		* @return size_t The amount of written data.
		*/
		template<typename data_t>
		size_t rawRead(	const ptr_t source,
								data_t* dest,
								const size_t amount) const
		{
			SIZE_T bytesRead;
			int ec = ::ReadProcessMemory(	handle_,
													reinterpret_cast<const void*>(source),
													static_cast<void*>(dest),
													amount,
													&bytesRead);
			if(!ec)
			{
				const dword_t error = GetLastError();
				throw WinException(	"Process::rawRead<>()",
											"ReadProcessMemory()",
											error);				
			}

			return bytesRead;
		}

		/**
		* Reads data from an address.
		* @param address The data's address.
		* @return data_t The read data.
		*/
		template <typename data_t>
		data_t readMemory(ptr_t address) const
		{
			data_t returnedBytes;
			rawRead(address, &returnedBytes, sizeof(data_t));

			return returnedBytes;
		}

		/**
		* Reads a (zero-terminated) string from an address.
		* @param address The string's address.
		* @param amountChars The maximum length which will be read.
		* @param dest Reference to a STL string which holds the read data
		* @return size_t Length of the read zero terminated string
		*/
		template <typename char_t>
		size_t readString(	ptr_t address,
									size_t amountChars,
									std::basic_string<char_t>& dest) const
		{
			std::vector<char_t> returnedBytes(amountChars);
			rawRead(address, &returnedBytes[0], returnedBytes.size());

			//Append a zero in case we didn't read the full string
			returnedBytes.push_back(0);
			dest.assign(&returnedBytes[0]);

			return dest.length();
		}

		/**
		* Writes data to an address.
		* @param dest The address the data will be written to.
		* @param source The data which has to be written.
		* @param amount The amount of bytes to write.
		* @return size_t The amount of written bytes.
		*/
		template<typename data_t>
		size_t rawWrite(	const ptr_t dest,
								const data_t* source,
								const size_t amount) const
		{
			SIZE_T bytesWritten;
			int ec = ::WriteProcessMemory(	handle_,
														reinterpret_cast<void*>(dest),
														static_cast<const void*>(source),
														amount,
														&bytesWritten);
			if(!ec)
			{
				const dword_t error = GetLastError();
				throw WinException(	"Process::rawWrite<>()",
											"WriteProcessMemory()",
											error);				
			}

			return bytesWritten;
		}

		/**
		* Writes data to an address.
		* @param dest The address the data will be written to.
		* @param value The data which has to be written.
		* @return size_t The amount of written bytes.
		*/
		template <typename data_t>
		size_t writeMemory(	const ptr_t dest,
									const data_t& value) const
		{
			return rawWrite(address, &value, sizeof(value));
		}

		/**
		* Writes a zero-terminated string to an address.
		* @param address The address the string will be written to.
		* @param value A STL string-type which has to be written.
		* @return size_t The amount of written bytes.
		*/
		template <typename char_t>
		size_t writeString(	const ptr_t address,
									const std::basic_string<char_t>& value) const
		{
			if(!value.length())
				return 0;

			const size_t stringLength = (value.length() + 1) * sizeof(char_t);
			return rawWrite(address, &value[0], stringLength);
		}

	private:

		/**********************************************************************
		***********************************************************************
		************************ PRIVATE MEMBER FUNCTIONS *********************
		***********************************************************************
		**********************************************************************/

		/*
		* Sets permissions for the current process to debug other processes
		*/
		void addDebugPrivileges_() const;

		/**********************************************************************
		***********************************************************************
		*********************** PRIVATE MEMBER VARIABLES **********************
		***********************************************************************
		**********************************************************************/

		handle_t		handle_;
		pid_t	id_;
	};
}

#endif //SYNTHETIC_PROCESS_PROCESS_HPP

/******************
******* EOF *******
******************/