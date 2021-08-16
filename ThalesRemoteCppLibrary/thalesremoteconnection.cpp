/******************************************************************
 *  ____       __                        __    __   __      _ __
 * /_  / ___ _/ /  ___  ___ ___________ / /__ / /__/ /_____(_) /__
 *  / /_/ _ `/ _ \/ _ \/ -_) __/___/ -_) / -_)  '_/ __/ __/ /  '_/
 * /___/\_,_/_//_/_//_/\__/_/      \__/_/\__/_/\_\\__/_/ /_/_/\_\
 *
 * Copyright 2019 ZAHNER-elektrik I. Zahner-Schiller GmbH & Co. KG
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

    socket_handle(INVALID_SOCKET),
    receiving_worker_is_running(false),
    receivingWorker(nullptr)
{

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

    this->socket_handle = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32
    if (this->socket_handle == INVALID_SOCKET) {
#else
    if (this->socket_handle < 0) {
#endif

        std::cerr << "could not create socket" << std::endl;
        return false;
    }

    struct addrinfo hints = {};
    struct addrinfo *result_pointer;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(address.data(), "260", &hints, &result_pointer) != 0)
    {

        std::cerr << "error while resolving address" << std::endl;
        this->closeSocket();
        return false;
    }

    struct addrinfo *first_addr = result_pointer;

    if (first_addr == nullptr)
    {

        std::cerr << "could not resolve hostname" << std::endl;
        freeaddrinfo(result_pointer);
        this->closeSocket();
        return false;
    }

    if (connect(this->socket_handle, first_addr->ai_addr, static_cast<int>(first_addr->ai_addrlen)) < 0)
    {

        std::cerr << "could not connect to term" << std::endl;
        this->closeSocket();
        return false;
    }

    this->startTelegramListener();

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    unsigned short payload_length = static_cast<unsigned short>(connectionName.length());

    std::vector<unsigned char> registration_packet;

    registration_packet.reserve(payload_length + 6);

    registration_packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[0]);
    registration_packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[1]);

    const std::vector<unsigned char> fixedHeaderBytes =
    {
        0x02, 0xd0,
        0xff, 0xff,
        0xff, 0xff
    };

    registration_packet.insert(registration_packet.end(), fixedHeaderBytes.begin(), fixedHeaderBytes.end());

    std::copy(connectionName.begin(), connectionName.end(), std::back_inserter(registration_packet));
    send(this->socket_handle, reinterpret_cast<char *>(registration_packet.data()), static_cast<int>(registration_packet.size()), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    return true;
}

void ZenniumConnection::disconnectFromTerm()
{

    try
    {
        this->sendStringAndWaitForReplyString("3,ScriptRemote,0,RS",0x80);
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

void ZenniumConnection::sendTelegram(std::string payload, char message_type)
{

    std::vector<char> packet;
    uint16_t payload_length = static_cast<uint16_t>(payload.size());

    packet.reserve(payload_length + 3);
    packet.push_back(reinterpret_cast<char *>(&payload_length)[0]);
    packet.push_back(reinterpret_cast<char *>(&payload_length)[1]);
    packet.push_back(message_type);

    std::copy(payload.begin(), payload.end(), std::back_inserter(packet));

    int status = send(this->socket_handle, reinterpret_cast<char *>(packet.data()), static_cast<int>(packet.size()), 0);

    if(status == -1)
    {
        throw TermConnectionError("Socket error during data transmission.");
    }
}

void ZenniumConnection::sendTelegram(std::vector<unsigned char> payload, unsigned char message_type)
{

    std::vector<unsigned char> packet;
    uint16_t payload_length = static_cast<uint16_t>(payload.size());

    packet.reserve(payload_length + 3);
    packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[0]);
    packet.push_back(reinterpret_cast<unsigned char *>(&payload_length)[1]);
    packet.push_back(message_type);

    packet.insert(packet.end(), payload.begin(), payload.end());

    int status = send(this->socket_handle, reinterpret_cast<char *>(packet.data()), static_cast<int>(packet.size()), 0);

    if(status == -1)
    {
        throw TermConnectionError("Socket error during data transmission.");
    }
}


std::string ZenniumConnection::waitForStringTelegram()
{

    while (this->telegramReceived() == false)
    {
        this->telegramsAvailableMutex.lock();
    }

    return this->receiveStringTelegram();
}

std::vector<uint8_t> ZenniumConnection::waitForTelegram()
{

    while (this->telegramReceived() == false)
    {
        this->telegramsAvailableMutex.lock();
    }

    return this->receiveTelegram();
}

std::vector<uint8_t> ZenniumConnection::waitForTelegram(const std::chrono::duration<int, std::milli> timeout) {

    std::chrono::milliseconds startTime = this->getCurrentTimeInMilliseconds();
    std::chrono::duration<int, std::milli> remainingTime;
    std::chrono::duration<int, std::milli> elapsedTime;

    while (this->telegramReceived() == false)
    {
        elapsedTime = this->getCurrentTimeInMilliseconds() - startTime;
        if (elapsedTime > timeout)
        {
            return std::vector<uint8_t>();
        }
        remainingTime = timeout - elapsedTime;

        this->telegramsAvailableMutex.try_lock_for(remainingTime);
    }

    return this->receiveTelegram();
}

std::string ZenniumConnection::waitForStringTelegram(const std::chrono::duration<int, std::milli> timeout)
{

    std::vector<uint8_t> telegram = waitForTelegram(timeout);

    return std::string(reinterpret_cast<char *>(telegram.data()), telegram.size());
}

std::string ZenniumConnection::receiveStringTelegram()
{

    std::vector<uint8_t> telegram = this->receiveTelegram();

    return std::string(reinterpret_cast<char *>(telegram.data()), telegram.size());
}

std::vector<uint8_t> ZenniumConnection::receiveTelegram()
{

    std::vector<uint8_t> receivedTelegram;

    // Making sure we won't read from the queue while the thread might be
    // in the process of putting in a new telegram.
    this->receivedTelegramsGuard.lock();

    if (this->receivedTelegrams.empty() == false)
    {
        receivedTelegram = this->receivedTelegrams.front();
        this->receivedTelegrams.pop();
    }

    this->receivedTelegramsGuard.unlock();

    if(receivedTelegram.size() == 0)
    {
        throw TermConnectionError("Socket error during data reception.");
    }

    return receivedTelegram;
}

std::string ZenniumConnection::sendStringAndWaitForReplyString(std::string payload, char message_type)
{

    // This is just a convenience method.
    this->sendTelegram(payload, message_type);
    return this->waitForStringTelegram();
}

bool ZenniumConnection::telegramReceived()
{

    bool telegramsAvailable = false;

    this->receivedTelegramsGuard.lock();

    telegramsAvailable = !this->receivedTelegrams.empty();

    this->receivedTelegramsGuard.unlock();

    return telegramsAvailable;
}

void ZenniumConnection::clearIncomingTelegramQueue()
{

    this->receivedTelegramsGuard.lock();

    while(this->receivedTelegrams.empty() == false)
    {
        this->receivedTelegrams.pop();
    }

    this->receivedTelegramsGuard.unlock();
}

std::vector<uint8_t> ZenniumConnection::readTelegramFromSocket()
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

            return std::vector<uint8_t>();
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
                return std::vector<uint8_t>();
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
    return incoming_packet;
}

void ZenniumConnection::telegramListenerJob()
{
    do {
        std::vector<uint8_t> telegram = readTelegramFromSocket();

        /*
         * To free the waiting receive threads, the Empty Telegram is put into the queue.
         * The receive thread is then terminated.
         */

        this->receivedTelegramsGuard.lock();
        this->receivedTelegrams.push(telegram);
        this->telegramsAvailableMutex.unlock();
        this->receivedTelegramsGuard.unlock();

        if (telegram.size() == 0)
        {

            this->receiving_worker_is_running = false;
        }

    } while (this->receiving_worker_is_running);
}

void ZenniumConnection::startTelegramListener()
{

    this->receiving_worker_is_running = true;
    this->telegramsAvailableMutex.lock();
    this->receivingWorker = new std::thread(&ZenniumConnection::telegramListenerJob, this);
}

void ZenniumConnection::stopTelegramListener()
{

    shutdown(this->socket_handle, SHUT_RD);

    this->receiving_worker_is_running = false;
    this->receivingWorker->join();

    this->telegramsAvailableMutex.unlock();
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
