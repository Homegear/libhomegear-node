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

INode::INode(std::string path, std::string nodeNamespace, std::string type, const std::atomic_bool* frontendConnected)
{
	_referenceCounter = 0;

	_locked = false;
	_path = path;
	_namespace = nodeNamespace;
	_type = type;
	_frontendConnected = frontendConnected;
}

INode::~INode()
{
	//Function pointers need to be cleaned up before unloading the module
	_log = std::function<void(std::string, int32_t, std::string)>();
	_subscribePeer = std::function<void(std::string, uint64_t, int32_t, std::string)>();
	_unsubscribePeer = std::function<void(std::string, uint64_t, int32_t, std::string)>();
	_output = std::function<void(std::string, uint32_t, PVariable)>();
	_invoke = std::function<PVariable(std::string, PArray&)>();
	_invokeNodeMethod = std::function<PVariable(std::string, std::string, PArray&)>();
	_nodeEvent = std::function<void(std::string, std::string, PVariable)>();
	_getNodeData = std::function<PVariable(std::string, std::string)>();
	_setNodeData = std::function<void(std::string, std::string, PVariable)>();
	_getConfigParameter = std::function<PVariable(std::string, std::string)>();
}

void INode::setLog(std::function<void(std::string, int32_t, std::string)> value)
{
	_log.swap(value);
	Output::init(_id, &_log);
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

PVariable INode::invokeNodeMethod(std::string nodeId, std::string methodName, PArray& parameters)
{
	if(_invokeNodeMethod) return _invokeNodeMethod(nodeId, methodName, parameters);
	return Variable::createError(-32500, "No callback method set.");
}

PVariable INode::invokeLocal(std::string methodName, PArray& parameters)
{
	std::map<std::string, std::function<PVariable(PArray& parameters)>>::iterator localMethodIterator = _localRpcMethods.find(methodName);
	if(localMethodIterator == _localRpcMethods.end())
	{
		Output::printError("Warning: RPC method not found: " + methodName);
		return Variable::createError(-32601, ": Requested method not found.");
	}

	return localMethodIterator->second(parameters);
}

void INode::nodeEvent(std::string topic, PVariable& value)
{
	if(_nodeEvent) _nodeEvent(_id, topic, value);
}

PVariable INode::getNodeData(std::string key)
{
	if(_getNodeData) return _getNodeData(_id, key);
	return Variable::createError(-32500, "No callback method set.");
}

void INode::setNodeData(std::string key, PVariable value)
{
	if(_setNodeData) _setNodeData(_id, key, value);
}

PVariable INode::getConfigParameter(std::string nodeId, std::string name)
{
	if(_getConfigParameter) return _getConfigParameter(nodeId, name);
	return Variable::createError(-32500, "No callback method set.");
}

}
