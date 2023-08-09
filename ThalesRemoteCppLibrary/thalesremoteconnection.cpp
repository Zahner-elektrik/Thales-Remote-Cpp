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

#include "thalesremoteconnection.h"
#include "termconnectionerror.h"

ZenniumConnection::ZenniumConnection() :

    defaultTimeout(std::chrono::duration<int, std::milli>::max()),
    socket_handle(INVALID_SOCKET),
    receiving_worker_is_running(false),
    receivingWorker(nullptr)
{
    this->availableChannels = {2,128,129,130,131,132};

    for(int channel : this->availableChannels)
    {
        std::shared_ptr<ThreadsafeQueue> p = std::make_shared<ThreadsafeQueue>();
        this->queuesForChannels.insert({channel, p});
    }

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

}

ZenniumConnection::~ZenniumConnection()
{

#ifdef _WIN32
    WSACleanup();
#endif

}

bool ZenniumConnection::connectToTerm(std::string address, std::string connectionName)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    this->socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    this->connectionName = connectionName;

#ifdef _WIN32
    if (this->socket_handle == INVALID_SOCKET) {
#else
    if (this->socket_handle < 0) {
#endif

        throw TermConnectionError("Could not create socket");
        return false;
    }

    struct addrinfo hints = {};
    struct addrinfo *result_pointer;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(address.data(), "260", &hints, &result_pointer) != 0)
    {
        this->closeSocket();
        throw TermConnectionError("Error while resolving address");
        return false;
    }

    struct addrinfo *first_addr = result_pointer;

    if (first_addr == nullptr)
    {
        freeaddrinfo(result_pointer);
        this->closeSocket();
        throw TermConnectionError("Could not resolve hostname");
        return false;
    }

    if (connect(this->socket_handle, first_addr->ai_addr, static_cast<int>(first_addr->ai_addrlen)) < 0)
    {
        this->closeSocket();
        throw TermConnectionError("Could not connect to term");
        return false;
    }

    this->startTelegramListener();

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    unsigned short payload_length = static_cast<unsigned short>(connectionName.length());

    std::vector<unsigned char> registration_packet;

    registration_packet.reserve(payload_length + 6);

    registration_packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[1]);
    registration_packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[0]);

    const std::vector<unsigned char> fixedHeaderBytes =
    {
        0x12, 0xd0,
        0xff, 0xff,
        0xff, 0xff
    };

    registration_packet.insert(registration_packet.end(), fixedHeaderBytes.begin(), fixedHeaderBytes.end());

    std::copy(connectionName.begin(), connectionName.end(), std::back_inserter(registration_packet));
    sendall(this->socket_handle, reinterpret_cast<char *>(registration_packet.data()), static_cast<int>(registration_packet.size()), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    return true;
}

void ZenniumConnection::disconnectFromTerm()
{

    try
    {
        this->sendStringAndWaitForReplyString("3," + this->connectionName + ",0,RS",0x80);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        this->sendTelegram("\xff\xff", 4);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }  catch (...){}
    this->stopTelegramListener();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    this->closeSocket();
}

bool ZenniumConnection::isConnectedToTerm() const
{

#ifdef _WIN32
    return (this->socket_handle != INVALID_SOCKET);
#else
    return (this->socket_handle > 0);
#endif

}

int ZenniumConnection::sendall(SOCKET s, char *data, int dataSize, int flags)
{
    int totalSent = 0; // How much we've sent already
    int bytesLeft = dataSize; // How much we still need to send
    int n;

    int attempts = 0; // Counts the number of attempts

    while(totalSent < dataSize) {
        n = send(s, data + totalSent, bytesLeft, flags);
        if (n == -1) { break; }
        totalSent += n;
        bytesLeft -= n;

        attempts++;
        if(attempts >= dataSize) {
            //something is wrong
            break;
        }
    }

    return (n==-1) ? -1 : totalSent; // Return -1 on error, otherwise return the number of bytes sent
}

void ZenniumConnection::sendTelegram(std::string payload, int message_type)
{
    std::vector<unsigned char> bytes(payload.begin(), payload.end());
    this->sendTelegram(bytes, message_type);
}

void ZenniumConnection::sendTelegram(std::vector<unsigned char> payload, int message_type)
{

    std::vector<unsigned char> packet;
    uint16_t payload_length = static_cast<uint16_t>(payload.size());

    packet.reserve(payload_length + 3);
    packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[0]);
    packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[1]);
    packet.push_back(message_type);

    packet.insert(packet.end(), payload.begin(), payload.end());

    int status = sendall(this->socket_handle, reinterpret_cast<char *>(packet.data()), static_cast<int>(packet.size()), 0);

    if(status == -1)
    {
        throw TermConnectionError("Socket error during data transmission.");
    }
}

std::vector<uint8_t> ZenniumConnection::waitForTelegram(int message_type)
{
    return this->waitForTelegram(message_type, this->defaultTimeout);
}

std::vector<uint8_t> ZenniumConnection::waitForTelegram(int message_type, const std::chrono::duration<int, std::milli> timeout) {

    auto receivedTelegram = this->queuesForChannels[message_type]->get(true, timeout);


    if(receivedTelegram.size() == 0)
    {
        throw TermConnectionError("Empty telegram received.");
    }
    return receivedTelegram;
}

