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

#include "MessageProperty.h"
#include "Math.h"

namespace Flows {

MessageProperty::MessageProperty(const std::string &property) {
  std::string currentString;
  currentString.reserve(property.size());
  bool inBrackets = false;
  for (auto c : property) {
    if (c == '[') {
      if (!currentString.empty()) {
        _property.emplace_back(std::make_pair(currentString, false));
        currentString.clear();
      }
      inBrackets = true;
    } else if (c == ']') {
      inBrackets = false;
      _property.emplace_back(std::make_pair(currentString, true));
      currentString.clear();
    } else if (inBrackets) {
      currentString.push_back(c);
    } else if (c == '.') {
      if (!currentString.empty()) _property.emplace_back(std::make_pair(currentString, false));
      currentString.clear();
    } else currentString.push_back(c);
  }
  if (!currentString.empty()) _property.emplace_back(std::make_pair(currentString, false));
}

bool MessageProperty::empty() {
  return _property.empty();
}

Flows::PVariable MessageProperty::match(Flows::PVariable &message) {
  Flows::PVariable currentMessage = message;
  for (auto &element : _property) {
    if (element.second) {
      auto arrayIndex = (uint64_t)Math::getNumber64(element.first);
      if (arrayIndex >= currentMessage->arrayValue->size()) return Flows::PVariable();
      currentMessage = currentMessage->arrayValue->at(arrayIndex);
    } else {
      auto messageIterator = currentMessage->structValue->find(element.first);
      if (messageIterator == currentMessage->structValue->end()) return Flows::PVariable();
      currentMessage = messageIterator->second;
    }
  }

  return currentMessage;
}

bool MessageProperty::erase(Flows::PVariable &message) {
  Flows::PVariable currentMessage = message;
  for (int32_t i = 0; (unsigned)i < _property.size(); i++) {
    if (_property.at(i).second) {
      auto arrayIndex = Math::getUnsignedNumber64(_property.at(i).first);
      if (arrayIndex >= currentMessage->arrayValue->size()) return false;
      if ((unsigned)i < _property.size() - 1) currentMessage = currentMessage->arrayValue->at(arrayIndex);
      else currentMessage->arrayValue->erase(currentMessage->arrayValue->begin() + arrayIndex);
    } else {
      auto messageIterator = currentMessage->structValue->find(_property.at(i).first);
      if (messageIterator == currentMessage->structValue->end()) return false;
      if ((unsigned)i < _property.size() - 1) currentMessage = messageIterator->second;
      else currentMessage->structValue->erase(messageIterator);
    }
  }
  return true;
}

bool MessageProperty::set(Flows::PVariable &message, Flows::PVariable &value) {
  Flows::PVariable currentMessage = message;
  for (int32_t i = 0; (unsigned)i < _property.size(); i++) {
    if (_property.at(i).second) {
      auto arrayIndex = Math::getUnsignedNumber64(_property.at(i).first);
      if (arrayIndex >= currentMessage->arrayValue->size()) {
        if ((unsigned)i < _property.size() - 1) {
          Flows::PVariable subElement;
          currentMessage->arrayValue->reserve(arrayIndex + 1);
          while (arrayIndex >= currentMessage->arrayValue->size()) {
            subElement = std::make_shared<Flows::Variable>(_property.at(i + 1).second ? Flows::VariableType::tArray : Flows::VariableType::tStruct);
            currentMessage->arrayValue->emplace_back(subElement);
          }
          currentMessage = subElement;
        } else {
          currentMessage->arrayValue->reserve(arrayIndex + 1);
          while (arrayIndex >= currentMessage->arrayValue->size()) {
            currentMessage->arrayValue->emplace_back(std::make_shared<Flows::Variable>());
          }
          currentMessage->arrayValue->emplace_back(value);
        }
      } else {
        if ((unsigned)i < _property.size() - 1) currentMessage = currentMessage->arrayValue->at(arrayIndex);
        else currentMessage->arrayValue->at(arrayIndex) = value;
      }
    } else {
      auto messageIterator = currentMessage->structValue->find(_property.at(i).first);
      if (messageIterator == currentMessage->structValue->end()) {
        if ((unsigned)i < _property.size() - 1) {
          auto subElement = std::make_shared<Flows::Variable>(_property.at(i + 1).second ? Flows::VariableType::tArray : Flows::VariableType::tStruct);
          currentMessage->structValue->emplace(_property.at(i).first, subElement);
          currentMessage = subElement;
        } else currentMessage->structValue->emplace(_property.at(i).first, value);
      } else {
        if ((unsigned)i < _property.size() - 1) currentMessage = messageIterator->second;
        else messageIterator->second = value;
      }
    }
  }
  return true;
}

}