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

#ifndef SYNTHETIC_PROCESS_ALLOCATOR_HPP
#define SYNTHETIC_PROCESS_ALLOCATOR_HPP

//Windows header files:
#include <Windows.h>

//C++ header files:
#include <list>
#include <algorithm>

//Synthetic header files:
#include "Process.hpp"
#include "Types.hpp"
#include "WinException.hpp"

namespace Synthetic
{
	/**
	* Auxiliary class to allocate/free memory in remote processes\n
	* Becomes invalid as soon as Process reference becomes invalid\n
	* scoped_release = If set to true all allocated memory is
	* released by the destructor\n
	*/
	template <bool scoped_release>
	class Allocator
	{
	public:

		/**
		* Constructor constructing a remote Allocator.
		* @param proc Reference to a Process object which has to be valid the
		* whole lifetime.
		*/
		Allocator(const Process& proc) : proc_(proc)
		{ }

		/**
		* Destructor, frees all memory if scoped_release was specified.
		*/
		~Allocator()
		{
			if(scoped_release)
				deallocateAll();
		}

		/**
		* Allocates memory
		* @param count The count of elements of typ data_t to allocate
		* @return ptr_t Address of the allocated memory
		*/
		template<typename data_t>
		ptr_t allocate(size_t count)
		{
			//Get memory
			void* allocatedMemory = VirtualAllocEx(	proc_.getHandle(),
																	NULL,
																	count * sizeof(data_t),
																	MEM_COMMIT,
																	PAGE_EXECUTE_READWRITE);
			if(!allocatedMemory)
			{
				dword_t error = GetLastError();
				throw WinException(	"Allocator::allocate()",
											"VirtualAllocEx()",
											error);
			}

			ptr_t temp = reinterpret_cast<ptr_t>(allocatedMemory);

			//If case scoped_release was specified we need to store the pointer
			if(scoped_release)
				allocations_.push_back(temp);

			return temp;
		}

		/**
		* Frees memory
		* @param ptr Address of the memory to deallocate
		*/
		void deallocate(ptr_t ptr)
		{
			//Free memory
			int ec = VirtualFreeEx(	proc_.getHandle(),
											reinterpret_cast<void*>(ptr),
											0,
											MEM_RELEASE);
			if(!ec)
			{
				dword_t error = GetLastError();
				throw WinException(	"Process::freeMemory()",
											"VirtualFreeEx()",
											error);
			}

			//If case scoped_release was specified we need to remove the pointer
			if(scoped_release)
			{
				if(allocations_.size() == 0)
					return;

				AllocIter element;
				element = std::find(allocations_.begin(), allocations_.end(), ptr);

				if((*element) == ptr)
					allocations_.erase(element);
			}
		}

		/**
		* Frees all memory allocated by this allocator
		*/
		void deallocateAll()
		{
			if(allocations_.size() == 0)
				return;

			if(allocations_.size() == 1)
			{
				deallocate(*allocations_.begin());
				return;
			}

			for(AllocIter i = allocations_.begin(); i != allocations_.end(); ++i)
				deallocate(*i);
		}

	private:
		const Process& proc_;
		std::list<ptr_t> allocations_;
		typedef std::list<ptr_t>::iterator AllocIter;
	};

	typedef Allocator<true>		ScopedAllocator;
	typedef Allocator<false>	DefaultAllocator;
}

#endif //SYNTHETIC_PROCESS_ALLOCATOR_HPP

/******************
******* EOF *******
******************/