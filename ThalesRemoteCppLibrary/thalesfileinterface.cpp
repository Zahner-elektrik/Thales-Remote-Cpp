/******************************************************************
 *  ____       __                        __    __   __      _ __
 * /_  / ___ _/ /  ___  ___ ___________ / /__ / /__/ /_____(_) /__
 *  / /_/ _ `/ _ \/ _ \/ -_) __/___/ -_) / -_)  '_/ __/ __/ /  '_/
 * /___/\_,_/_//_/_//_/\__/_/      \__/_/\__/_/\_\\__/_/ /_/_/\_\
 *
 * Copyright 2022 ZAHNER-elektrik I. Zahner-Schiller GmbH & Co. KG
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
#include "thalesfileinterface.h"
#include <iostream>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <regex>

ThalesFileInterface::ThalesFileInterface(std::string address, std::string connectionName)
{
    this->connectionName = connectionName;
    this->remoteConnection = new ZenniumConnection();
    this->remoteConnection->connectToTerm(address, connectionName);
    this->filesToSkip.push_back("lastshot.ism");
    this->saveReceivedFilesToDisk = false;
    this->keepReceivedFilesInObject = true;
    this->receiving_worker_is_running = false;
}

void ThalesFileInterface::close()
{
    if(this->receiving_worker_is_running == true)
    {
        this->disableAutomaticFileExchange();
    }
    this->remoteConnection->disconnectFromTerm();
}

std::string ThalesFileInterface::enableAutomaticFileExchange(bool enable, std::string fileExtensions)
{
    std::string retval;
    if(enable == true)
    {
        retval = this->remoteConnection->sendStringAndWaitForReplyString(
                    "3," + this->connectionName + ",4,ON," + fileExtensions,
                    128,
                    std::chrono::duration<int, std::milli>::max(),
                    132
                    );
        this->startWorker();
    }
    else
    {
        retval = this->remoteConnection->sendStringAndWaitForReplyString(
                    "3," + this->connectionName + ",4,OFF",
                    128,
                    std::chrono::duration<int, std::milli>::max(),
                    132
                    );
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        this->stoppWorker();

    }
    return retval;
}

std::string ThalesFileInterface::disableAutomaticFileExchange()
{
    return this->enableAutomaticFileExchange(false);
}

ThalesFileInterface::FileObject ThalesFileInterface::aquireFile(std::string filename)
{
    FileObject retval;
    retval.name = "";
    if(receiving_worker_is_running == false)
    {
        this->remoteConnection->sendTelegram(
                    "3," + this->connectionName + ",1," + filename,
                    128);
        retval = this->receiveFile(std::chrono::duration<int, std::milli>::max());
    }
    return retval;
}

void ThalesFileInterface::appendFilesToSkip(std::string filename)
{
    this->filesToSkip.push_back(filename);
}

void ThalesFileInterface::setSavePath(std::string path)
{
    this->pathToSave = path;
}

void ThalesFileInterface::enableSaveReceivedFilesToDisk(std::string path, bool enable)
{
    this->saveReceivedFilesToDisk = enable;
    this->setSavePath(path);
}

void ThalesFileInterface::disableSaveReceivedFilesToDisk()
{
    this->saveReceivedFilesToDisk = false;
}

void ThalesFileInterface::enableKeepReceivedFilesInObject(bool enable)
{
    keepReceivedFilesInObject = enable;
}

void ThalesFileInterface::disableKeepReceivedFilesInObject()
{
    this->enableKeepReceivedFilesInObject(false);
}

const std::vector<ThalesFileInterface::FileObject>& ThalesFileInterface::getReceivedFiles()
{
    return receivedFiles;
}

const ThalesFileInterface::FileObject &ThalesFileInterface::getLatestReceivedFile()
{
    return receivedFiles.back();
}

void ThalesFileInterface::saveReceivedFile(FileObject file)
{
    if(this->saveReceivedFilesToDisk == true)
    {
        std::filesystem::path dir(this->pathToSave);
        std::filesystem::path fileName(file.name);
        std::filesystem::path fileNameWithPath = dir / fileName;

        std::ofstream fileStream(fileNameWithPath,std::ofstream::binary);
        for(auto data : file.binary_data)
        {
            fileStream << data;
        }
        fileStream.close();
    }
}

void ThalesFileInterface::deleteReceivedFiles()
{
    this->receivedFiles.clear();
}

ThalesFileInterface::FileObject ThalesFileInterface::receiveFile(const std::chrono::duration<int, std::milli> timeout)
{
    FileObject retval;
    retval.name = "";
    std::string filePath;

    try {
        filePath = this->remoteConnection->waitForStringTelegram(130,timeout);
    }  catch (...) {
        return retval;
    }

    std::string fileLength = this->remoteConnection->waitForStringTelegram(129);
    std::stringstream converterStream(fileLength);
    int fileLengthBytes;
    converterStream >> fileLengthBytes;

    int bytesToReceive = fileLengthBytes;

    std::vector<uint8_t> fileData;
    while(bytesToReceive > 0)
    {
        auto readBytes = this->remoteConnection->waitForTelegram(131);
        fileData.insert(fileData.end(),readBytes.begin(),readBytes.end());
        bytesToReceive -= readBytes.size();
    }

    retval.binary_data = fileData;
    retval.path = filePath;
    retval.name = std::filesystem::path(filePath).filename().string();
    return retval;
}

void ThalesFileInterface::startWorker()
{
    if(this->receiving_worker_is_running == false)
    {
        this->receiving_worker_is_running = true;
        this->receivingWorker = new std::thread(&ThalesFileInterface::fileReceiverJob, this);
    }
}

void ThalesFileInterface::stoppWorker()
{
    if(this->receiving_worker_is_running == true)
    {
        this->receiving_worker_is_running = false;
        this->receivingWorker->join();
        delete this->receivingWorker;
    }
}

void ThalesFileInterface::fileReceiverJob()
{
    while (this->receiving_worker_is_running == true)
    {
        try {
            auto file = this->receiveFile(std::chrono::milliseconds(1000));
            if(file.name != "")
            {
                if(std::find(filesToSkip.begin(),filesToSkip.end(),file.name) == filesToSkip.end())
                {
                    if(saveReceivedFilesToDisk == true)
                    {
                        this->saveReceivedFile(file);
                    }
                    if(keepReceivedFilesInObject == true)
                    {
                        this->receivedFiles.push_back(file);
                    }
                }
            }
        }  catch (...) {
            this->receiving_worker_is_running = false;
        }
    }
}
