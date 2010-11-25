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

#ifndef SYNTHETIC_PEFILEVIEW_HPP
#define SYNTHETIC_PEFILEVIEW_HPP

//C++ header files:
#include <vector>

//PeLib header files:
#include "../PeLib/PeLib.h" 

//Synthetic header files:
#include "Types.hpp"
#include "Process.hpp"
#include "Module.hpp"

namespace Synthetic
{
	/**
	* Class offering PeLib access to a loaded module.
	*/
	template<uint8_t bits>
	class PeFileView
	{

	public:

		/**
		* Constructor, reads pe data from memory.
		* @param proc The process which loaded the module.
		* @param mod The module to load.
		*/
		PeFileView(const Process& proc, const Module& mod) :	proc_(proc)
		{
			using namespace std;
			using namespace PeLib;

			modBase_ = mod.getBaseAddress();

			if(!modBase_)
				throw invalid_argument(	"PeFileView::PeFileView() Error : " \
												"Attempt to attach invalid module");

			//We simply buffer 16kb to be sure we read all important header stuff
			PELIB_IMAGE_NT_HEADERS<bits> ntImage;

			//Read mz header
			PELIB_IMAGE_DOS_HEADER dosHeader;
			dosHeader = proc_.readMemory<PELIB_IMAGE_DOS_HEADER>(modBase_);
			mzHeader_.read(	reinterpret_cast<uint8_t*>(&dosHeader),
									sizeof(PELIB_IMAGE_DOS_HEADER));
			if(!mzHeader_.isValid())
			{
				throw runtime_error( "PeFileView::PeFileView() Error : " \
											"MZ header is invalid !");
			}

			//Read nt header image
			PELIB_IMAGE_NT_HEADERS<bits> ntHeader;
			ptr_t ntOffset = mzHeader_.getAddressOfPeHeader();
			ntHeader = readFileOffset<PELIB_IMAGE_NT_HEADERS<bits>>(ntOffset);

			//Calculate nt header size
			size_t bytesNeeded = 0;
			bytesNeeded += ntHeader.size();
			bytesNeeded += ntHeader.OptionalHeader.NumberOfRvaAndSizes * 8;
			bytesNeeded += ntHeader.FileHeader.NumberOfSections * 0x28;

			//Read nt header into buffer
			vector<uint8_t> buffer(bytesNeeded);
			proc_.rawRead(modBase_ + ntOffset, &buffer[0], buffer.size());

			//Read pe header
			peHeader_.read(&buffer[0], static_cast<unsigned int>(buffer.size()), 0);
			if(!peHeader_.isValid())
			{
				throw runtime_error( "PeFileView::PeFileView() Error : " \
											"PE header is invalid !");
			}
		}

		/**
		* Reads data from a file offset relative to loaded module address.
		* @param offset File offset.
		* @return Read data.
		*/
		template<typename data_t>
		data_t readFileOffset(size_t offset) const
		{
			return proc_.readMemory<data_t>(modBase_ + offset);
		}

		/**
		* Returns the memory address the module was loaded at
		* @return The modules base address
		*/
		ptr_t getModuleBase() const
		{
			return modBase_;
		}

		/**
		* Returns the MZ header as PeLib object.
		* @return MZ header.
		*/
		PeLib::MzHeader& getMzHeader()
		{
			return mzHeader_;
		}

		/**
		* Const version.
		* Returns the MZ header as PeLib object.
		* @return MZ header.
		*/
		const PeLib::MzHeader& getMzHeader() const
		{
			return mzHeader_;
		}

		/**
		* Returns the PE header as PeLib object.
		* @return PE header.
		*/
		PeLib::PeHeaderT<bits>& getPeHeader()
		{
			return peHeader_;
		}

		/**
		* Const version.
		* Returns the PE header as PeLib object.
		* @return PE header.
		*/
		const PeLib::PeHeaderT<bits>& getPeHeader() const
		{
			return peHeader_;
		}

	private:
		ptr_t		modBase_;
		const Process& proc_;

		PeLib::MzHeader			mzHeader_;
		PeLib::PeHeaderT<bits>	peHeader_;
	};

	typedef PeFileView<32> PeFileView32;
	typedef PeFileView<64> PeFileView64;
}

#endif //SYNTHETIC_PEFILEVIEW_HPP

/******************
******* EOF *******
******************/