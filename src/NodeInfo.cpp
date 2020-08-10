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

#include "NodeInfo.h"

namespace Flows {

PVariable NodeInfo::serialize() {
  PVariable info = std::make_shared<Variable>(VariableType::tStruct);
  info->structValue->emplace("id", std::make_shared<Variable>(id));
  info->structValue->emplace("flowId", std::make_shared<Variable>(flowId));
  info->structValue->emplace("namespace", std::make_shared<Variable>(nodeNamespace));
  info->structValue->emplace("type", std::make_shared<Variable>(type));
  info->structValue->emplace("info", this->info);

  PVariable array = std::make_shared<Variable>(VariableType::tArray);
  array->arrayValue->reserve(wiresIn.size());
  for (auto &input : wiresIn) {
    PVariable innerArray = std::make_shared<Variable>(VariableType::tStruct);
    for (auto &wire : input) {
      innerArray->structValue->emplace("id", std::make_shared<Variable>(wire.id));
      innerArray->structValue->emplace("port", std::make_shared<Variable>(wire.port));
    }
    array->arrayValue->push_back(innerArray);
  }
  info->structValue->emplace("wiresIn", array);

  array = std::make_shared<Variable>(VariableType::tArray);
  array->arrayValue->reserve(wiresOut.size());
  for (auto &output : wiresOut) {
    PVariable innerArray = std::make_shared<Variable>(VariableType::tStruct);
    for (auto &wire : output) {
      innerArray->structValue->emplace("id", std::make_shared<Variable>(wire.id));
      innerArray->structValue->emplace("port", std::make_shared<Variable>(wire.port));
    }
    array->arrayValue->push_back(innerArray);
  }
  info->structValue->emplace("wiresOut", array);

  return info;
}

}
