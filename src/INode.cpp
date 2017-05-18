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

#include "INode.h"

namespace Flows
{

INode::INode(std::string path, std::string name)
{
	_referenceCounter = 0;

	_locked = false;
	_path = path;
	_name = name;
}

INode::~INode()
{
}

void INode::log(int32_t logLevel, std::string message)
{
	if(_log) _log(_id, logLevel, message);
}

void INode::subscribePeer(uint64_t peerId, int32_t channel, std::string variable)
{
	if(_subscribePeer) _subscribePeer(_id, peerId, channel, variable);
}

void INode::unsubscribePeer(uint64_t peerId, int32_t channel, std::string variable)
{
	if(_unsubscribePeer) _unsubscribePeer(_id, peerId, channel, variable);
}

void INode::output(uint32_t index, PVariable message)
{
	if(_output) _output(_id, index, message);
}

PVariable INode::invoke(std::string methodName, PArray& parameters)
{
	if(_invoke) return _invoke(methodName, parameters);
	return Variable::createError(-32500, "No callback method set.");
}

}
