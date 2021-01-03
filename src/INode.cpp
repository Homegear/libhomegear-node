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

#include "INode.h"

#include <utility>

#include "../config.h"

namespace Flows {

INode::INode(const std::string &path, const std::string &type, const std::atomic_bool *frontendConnected) {
  _out = std::make_shared<Output>(_id, nullptr); //_id is empty at this point and will be set through setId()
  _path = path;
  _type = type;
  _frontendConnected = frontendConnected;
}

INode::~INode() {
  //Function pointers need to be cleaned up before unloading the module
  _log = std::function<void(std::string, int32_t, std::string)>();
  _subscribePeer = std::function<void(std::string, uint64_t, int32_t, std::string)>();
  _unsubscribePeer = std::function<void(std::string, uint64_t, int32_t, std::string)>();
  _subscribeFlow = std::function<void(std::string, std::string)>();
  _unsubscribeFlow = std::function<void(std::string, std::string)>();
  _subscribeGlobal = std::function<void(std::string)>();
  _unsubscribeGlobal = std::function<void(std::string)>();
  _output = std::function<void(std::string, uint32_t, PVariable, bool)>();
  _invoke = std::function<PVariable(std::string, PArray)>();
  _invokeNodeMethod = std::function<PVariable(std::string, std::string, PArray, bool)>();
  _nodeEvent = std::function<void(std::string, std::string, const PVariable &, bool)>();
  _getNodeData = std::function<PVariable(std::string, std::string)>();
  _setNodeData = std::function<void(std::string, std::string, PVariable)>();
  _getConfigParameter = std::function<PVariable(std::string, std::string)>();
}

std::string INode::version() {
  return VERSION;
}

void INode::setLog(std::function<void(const std::string &, int32_t, const std::string &)> value) {
  _log.swap(value);
  if (_out) _out->setLog(&_log);
}

void INode::log(int32_t logLevel, const std::string &message) {
  if (_log) _log(_id, logLevel, message);
}

void INode::frontendEventLog(const std::string &message) {
  if (_frontendEventLog) _frontendEventLog(_id, message);
}

void INode::subscribePeer(uint64_t peerId, int32_t channel, const std::string &variable) {
  if (_subscribePeer) _subscribePeer(_id, peerId, channel, variable);
}

void INode::unsubscribePeer(uint64_t peerId, int32_t channel, const std::string &variable) {
  if (_unsubscribePeer) _unsubscribePeer(_id, peerId, channel, variable);
}

void INode::subscribeFlow() {
  if (_subscribeFlow) _subscribeFlow(_id, _flowId);
}

void INode::unsubscribeFlow() {
  if (_unsubscribeFlow) _unsubscribeFlow(_id, _flowId);
}

void INode::subscribeGlobal() {
  if (_subscribeGlobal) _subscribeGlobal(_id);
}

void INode::unsubscribeGlobal() {
  if (_unsubscribeGlobal) _unsubscribeGlobal(_id);
}

void INode::subscribeHomegearEvents() {
  if (_subscribeHomegearEvents) _subscribeHomegearEvents(_id);
}

void INode::unsubscribeHomegearEvents() {
  if (_unsubscribeHomegearEvents) _unsubscribeHomegearEvents(_id);
}

void INode::subscribeStatusEvents() {
  if (_subscribeStatusEvents) _subscribeStatusEvents(_id);
}

void INode::unsubscribeStatusEvents() {
  if (_unsubscribeStatusEvents) _unsubscribeStatusEvents(_id);
}

void INode::subscribeErrorEvents() {
  if (_subscribeErrorEvents) _subscribeErrorEvents(_id);
}

void INode::unsubscribeErrorEvents() {
  if (_unsubscribeErrorEvents) _unsubscribeErrorEvents(_id);
}

void INode::output(uint32_t index, PVariable message, bool synchronous) {
  if (_output) _output(_id, index, std::move(message), synchronous);
}

PVariable INode::invoke(const std::string &methodName, PArray parameters) {
  if (_invoke) return _invoke(methodName, std::move(parameters));
  return Variable::createError(-32500, "No callback method set.");
}

PVariable INode::invokeNodeMethod(const std::string &nodeId, const std::string &methodName, PArray parameters, bool wait) {
  if (_invokeNodeMethod) return _invokeNodeMethod(nodeId, methodName, std::move(parameters), wait);
  return Variable::createError(-32500, "No callback method set.");
}

PVariable INode::invokeLocal(const std::string &methodName, const PArray &parameters) {
  auto localMethodIterator = _localRpcMethods.find(methodName);
  if (localMethodIterator == _localRpcMethods.end()) {
    _out->printError("Warning: RPC method not found: " + methodName);
    return Variable::createError(-32601, ": Requested method not found.");
  }

  return localMethodIterator->second(parameters);
}

void INode::nodeEvent(const std::string &topic, const PVariable &value, bool retain) {
  if (_nodeEvent) _nodeEvent(_id, topic, value, retain);
}

PVariable INode::getNodeData(const std::string &key) {
  if (_getNodeData) return _getNodeData(_id, key);
  return Variable::createError(-32500, "No callback method set.");
}

void INode::setNodeData(const std::string &key, PVariable value) {
  if (_setNodeData) _setNodeData(_id, key, std::move(value));
}

PVariable INode::getFlowData(const std::string &key) {
  if (_getFlowData) return _getFlowData(_flowId, key);
  return Variable::createError(-32500, "No callback method set.");
}

void INode::setFlowData(const std::string &key, PVariable value) {
  if (_setFlowData) _setFlowData(_flowId, key, std::move(value));
}

PVariable INode::getGlobalData(const std::string &key) {
  if (_getGlobalData) return _getGlobalData(key);
  return Variable::createError(-32500, "No callback method set.");
}

void INode::setGlobalData(const std::string &key, PVariable value) {
  if (_setGlobalData) _setGlobalData(key, std::move(value));
}

void INode::setInternalMessage(PVariable message) {
  if (_setInternalMessage) _setInternalMessage(_id, std::move(message));
}

PVariable INode::getConfigParameter(const std::string &nodeId, const std::string &name) {
  if (_getConfigParameter) return _getConfigParameter(nodeId, name);
  return Variable::createError(-32500, "No callback method set.");
}

PVariable INode::getNodeVariable(const std::string &variable) {
  return Flows::PVariable();
}

void INode::setNodeVariable(const std::string &variable, const PVariable &value) {
}

}
