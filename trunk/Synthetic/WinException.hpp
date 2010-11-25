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

#ifndef SYNTHETIC_WINEXCEPTION_HPP
#define SYNTHETIC_WINEXCEPTION_HPP

//Windows Header Files:
#include <windows.h>

//C++ Header Files:
#include <string>
#include <exception>
#include <sstream>
#include <vector>

namespace Synthetic {

class WinException : public std::exception
{
public:

	/**
	*Constructor
	*Prepares error information
	*@param causedIn Where did the error happen?
	*@param failedName Which WinAPI-function failed?
	*@param errorCode What does GetLastError say?
	*/
	WinException(	const std::string& causedIn,
						const std::string& failedName,
						DWORD errorCode) :	causedIn_(causedIn),
													failedName_(failedName),
													errorCode_(errorCode)
	{
		//Create a describing error string
		std::vector<char> buffer(MAX_PATH);
		DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		DWORD ec = FormatMessageA(	flags,
											NULL,
											errorCode,
											MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
											&buffer[0],
											static_cast<DWORD>(buffer.size()),
											NULL);
		std::string errString;
		ec ?	errString.assign(buffer.begin(), buffer.begin() + ec) :
				errString.assign("Unknown Error");

		//Format a meaningfull error message
		std::stringstream errorMessage;
		errorMessage << causedIn_ << " Error : " << failedName_ <<
		" failed with errorcode " << errorCode_ << "(" << 
		(ec ? &buffer[0] : "Unknown Error") << ")";

		formattedError_.assign(errorMessage.str());
	}

	/**
	*@return A formatted error message
	*/
	const char* what() const
	{
		return formattedError_.c_str();
	}

	/**
	*@return Where did the error happen?
	*/
	const std::string& causedIn() const
	{
		return causedIn_;
	}

	/**
	*@return Which WinAPI-function failed?
	*/
	const std::string& failedName() const
	{
		return failedName_;
	}

	/**
	*@return What does GetLastError say?
	*/
	DWORD errorCode() const
	{
		return errorCode_;
	}

protected:

	std::string formattedError_;
	std::string causedIn_;
	std::string failedName_;
	DWORD errorCode_;
};

} //End namespace Synthetic

#endif //SYNTHETIC_WINEXCEPTION_HPP

/******************
******* EOF *******
******************/