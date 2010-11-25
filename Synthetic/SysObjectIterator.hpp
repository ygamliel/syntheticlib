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

#ifndef SYNTHETIC_SYSOBJECTITERATOR_HPP
#define SYNTHETIC_SYSOBJECTITERATOR_HPP

//Windows header files:
#include <Windows.h>
#include <TlHelp32.h>

//C++ header files:
#include <stdexcept>
#include <iterator>

//Synthetic header files:
#include "Types.hpp"
#include "WinException.hpp"
#include "Auxiliary.hpp"

namespace Synthetic
{
	/**
	* STL compliant input iterator\n
	* Template class to wrap iterating the Tlhelp32 API.\n
	* entry_t			= Type of entry returned by the First/Next functions\n
	* func_getfirst	= Functionpointer to function retrieving the first entry\n
	* func_getnext		= Function pointer to function retrieving the next entry\n
	* flag_spec			= Defines a flag for use in CreateToolhelp32Snapshot()\n
	*/
	template<	class entry_t,
					BOOL (__stdcall *func_getfirst)(HANDLE, entry_t*),
					BOOL (__stdcall *func_getnext)(HANDLE, entry_t*),
					unsigned long flag_spec>
	class SysObjectIterator : public std::iterator<	std::input_iterator_tag,
																	entry_t>
	{

	public:

		/**
		* Simple Constructor allocating data and validating the iterator.
		* @param pid Specify a process id for which data should be iterated.
		* Passing zero will iterate all specific resources on the system.
		*/
		SysObjectIterator(pid_t pid)
		{
			//Get a snapshot
			entry_.dwSize = sizeof(entry_t);
			snapshot_ = CreateToolhelp32Snapshot(flag_spec, pid);
			if(snapshot_ == INVALID_HANDLE_VALUE)
			{
				DWORD error = GetLastError();
				throw WinException(	"SysObjectIterator::SysObjectIterator()",
											"CreateToolhelp32Snapshot()",
											error);
			}

			//Get first entry
			state_ = func_getfirst(snapshot_, &entry_);
			if(!state_)
			{
				DWORD error = GetLastError();
				throw WinException(	"SysObjectIterator::SysObjectIterator()",
											"func_getfirst()",
											error);
			}
		}

		/**
		* Hackish constructor to create an invalid iterator.
		* Constructing it will create an invalid iterator
		* for use in loops etc.
		*/
		SysObjectIterator() : state_(FALSE), snapshot_(0)
		{ }

		/**
		* Copyconstructor for deep copy.
		* @param it SysObjectIterator to copy.
		*/
		SysObjectIterator(const SysObjectIterator& it)
		{
			if(!it.isInValidState())
			{
				state_		= FALSE;
				snapshot_	= 0;
			}
			else
			{
				state_		= TRUE;
				snapshot_	= Aux::duplicateHandleLocal(it.snapshot_);
				entry_		= it.entry_;
			}
		}

		/**
		* Simple destructor freeing resources.
		*/
		~SysObjectIterator()
		{
			if(snapshot_ != INVALID_HANDLE_VALUE && snapshot_ != 0)
				CloseHandle(snapshot_);
		}

		/**
		* Checks if the iterator is valid.
		* @return true if valid, false otherwise.
		*/
		bool isInValidState() const
		{
			return state_ != FALSE;
		}

		/**
		* Comparison, ONLY checks for validity!
		* @return true if both objects are in the same state, false otherwise.
		*/
		bool operator==(const SysObjectIterator& it) const
		{
			return (isInValidState() == it.isInValidState());
		}

		/**
		* Comparison, ONLY checks for validity!
		* @return True if both objects are in different states, false otherwise.
		*/
		bool operator!=(const SysObjectIterator& it) const
		{
			return !(*this == it);
		}

		/**
		* Pseudo-dereferencing operator returning the current entry.
		* @return Current entry.
		*/
		const entry_t& operator*() const
		{
			using namespace std;

			if(!isInValidState())
			{
				throw runtime_error(	"SysObjectIterator::operator*() Error : " \
											"Object is in invalid state");
								
			}

			return entry_;
		}

		/**
		* Pseudo-pointer operator returning a pointer to the current entry.
		* @return Pointer to current entry.
		*/
		const entry_t* operator->() const
		{
			using namespace std;

			if(!isInValidState())
			{
				throw runtime_error(	"SysObjectIterator::operator*() Error : " \
											"Object is in invalid state");
								
			}

			return &entry_;
		}

		/**
		* Preincrement operator jumping to next entry and returning
		* incremented iterator.
		* @return Incremented iterator.
		*/
		SysObjectIterator& operator++() 
		{
			using namespace std;

			if(!isInValidState())
			{
				throw runtime_error(	"SysObjectIterator::operator++() Error : " \
											"Object is in invalid state");
								
			}

			state_ = func_getnext(snapshot_, &entry_);

			return *this;
		}

		/**
		* Postincrement operator jumping to next entry and returning
		* previous iterator.
		* @return Previous iterator.
		*/
		SysObjectIterator operator++(int) 
		{
			using namespace std;

			if(!isInValidState())
			{
				throw runtime_error(	"SysObjectIterator::operator++() Error : " \
											"Object is in invalid state");
								
			}

			SysObjectIterator result = *this;
			++(*this);
			return result;
		}

	protected:
		HANDLE snapshot_;
		entry_t entry_;
		BOOL state_;
	};

	typedef SysObjectIterator<	PROCESSENTRY32W,
										Process32FirstW,
										Process32NextW,
										TH32CS_SNAPPROCESS>	ProcessIterator;

	typedef SysObjectIterator< THREADENTRY32,
										Thread32First,
										Thread32Next,
										TH32CS_SNAPTHREAD>	ThreadIterator;

	typedef SysObjectIterator<	MODULEENTRY32W,
										Module32FirstW,
										Module32NextW,
										TH32CS_SNAPMODULE>	ModuleIterator;

	typedef SysObjectIterator<	HEAPLIST32,
										Heap32ListFirst,
										Heap32ListNext,
										TH32CS_SNAPHEAPLIST>	HeapListIterator;
									
}

#endif //SYNTHETIC_SYSOBJECTITERATOR_HPP

/******************
******* EOF *******
******************/