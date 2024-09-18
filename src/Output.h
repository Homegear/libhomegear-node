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

#ifndef NODEOUTPUT_H_
#define NODEOUTPUT_H_

#include <string>
#include <sstream>
#include <functional>
#include <iomanip>
#include <cstdint>

namespace Flows {
/**
 * Class to print output of different kinds to the standard and error output.
 * The output is automatically prefixed with the date and filtered according to the current debug level.
 */
class Output {
 public:
  /**
   * The main constructor.
   */
  Output(std::string &nodeId, std::function<void(const std::string &, int32_t, const std::string &)> *logMethod);

  /**
   * The destructor.
   * It does nothing.
   */
  virtual ~Output() = default;

  /**
   * Sets _nodeId.
   *
   * @param value The new node ID.
   */
  void setNodeId(const std::string &value) { _nodeId = value; }

  /**
   * Sets _log.
   *
   * @param value The new log function.
   */
  void setLog(std::function<void(const std::string &, int32_t, const std::string &)> *value) { _log = value; }

  /**
   * Prints an error message with filename, line number and function name.
   *
   * @param file The name of the file where the error occured.
   * @param line The line number where the error occured.
   * @param function The function name where the error occured.
   * @param what The error message.
   */
  void printEx(const std::string& file, uint32_t line, const std::string& function, const std::string& what = "");

  /**
   * Prints a critical error message (debug level < 1).
   *
   * @see printError()
   * @see printWarning()
   * @see printInfo()
   * @see printDebug()
   * @see printMessage()
   * @param errorString The error message.
   * @param errorCallback If set to false, the error will not be send to RPC event servers (default true)
   */
  void printCritical(const std::string& errorString, bool errorCallback = true);

  /**
   * Prints an error message (debug level < 2).
   *
   * @see printCritical()
   * @see printWarning()
   * @see printInfo()
   * @see printDebug()
   * @see printMessage()
   * @param errorString The error message.
   */
  void printError(const std::string& errorString);

  /**
   * Prints a warning message (debug level < 3).
   *
   * @see printCritical()
   * @see printError()
   * @see printInfo()
   * @see printDebug()
   * @see printMessage()
   * @param errorString The warning message.
   */
  void printWarning(const std::string& errorString);

  /**
   * Prints a info message (debug level < 4).
   *
   * @see printCritical()
   * @see printError()
   * @see printWarning()
   * @see printDebug()
   * @see printMessage()
   * @param message The message.
   */
  void printInfo(const std::string& message);

  /**
   * Prints a debug message (debug level < 5).
   *
   * @see printCritical()
   * @see printError()
   * @see printWarning()
   * @see printInfo()
   * @see printMessage()
   * @param message The message.
   * @param minDebugLevel The minimal debug level (default 5).
   */
  void printDebug(const std::string& message, int32_t minDebugLevel = 5);

  /**
   * Prints a message regardless of the current debug level.
   *
   * @see printCritical()
   * @see printError()
   * @see printWarning()
   * @see printInfo()
   * @see printDebug()
   * @param message The message.
   * @param minDebugLevel The minimal debug level (default 0).
   */
  void printMessage(const std::string& message, int32_t minDebugLevel = 0);

  /**
   * Calls the error callback function registered with the constructor.
   */
 private:
  std::string _nodeId;
  std::function<void(const std::string &, int32_t, const std::string &)> *_log = nullptr;
};
}
#endif
