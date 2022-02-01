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
#ifndef THALESFILEINTERFACE_H
#define THALESFILEINTERFACE_H

#include <string>
#include <vector>
#include "thalesremoteconnection.h"

/** The ThalesFileInterface class
 *
 *  Class which realizes the file transfer between Term software and C++.
 *
 *  The measurement files cannot be stored on network drives with the Term software, therefore
 *  this class was implemented. If Python runs on another computer and controls Thales via network,
 *  then with this class the measurement result files can be exchanged via network.
 *
 *  This class establishes an additional socket connection to the Term, which can be used to transfer
 *  individual files manually. Or you can set that all ism, isc or isw files are transferred automatically
 *  at the end of the measurement.
 */
class ThalesFileInterface
{
public:
    class FileObject
    {
    public:
        std::string name;                   /**< Filename without path. */
        std::string path;                   /**< Filename with path on the Thales computer. */
        std::vector<uint8_t> binary_data;   /**< Data as bytearray. */
    };

    /** Construct a new Thales File Interface object
     *
     * \param address The hostname or ip-address of the host running Term.
     * \param connectionName The name of the connection default FileExchange. But can also be freely assigned.
     */
    ThalesFileInterface(std::string address = "localhost", std::string connectionName = "FileExchange");

    /** Close the file interface.
    *
     *  The automatic file sending is disabled and the socket connection is closed.
     */
    void close();

    /** Turn on automatic file exchange.
     *
     * If automatic file exchange is enabled, the files configured with the fileExtensions variable are transferred automatically.
     * It must be set separately whether ide files should remain in the object and whether they should be saved to the hard disk.
     *
     * \param enable Enable automatic file exchange. Default = true.
     * \param fileExtensions File extensions that will be exchanged. You can see the default paramter. But you can also specify only one type of file or more.
     * \return The response string from the device.
     */
    std::string enableAutomaticFileExchange(bool enable = true, std::string fileExtensions = "*.ism*.isc*.isw");

    /** Turn off automatic file exchange.
     *
     * \return The response string from the device.
     */
    std::string disableAutomaticFileExchange();

    /** Transfer a single file.
     *
     *  This command transfers a single from Term to Python.
     *  This command can only be executed if automatic transfer is disabled.
     *  The parameter filename is used to specify the full path of the file, on the computer running
     *  the Thales software, to be transferred e.g. r"C:\\THALES\\temp\\test1\\myeis.ism".
     *
     *  If the file does not exist everythin in the return value is empty.
     *
     * \param filename
     * \return
     */
    FileObject aquireFile(std::string filename);

    /** Set filenames to be filtered and not processed by C++.
     *
     *  Files with these names are not saved to disk by C++ and do not remain in the object.
     *
     * @param filename Filename to be filtered.
     */
    void appendFilesToSkip(std::string filename);

    /** Set the path where the files should be saved on the local computer.
     *
     *  This command sets only the path.
     *  The path must be accessible by C++ and exist, otherwise there are no restrictions on the path.
     *
     * @param path
     */
    void setSavePath(std::string path);

    /** Enable the automatic saving of files to the hard disk.
     *
     * This command configures that the files are automatically saved by C++.
     * With this command the path must be passed with the parameter path.
     * The path must be accessible by C++ and exist, otherwise there are no restrictions on the path.
     *
     * @param path Optional path where the files should be saved. For example r"D:\\myLocalDirectory".
     * @param enable Enable automatic file saving to the hard disk. Default = true.
     */
    void enableSaveReceivedFilesToDisk(std::string path, bool enable = true);

    /** Disable the automatic saving of files to the hard disk.
     *
     */
    void disableSaveReceivedFilesToDisk();

    /** Enable that the files remain in the C++ object.
     *
     * If you perform many measurements, the C++ object would grow larger and larger due to the
     * number of files, so you can disable that the files are stored in the object as an array.
     * By default, the files remain in the object.
     *
     * @param enable true = Allow the files to remain in the object.
     */
    void enableKeepReceivedFilesInObject(bool enable = true);

    /** Disable that the files remain in the C++ object.
     *
     */
    void disableKeepReceivedFilesInObject();

    /** Read all received files from the object.
     *
     * @return vector with the file objects.
     */
    const std::vector<FileObject>& getReceivedFiles();

    /** Read the latest received files from the object.
     *
     * @return file object.
     */
    const FileObject& getLatestReceivedFile();

    /** Writing a file object to the hard disk.
     *
     *  Writes the file object to the hard disk at the previously defined path.
     *
     * @param file The file object.
     */
    void saveReceivedFile(FileObject file);

    /** Delete all receifed files from the object.
     *
     */
    void deleteReceivedFiles();

private:
    /** Receive a file via the interface.
     *
     */
    FileObject receiveFile(const std::chrono::duration<int, std::milli> timeout = std::chrono::duration<int, std::milli>::max());

    /** Start the receive thread.
     *
     */
    void startWorker();

    /** Stop receiving thread.
     *
     */
    void stoppWorker();

    /** Function which is executed as receive thread.
     *
     */
    void fileReceiverJob();


    std::string connectionName;
    ZenniumConnection * remoteConnection;

    bool receiving_worker_is_running;
    std::thread *receivingWorker;

    bool automaticFileExchange;
    std::vector<std::string>filesToSkip;
    std::vector<FileObject>receivedFiles;
    std::string pathToSave;
    bool saveReceivedFilesToDisk;
    bool keepReceivedFilesInObject;
};

#endif // THALESFILEINTERFACE_H
