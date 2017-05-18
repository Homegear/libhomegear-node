/* Copyright 2013-2017 Sathya Laufer
 *
 * libhomegear-base is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libhomegear-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libhomegear-base.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU Lesser General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
*/

#include "HelperFunctions.h"

namespace Flows
{

HelperFunctions::HelperFunctions()
{
}

HelperFunctions::~HelperFunctions()
{
}

std::string HelperFunctions::getHexString(const uint8_t* buffer, uint32_t size)
{
	if(!buffer) return "";
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0') << std::uppercase;
	for(const uint8_t* i = buffer; i < buffer + size; ++i)
	{
		stringstream << std::setw(2) << (int32_t)(*i);
	}
	stringstream << std::dec;
	return stringstream.str();
}

std::string HelperFunctions::getHexString(const char* buffer, uint32_t size)
{
	if(!buffer) return "";
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0') << std::uppercase;
	for(const char* i = buffer; i < buffer + size; ++i)
	{
		stringstream << std::setw(2) << (int32_t)((uint8_t)(*i));
	}
	stringstream << std::dec;
	return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::vector<uint8_t>& data)
{
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0') << std::uppercase;
	for(std::vector<uint8_t>::const_iterator i = data.begin(); i != data.end(); ++i)
	{
		stringstream << std::setw(2) << (int32_t)(*i);
	}
	stringstream << std::dec;
	return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::vector<char>& data)
{
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0') << std::uppercase;
	for(std::vector<char>::const_iterator i = data.begin(); i != data.end(); ++i)
	{
		stringstream << std::setw(2) << (int32_t)((uint8_t)(*i));
	}
	stringstream << std::dec;
	return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::vector<uint16_t>& data)
{
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0') << std::uppercase;
	for(std::vector<uint16_t>::const_iterator i = data.begin(); i != data.end(); ++i)
	{
		stringstream << std::setw(2) << (int32_t)((*i) >> 8) << std::setw(2) << (int32_t)((*i) & 0xFF);
	}
	stringstream << std::dec;
	return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::string& data)
{
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0') << std::uppercase;
	for(std::string::const_iterator i = data.begin(); i != data.end(); ++i)
	{
		stringstream << std::setw(2) << (int32_t)((uint8_t)(*i));
	}
	stringstream << std::dec;
	return stringstream.str();
}

std::string HelperFunctions::getHexString(int32_t number, int32_t width)
{
	std::ostringstream stringstream;
	stringstream << std::hex << std::setfill('0');
	if(width > -1) stringstream << std::setw(width);
	stringstream << std::uppercase << number << std::dec;
	return stringstream.str();
}

}
