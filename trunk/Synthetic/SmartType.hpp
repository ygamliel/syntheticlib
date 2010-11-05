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

#ifndef SYNTHETIC_SAFEHANDLE_HPP
#define SYNTHETIC_SAFEHANDLE_HPP

//Windows header Files:
#include <windows.h>

namespace Synthetic
{
	/**
	* RAII class to automatically delete resources\n
	* data_t = Type of data\n
	* deleter = Custom function deleting an object of data_t\n
	*/
	template<class data_t, BOOL(__stdcall *deleter)(data_t)>
	class SmartType
	{
	public:

		/**
		* Constructor.
		* @param data The data which should be deleted by the destructor.
		*/
		SmartType(data_t data = 0) : data_(data)
		{ }

		/**
		* Destructor.
		* Deletes data.
		*/
		~SmartType()
		{
			close();
		}

		/**
		* Assignment operator.
		* Deletes previous data and assigns new.
		* @param data The data which should be assigned.
		* @return data_t The newly assigned data.
		*/
		data_t operator=(data_t data)
		{
			close();
			data_ = data;
			return data_;
		}

		/**
		* Operator overload to provide intuitive usage of the object.
		*/
		operator data_t() const
		{
			return data_;
		}

		/**
		* Simple validity check of the data
		* @return bool true if data is unequal zero, false otherwise
		*/
		bool isValid() const
		{
			return data_ != 0;
		}

		/**
		* Deletes the stored data
		*/
		void close()
		{
			if(isValid())
			{
				deleter(data_);
				data_ = 0;
			}
		}

		/**
		* Provides direct access to the stored element by passing a reference
		* @return Reference to the stored data
		*/
		data_t& get()
		{
			return data_;
		}

	private:
		data_t data_;
	};

	typedef SmartType<HANDLE, CloseHandle> SmartHandle;
	typedef SmartType<HMODULE, FreeLibrary> SmartModule;

}

#endif //SYNTHETIC_SAFEHANDLE_HPP

/******************
******* EOF *******
******************/