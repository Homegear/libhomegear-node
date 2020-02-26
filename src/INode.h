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

#ifndef INODE_H_
#define INODE_H_

#include "Variable.h"
#include "NodeInfo.h"
#include "Output.h"
#include "HelperFunctions.h"
#include "Math.h"

#include <atomic>
#include <mutex>
#include <string>
#include <memory>

namespace Flows
{

class INode
{
public:
	INode(std::string path, std::string nodeNamespace, std::string type, const std::atomic_bool* frontendConnected);
    INode(const INode&) = delete;
    INode& operator=(const INode&) = delete;
	virtual ~INode();

    static std::string version();
	std::string getNamespace() { return _namespace; }
	std::string getType() { return _type; }
	std::string getPath() { return _path; }
	std::string getFlowId() { return _flowId; }
	void setFlowId(const std::string& value) { _flowId = value; }
	std::string getId() { return _id; }
	void setId(std::string value) { _id = value; if(_out) _out->setNodeId(value); }
	std::mutex& getInputMutex() { return _inputMutex; }

	virtual bool init(PNodeInfo nodeInfo) { return true; };
	virtual bool start() { return true; }

	/**
	 * Mustn't block. Set variables causing threads to finish here. After stop() is called for all nodes, waitForStop() is called() where threads can be joined.
	 */
	virtual void stop() {}

	/**
	 * Wait here until everything is cleaned up. Keep the waiting as short as possible as this method is called serially and synchronouly for all nodes. Join threads here.
	 */
	virtual void waitForStop() {}

	/**
	 * Mustn't block.
	 */
	virtual void configNodesStarted() {}

	/**
	 * Mustn't block.
	 */
	virtual void startUpComplete() {}

	/**
	 * Mustn't block.
	 */
	virtual void variableEvent(std::string source, uint64_t peerId, int32_t channel, std::string variable, PVariable value) {}

    /**
     * Mustn't block.
     */
    virtual void flowVariableEvent(std::string flowId, std::string variable, PVariable value) {}

    /**
     * Mustn't block.
     */
    virtual void globalVariableEvent(std::string variable, PVariable value) {}

    /**
     * Mustn't block.
     */
    virtual void homegearEvent(const std::string& type, const PArray& data) {}

    /**
	 * Mustn't block.
	 */
    virtual PVariable getNodeVariable(const std::string& variable);

	/**
	 * Mustn't block.
	 */
	virtual void setNodeVariable(const std::string& variable, PVariable value);

	/**
	 * Mustn't block.
	 */
	virtual PVariable getConfigParameterIncoming(std::string name) { return std::make_shared<Flows::Variable>(); }

	// {{{ Internal methods
		void setLog(std::function<void(const std::string&, int32_t, const std::string&)> value);
		void setSubscribePeer(std::function<void(const std::string&, uint64_t, int32_t, const std::string&)> value) { _subscribePeer.swap(value); }
		void setUnsubscribePeer(std::function<void(const std::string&, uint64_t, int32_t, const std::string&)> value) { _unsubscribePeer.swap(value); }
        void setSubscribeFlow(std::function<void(const std::string&, const std::string&)> value) { _subscribeFlow.swap(value); }
        void setUnsubscribeFlow(std::function<void(const std::string&, const std::string&)> value) { _unsubscribeFlow.swap(value); }
        void setSubscribeGlobal(std::function<void(const std::string&)> value) { _subscribeGlobal.swap(value); }
        void setUnsubscribeGlobal(std::function<void(const std::string&)> value) { _unsubscribeGlobal.swap(value); }
        void setSubscribeHomegearEvents(std::function<void(const std::string&)> value) { _subscribeHomegearEvents.swap(value); }
        void setUnsubscribeHomegearEvents(std::function<void(const std::string&)> value) { _unsubscribeHomegearEvents.swap(value); }
		void setOutput(std::function<void(const std::string&, uint32_t, PVariable, bool)> value) { _output.swap(value); }
		void setInvoke(std::function<PVariable(const std::string&, PArray)> value) { _invoke.swap(value); }
		void setInvokeNodeMethod(std::function<PVariable(const std::string&, const std::string&, PArray, bool)> value) { _invokeNodeMethod.swap(value); }
		void setNodeEvent(std::function<void(const std::string&, const std::string&, PVariable)> value) { _nodeEvent.swap(value); }
		void setGetNodeData(std::function<PVariable(const std::string&, const std::string&)> value) { _getNodeData.swap(value); }
		void setSetNodeData(std::function<void(const std::string&, const std::string&, PVariable)> value) { _setNodeData.swap(value); }
        void setGetFlowData(std::function<PVariable(const std::string&, const std::string&)> value) { _getFlowData.swap(value); }
        void setSetFlowData(std::function<void(const std::string&, const std::string&, PVariable)> value) { _setFlowData.swap(value); }
        void setGetGlobalData(std::function<PVariable(const std::string&)> value) { _getGlobalData.swap(value); }
        void setSetGlobalData(std::function<void(const std::string&, PVariable)> value) { _setGlobalData.swap(value); }
		void setSetInternalMessage(std::function<void(const std::string&, PVariable)> value) { _setInternalMessage.swap(value); }
		void setGetConfigParameter(std::function<PVariable(const std::string&, const std::string&)> value) { _getConfigParameter.swap(value); }
	// }}}

