AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Wall -std=c++11 -DFORTIFY_SOURCE=2 -DGCRYPT_NO_DEPRECATED
AM_LDFLAGS = -Wl,-rpath=/lib/homegear -Wl,-rpath=/usr/lib/homegear -Wl,-rpath=/usr/local/lib/homegear

lib_LTLIBRARIES = libhomegear-node.la
libhomegear_node_la_SOURCES = BinaryDecoder.cpp BinaryEncoder.cpp BinaryRpc.cpp HelperFunctions.cpp INode.cpp RpcDecoder.cpp RpcEncoder.cpp Variable.cpp
libhomegear_node_la_LDFLAGS = -version-info 1:0:0

otherincludedir = $(includedir)/homegear-node
nobase_otherinclude_HEADERS = BinaryDecoder.h BinaryEncoder.h BinaryRpc.h FlowException.h HelperFunctions.h INode.h NodeInfo.h NodeFactory.h RpcDecoder.h RpcEncoder.h RpcHeader.h Variable.h
