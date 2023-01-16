/******************************************************************
 *  ____       __                        __    __   __      _ __
 * /_  / ___ _/ /  ___  ___ ___________ / /__ / /__/ /_____(_) /__
 *  / /_/ _ `/ _ \/ _ \/ -_) __/___/ -_) / -_)  '_/ __/ __/ /  '_/
 * /___/\_,_/_//_/_//_/\__/_/      \__/_/\__/_/\_\\__/_/ /_/_/\_\
 *
 * Copyright 2023 ZAHNER-elektrik I. Zahner-Schiller GmbH & Co. KG
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef THALESREMOTECONNECTION_H
#define THALESREMOTECONNECTION_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <queue>
#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "threadsafequeue.h"
#include <memory>

#ifdef _WIN32

#define MSG_MORE 0
#define SHUT_RD 0

#ifdef _MSC_VER
#define NOMINMAX //Necessary for MSVC Compiler.
#endif

#include <ws2tcpip.h>
#include <windows.h>

#else

#define SOCKET int
#define INVALID_SOCKET -1

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#endif

class ZenniumConnection
{
public:

    ZenniumConnection();
    ~ZenniumConnection();

    /** Connect to Term Software(The Thales Terminal)
     *
     * \param  address The hostname or ip-address of the host running Term.
     * \param  connectionName The name of the connection ScriptRemote for Remote and Logging as Online Display.
     * \return true on success, false if failed
     */
    bool connectToTerm(std::string address, std::string connectionName);

    /** Close the connection to Term and cleanup.
     *
     * Stops the thread used for receiving telegrams assynchronously and shuts down
     * the network connection.
     */
    void disconnectFromTerm();

    /** Check if the connection to Term is open.
     *
     * \return true if connected, false if not.
     */
    bool isConnectedToTerm() const;

    /** Send a telegram (data) to Term.
     *
     * \param  payload The actual data which is being sent to Term.
     * \param  message_type Used internally by the DevCli dll. Depends on context. Most of the time 2.
     */
    void sendTelegram(std::string payload, int message_type);

    /** Send a telegram (data) to Term.
     *
     * \param  payload The actual data which is being sent to Term.
     * \param  message_type Used internally by the DevCli dll. Depends on context. Most of the time 2.
     */
    void sendTelegram(std::vector<unsigned char> payload, int message_type);

    std::string waitForStringTelegram(int message_type);
    /** Block maximal timeout milliseconds while waiting for an incoming telegram.
     *
     * If some Telegram has already arrived it will just return the last one from the queue.
     *
     * \param  message_type Used internally by the DevCli dll. Depends on context. Most of the time 2.
     * \param  answer_message_typ Message type which is used for the response. Mostly like the send number 2.
     *          If the overloaded method is used without this parameter, then the send number is used.
     * \param  timeout Timeout in milliseconds for waiting for the response.
     *          If an overloaded method without timeout is used, then the default timeout is used.
     *
     * \return The last received telegram or an empty string if the timeout was reached or something went wrong.
     */
    std::string waitForStringTelegram(int message_type, const std::chrono::duration<int, std::milli> timeout);

    std::vector<uint8_t> waitForTelegram(int message_type);
    /** Block maximal timeout milliseconds while waiting for an incoming telegram.
     *
     * If some Telegram has already arrived it will just return the last one from the queue.
     *
     * \param  message_type Used internally by the DevCli dll. Depends on context. Most of the time 2.
     * \param  timeout Timeout in milliseconds for waiting for the response.
     *          If an overloaded method without timeout is used, then the default timeout is used.
     *
     * \return The last received telegram or an empty string if the timeout was reached or something went wrong.
     */
    std::vector<uint8_t> waitForTelegram(int message_type, const std::chrono::duration<int, std::milli> timeout);

    /** Immediately return the last received telegram.
     *
     * \return The last received telegram or an empty string if no telegram was received or something went wrong.
     */
    std::string receiveStringTelegram();

    /** Immediately return the last received telegram.
     *
     * \return The last received telegram or an empty string if no telegram was received or something went wrong.
     */
    std::vector<uint8_t> receiveTelegram();

    std::string sendStringAndWaitForReplyString(std::string payload,
                                                int message_type);
    std::string sendStringAndWaitForReplyString(std::string payload,
                                                int message_type,
                                                const std::chrono::duration<int, std::milli> timeout);
    /** Convenience function: Send a telegram and wait for it's reply.
     *
     * \param  payload The actual data which is being sent to Term.
     * \param  message_type Used internally by the DevCli dll. Depends on context. Most of the time 2.
     * \param  timeout Timeout in milliseconds for waiting for the response.
     *          If an overloaded method without timeout is used, then the default timeout is used.
     * \param  answer_message_typ Message type which is used for the response. Mostly like the send number 2.
     *          If the overloaded method is used without this parameter, then the send number is used.
     *
     * \return The last received telegram or an empty string if someting went wrong.
     */
    std::string sendStringAndWaitForReplyString(std::string payload,
                                                int message_type,
                                                const std::chrono::duration<int, std::milli> timeout,
                                                int answer_message_typ);

    /** Get the used name of the connection.
     *
     * \return The name of the connection.
     */
    std::string getConnectionName();

    /** Set the default timeout.
     *
     *  If no default timeout was passed to the methods, then the default timeout used with this method is used.
     *  If this method has never been called the timeout is std::chrono::duration<int, std::milli>::max().
     *
     *  When the timout time has expired the wait function for the response from the Term Software is exited and a TermConnectionError exception is thrown.
     *
     * \param  timeout The default timeout.
     */
    void setTimeout(const std::chrono::duration<int, std::milli> timeout);

    /** Get the timout time.
     *
     *  If no default timeout was passed to the methods, then the default timeout used with this method is used.
     *  If this method has never been called the timeout is std::chrono::duration<int, std::milli>::max().
     *
     *  When the timout time has expired the wait function for the response from the Term Software is exited and a TermConnectionError exception is thrown.
     *
     * \return The set timout.
     */
    std::chrono::duration<int, std::milli> getTimeout();

protected:
    std::chrono::duration<int, std::milli> defaultTimeout;

    static const int term_port = 260;
    std::string connectionName;

    SOCKET socket_handle;

    std::vector<int> availableChannels;

    std::unordered_map<int, std::shared_ptr<ThreadsafeQueue>> queuesForChannels;

    bool receiving_worker_is_running;
    std::thread *receivingWorker;

    /** The method running in a separate thread, pushing the incomming packets into the queue. */
    void telegramListenerJob();

    /** Starts the thread handling the asyncronously incoming data. */
    void startTelegramListener();

    /** Stops the thread handling the incoming data gracefully. */
    void stopTelegramListener();

    /** Reads the raw telegram structure from the socket stream.
     *
     *  If the connection was disconnected, an empty array is returned.
     */
    std::tuple<int, std::vector<uint8_t>> readTelegramFromSocket();

    /** Helper function getting the current time in milliseconds. */
    std::chrono::milliseconds getCurrentTimeInMilliseconds() const;

    void closeSocket();

};

#endif // THALESREMOTECONNECTION_H
