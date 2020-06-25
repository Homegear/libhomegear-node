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

#ifndef LIBHOMEGEAR_NODE_MESSAGEPROPERTY_H
#define LIBHOMEGEAR_NODE_MESSAGEPROPERTY_H

#include "Variable.h"

namespace Flows
{

/**
 * Class to parse and work with Strings from frontend message property inputs.
 */
class MessageProperty
{
private:
    std::vector<std::pair<std::string, bool>> _property;
public:
    MessageProperty() = default;
    explicit MessageProperty(const std::string& property);

    bool empty();
    Flows::PVariable match(Flows::PVariable& message);
    bool erase(Flows::PVariable& message);
    bool set(Flows::PVariable& message, Flows::PVariable& value);
};

}

#endif //LIBHOMEGEAR_NODE_MESSAGEPROPERTY_H