std::string ZenniumConnection::waitForStringTelegram(int message_type)
{
    return this->waitForStringTelegram(message_type, this->defaultTimeout);
}

std::string ZenniumConnection::waitForStringTelegram(int message_type, const std::chrono::duration<int, std::milli> timeout)
{

    std::vector<uint8_t> telegram = waitForTelegram(message_type, timeout);

    return std::string(reinterpret_cast<char *>(telegram.data()), telegram.size());
}

bool ZenniumConnection::isTelegramAvailable(int message_type)
{
    auto empty = this->queuesForChannels[message_type]->empty();
    return !empty;
}

std::string ZenniumConnection::sendStringAndWaitForReplyString(std::string payload, int message_type)
{
    return this->sendStringAndWaitForReplyString(payload, message_type, this->defaultTimeout, message_type);
}

std::string ZenniumConnection::sendStringAndWaitForReplyString(std::string payload, int message_type, const std::chrono::duration<int, std::milli> timeout)
{
    return this->sendStringAndWaitForReplyString(payload,message_type,timeout,message_type);
}

std::string ZenniumConnection::sendStringAndWaitForReplyString(std::string payload, int message_type, const std::chrono::duration<int, std::milli> timeout, int answer_message_type)
{
    this->sendTelegram(payload, message_type);
    return this->waitForStringTelegram(answer_message_type, timeout);
}

std::string ZenniumConnection::getConnectionName()
{
    return this->connectionName;
}

void ZenniumConnection::setTimeout(const std::chrono::duration<int, std::milli> timeout)
{
    this->defaultTimeout = timeout;
}

std::chrono::duration<int, std::milli> ZenniumConnection::getTimeout()
{
    return this->defaultTimeout;
}

std::tuple<int, std::vector<uint8_t>> ZenniumConnection::readTelegramFromSocket()
{
    bool connectionInterrupted = false;
    std::vector<uint8_t> incoming_packet;
#ifdef _WIN32
    int received_bytes;
    int total_received_bytes = 0;
#else
    ssize_t received_bytes;
    size_t total_received_bytes = 0;
#endif

    char header_bytes[3];
    unsigned short *length_data = reinterpret_cast<unsigned short*>(header_bytes);

    do
    {
        received_bytes = recv(this->socket_handle, &header_bytes[total_received_bytes], 3 - total_received_bytes, 0);

        if (received_bytes == 0) {

            return {-1,std::vector<uint8_t>()};
        } else if (received_bytes == -1) {

            connectionInterrupted = true;
        }

#ifdef _WIN32
        total_received_bytes += received_bytes;
#else
        total_received_bytes += static_cast<size_t>(received_bytes);
#endif

    } while ((total_received_bytes < 3) && (connectionInterrupted == false));

    if(connectionInterrupted == false)
    {
        incoming_packet.resize(*length_data);

        total_received_bytes = 0;

        do
        {
            received_bytes = recv(this->socket_handle, reinterpret_cast<char *>(incoming_packet.data() + total_received_bytes), *length_data - total_received_bytes, 0);

            if (received_bytes == 0)
            {
                return {-1,std::vector<uint8_t>()};
            } else if (received_bytes == -1)
            {
                connectionInterrupted = true;
            }

#ifdef _WIN32
            total_received_bytes += received_bytes;
#else
            total_received_bytes += static_cast<size_t>(received_bytes);
#endif
        } while ((total_received_bytes < *length_data) && (connectionInterrupted == false));

    }
    if(connectionInterrupted == true)
    {
        incoming_packet.clear();
    }
    return {static_cast<unsigned char>(header_bytes[2]),incoming_packet};
}

void ZenniumConnection::telegramListenerJob()
{
    do {
        auto telegram = readTelegramFromSocket();

        if (std::get<1>(telegram).size() > 0 && std::find(availableChannels.begin(), availableChannels.end(), std::get<0>(telegram)) != availableChannels.end())
        {
            this->queuesForChannels[std::get<0>(telegram)]->put(std::get<1>(telegram));
        }
        else if(std::get<0>(telegram) == -1)
        {
            /*
             * Error:
             * To free the waiting receive threads, the Empty Telegram is put into the queue.
             * The receive thread is then terminated.
             */
            for(int channel : availableChannels)
            {
                this->queuesForChannels[channel]->put(std::vector<uint8_t>());
            }
            this->receiving_worker_is_running = false;
        }

    } while (this->receiving_worker_is_running);
}

void ZenniumConnection::startTelegramListener()
{

    this->receiving_worker_is_running = true;
    this->receivingWorker = new std::thread(&ZenniumConnection::telegramListenerJob, this);
}

void ZenniumConnection::stopTelegramListener()
{
    shutdown(this->socket_handle, SHUT_RD);

    this->receiving_worker_is_running = false;
    this->receivingWorker->join();
}

std::chrono::milliseconds ZenniumConnection::getCurrentTimeInMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}

void ZenniumConnection::closeSocket()
{

#ifdef _WIN32
    closesocket(this->socket_handle);
#else
    close(this->socket_handle);
#endif

    this->socket_handle = INVALID_SOCKET;
}
