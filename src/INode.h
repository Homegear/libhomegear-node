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

#ifndef INODE_H_
#define INODE_H_

#include "Variable.h"
#include "NodeInfo.h"
#include "Output.h"
#include "HelperFunctions.h"
#include "Math.h"

#include <atomic>
#include <string>
#include <memory>

namespace Flows
{

class INode
{
public:
	INode(std::string path, std::string name, const std::atomic_bool* nodeEventsEnabled);
	virtual ~INode();

	std::string getName() { return _name; }
	std::string getPath() { return _path; }
	std::string getId() { return _id; }
	void setId(std::string value) { _id = value; }

	virtual bool init(PNodeInfo nodeInfo) { return true; };
	virtual bool start() { return true; }
	virtual void stop() {}

	virtual void configNodesStarted() {}

	virtual void variableEvent(uint64_t peerId, int32_t channel, std::string variable, PVariable value) {}
	virtual void setNodeVariable(std::string& variable, PVariable& value) {}

	virtual PVariable getConfigParameterIncoming(std::string name) { return std::make_shared<Flows::Variable>(); }

	void setLog(std::function<void(std::string, int32_t, std::string)> value);
	void setSubscribePeer(std::function<void(std::string, uint64_t, int32_t, std::string)> value) { _subscribePeer.swap(value); }
	void setUnsubscribePeer(std::function<void(std::string, uint64_t, int32_t, std::string)> value) { _unsubscribePeer.swap(value); }
	void setOutput(std::function<void(std::string, uint32_t, PVariable)> value) { _output.swap(value); }
	void setInvoke(std::function<PVariable(std::string, PArray&)> value) { _invoke.swap(value); }
	void setInvokeNodeMethod(std::function<PVariable(std::string, std::string, PArray&)> value) { _invokeNodeMethod.swap(value); }
	void setNodeEvent(std::function<void(std::string, std::string, PVariable)> value) { _nodeEvent.swap(value); }
	void setGetNodeData(std::function<PVariable(std::string, std::string)> value) { _getNodeData.swap(value); }
	void setSetNodeData(std::function<void(std::string, std::string, PVariable)> value) { _setNodeData.swap(value); }
	void setGetConfigParameter(std::function<PVariable(std::string, std::string)> value) { _getConfigParameter.swap(value); }

	virtual void input(PNodeInfo nodeInfo, uint32_t index, PVariable message) {}

	/*
	 * Executes local RPC method
	 */
	PVariable invokeLocal(std::string methodName, PArray& parameters);
protected:
	std::string _path;
	std::string _name;
	std::string _id;
	const std::atomic_bool* _nodeEventsEnabled;

	/*
	 * Stores RPC methods for inter-node communication (intended for configuration nodes)
	 */
	std::map<std::string, std::function<PVariable(PArray& parameters)>> _localRpcMethods;

	void log(int32_t logLevel, std::string message);
	void subscribePeer(uint64_t peerId, int32_t channel = -1, std::string variable = "");
	void unsubscribePeer(uint64_t peerId, int32_t channel = -1, std::string variable = "");
	void output(uint32_t outputIndex, PVariable message);
	PVariable invoke(std::string methodName, PArray& parameters);
	PVariable invokeNodeMethod(std::string nodeId, std::string methodName, PArray& parameters);
	void nodeEvent(std::string topic, PVariable& value);
	PVariable getNodeData(std::string key);
	void setNodeData(std::string key, PVariable value);
	PVariable getConfigParameter(std::string nodeId, std::string name);
private:
	std::atomic_bool _locked;
	std::atomic_int _referenceCounter;
	std::function<void(std::string, int32_t, std::string)> _log;
	std::function<void(std::string, uint64_t, int32_t, std::string)> _subscribePeer;
	std::function<void(std::string, uint64_t, int32_t, std::string)> _unsubscribePeer;
	std::function<void(std::string, uint32_t, PVariable)> _output;
	std::function<PVariable(std::string, PArray&)> _invoke;
	std::function<PVariable(std::string, std::string, PArray&)> _invokeNodeMethod;
	std::function<void(std::string, std::string, PVariable)> _nodeEvent;
	std::function<PVariable(std::string, std::string)> _getNodeData;
	std::function<void(std::string, std::string, PVariable)> _setNodeData;
	std::function<PVariable(std::string, std::string)> _getConfigParameter;

	INode(const INode&) = delete;
	INode& operator=(const INode&) = delete;
};

typedef std::shared_ptr<INode> PINode;

}
#endif