	/**
	 * Mustn't block. When it blocks for a longer time, consider using IQueue.
	 *
	 * @param nodeInfo The info structure of this node. Mustn't be changed.
	 * @param index The index of the input starting with 0.
	 * @param message The message structure. Mustn't be changed.
	 */
	virtual void input(const PNodeInfo nodeInfo, uint32_t index, const PVariable message) {}

	/**
	 * Executes local RPC method. Mustn't block.
	 */
	virtual PVariable invokeLocal(const std::string& methodName, PArray parameters);
protected:
    std::shared_ptr<Output> _out;
	std::string _path;
	std::string _namespace;
	std::string _type;
	std::string _flowId;
	std::string _id;
	const std::atomic_bool* _frontendConnected;

	/**
	 * Stores RPC methods for inter-node communication (intended for configuration nodes)
	 */
	std::map<std::string, std::function<PVariable(PArray parameters)>> _localRpcMethods;

	void log(int32_t logLevel, const std::string& message);
    void frontendEventLog(const std::string& message);
	void subscribePeer(uint64_t peerId, int32_t channel = -1, const std::string& variable = "");
	void unsubscribePeer(uint64_t peerId, int32_t channel = -1, const std::string& variable = "");
    void subscribeFlow();
    void unsubscribeFlow();
    void subscribeGlobal();
    void unsubscribeGlobal();
    void subscribeHomegearEvents();
    void unsubscribeHomegearEvents();
	void output(uint32_t outputIndex, PVariable message, bool synchronous = false);
	PVariable invoke(const std::string& methodName, PArray parameters);
	PVariable invokeNodeMethod(const std::string& nodeId, const std::string& methodName, PArray parameters, bool);
	void nodeEvent(const std::string& topic, PVariable value);
	PVariable getNodeData(const std::string& key);
	void setNodeData(const std::string& key, PVariable value);
	PVariable getFlowData(const std::string& key);
	void setFlowData(const std::string& key, PVariable value);
	PVariable getGlobalData(const std::string& key);
	void setGlobalData(const std::string& key, PVariable value);
	void setInternalMessage(PVariable message);
	PVariable getConfigParameter(const std::string& nodeId, const std::string& name);
private:
	std::atomic_bool _locked;
	std::atomic_int _referenceCounter;
	std::mutex _inputMutex;
	std::function<void(const std::string&, int32_t, const std::string&)> _log;
    std::function<void(const std::string&, const std::string&)> _frontendEventLog;
	std::function<void(const std::string&, uint64_t, int32_t, const std::string&)> _subscribePeer;
	std::function<void(const std::string&, uint64_t, int32_t, const std::string&)> _unsubscribePeer;
    std::function<void(const std::string&, const std::string&)> _subscribeFlow;
    std::function<void(const std::string&, const std::string&)> _unsubscribeFlow;
    std::function<void(const std::string&)> _subscribeGlobal;
    std::function<void(const std::string&)> _unsubscribeGlobal;
    std::function<void(const std::string&)> _subscribeHomegearEvents;
    std::function<void(const std::string&)> _unsubscribeHomegearEvents;
	std::function<void(const std::string&, uint32_t, PVariable, bool)> _output;
	std::function<PVariable(const std::string&, PArray)> _invoke;
	std::function<PVariable(const std::string&, const std::string&, PArray, bool)> _invokeNodeMethod;
	std::function<void(const std::string&, const std::string&, PVariable)> _nodeEvent;
	std::function<PVariable(const std::string&, const std::string&)> _getNodeData;
	std::function<void(const std::string&, const std::string&, PVariable)> _setNodeData;
    std::function<PVariable(const std::string&, const std::string&)> _getFlowData;
    std::function<void(const std::string&, const std::string&, PVariable)> _setFlowData;
    std::function<PVariable(const std::string&)> _getGlobalData;
    std::function<void(const std::string&, PVariable)> _setGlobalData;
	std::function<void(const std::string&, PVariable)> _setInternalMessage;
	std::function<PVariable(const std::string&, const std::string&)> _getConfigParameter;
};

typedef std::shared_ptr<INode> PINode;

}
#endif
