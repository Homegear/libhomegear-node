/* Copyright 2013-2019 Homegear GmbH
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

#ifndef NODEJSONENCODER_H_
#define NODEJSONENCODER_H_

#include "Variable.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#if __GNUC__ > 4
#include <codecvt>
#endif

namespace Flows {

class JsonEncoder {
 public:
  JsonEncoder();
  ~JsonEncoder() = default;

  static std::string getString(const PVariable &variable);
  static std::vector<char> getVector(const PVariable &variable);

  static std::string encodeString(const std::string &s);
 private:
  static std::string toString(double number);
  static void encodeValue(const PVariable &variable, std::ostringstream &s);
  static void encodeValue(const PVariable &variable, std::vector<char> &s);
  static void encodeArray(const PVariable &variable, std::ostringstream &s);
  static void encodeArray(const PVariable &variable, std::vector<char> &s);
  static void encodeStruct(const PVariable &variable, std::ostringstream &s);
  static void encodeStruct(const PVariable &variable, std::vector<char> &s);
  static void encodeBoolean(const PVariable &variable, std::ostringstream &s);
  static void encodeBoolean(const PVariable &variable, std::vector<char> &s);
  static void encodeInteger(const PVariable &variable, std::ostringstream &s);
  static void encodeInteger(const PVariable &variable, std::vector<char> &s);
  static void encodeInteger64(const PVariable &variable, std::ostringstream &s);
  static void encodeInteger64(const PVariable &variable, std::vector<char> &s);
  static void encodeFloat(const PVariable &variable, std::ostringstream &s);
  static void encodeFloat(const PVariable &variable, std::vector<char> &s);
  static void encodeString(const PVariable &variable, std::ostringstream &s);
  static void encodeString(const PVariable &variable, std::vector<char> &s);
  static void encodeVoid(const PVariable &variable, std::ostringstream &s);
  static void encodeVoid(const PVariable &variable, std::vector<char> &s);
};

}

#endif
