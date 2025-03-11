#include <windows.h>
#include <filesystem>
#include <iostream>
#include <set>
#include <thread>

/*
 * Define the function prototypes
 */
typedef bool(__stdcall* getErrorMessageType)(void* handle, char* retval, int* retvalLen);
typedef void*(__stdcall* createZenniumConnectionType)();
typedef void(__stdcall* deleteZenniumConnectionType)(void* handle);
typedef bool(__stdcall* connectToTermType)(void* handle, char const* address, char const* connectionName);
typedef bool(__stdcall* waitForBinaryTelegramType)(void* handle, int message_type, char* retval, int* retvalLen);
typedef bool(__stdcall* waitForBinaryTelegramTimeoutType)(
    void* handle, int message_type, int timeout, char* retval, int* retvalLen
);
typedef bool(__stdcall* setTimeoutType)(void* handle, int timeout);
typedef bool(__stdcall* getTimeoutType)(void* handle, int* timeout);
typedef bool(__stdcall* disconnectFromTermType)(void* handle);

typedef void*(__stdcall* createThalesRemoteScriptWrapperType)(void* handle);
typedef void(__stdcall* deleteThalesRemoteScriptWrapperType)(void* handle);

typedef void*(__stdcall* createThalesFileInterfaceType)(void* handle);
typedef void(__stdcall* deleteThalesFileInterfaceType)(void* handle);
typedef bool(__stdcall* enableAutomaticFileExchangeType)(
    void* handle, char* retval, int* retvalLen, bool enable, char const* fileExtensions
);
typedef bool(__stdcall* disableAutomaticFileExchangeType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* appendFilesToSkipType)(void* handle, char const* filename);
typedef bool(__stdcall* setSavePathType)(void* handle, char const* path);
typedef bool(__stdcall* enableSaveReceivedFilesToDiskType)(void* handle, char const* path, bool enable);
typedef bool(__stdcall* disableSaveReceivedFilesToDiskType)(void* handle);

typedef bool(__stdcall* forceThalesIntoRemoteScriptType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* hideWindowType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* showWindowType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* getThalesVersionType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* getWorkstationHeartBeatType)(void* handle, int* retval);
typedef bool(__stdcall* getCurrentType)(void* handle, double* retval);
typedef bool(__stdcall* getPotentialType)(void* handle, double* retval);
typedef bool(__stdcall* getVoltageType)(void* handle, double* retval);
typedef bool(__stdcall* setCurrentType)(void* handle, char* retval, int* retvalLen, double current);
typedef bool(__stdcall* setPotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setMaximumShuntIndexType)(void* handle, char* retval, int* retvalLen, int shunt);
typedef bool(__stdcall* setMinimumShuntIndexType)(void* handle, char* retval, int* retvalLen, int shunt);
typedef bool(__stdcall* setShuntIndexType)(void* handle, char* retval, int* retvalLen, int index);
typedef bool(__stdcall* setVoltageRangeIndexType)(void* handle, char* retval, int* retvalLen, int index);
typedef bool(__stdcall* selectPotentiostatType)(void* handle, char* retval, int* retvalLen, int device);
typedef bool(__stdcall* selectPotentiostatWithoutPotentiostatStateChangeType)(
    void* handle, char* retval, int* retvalLen, int device
);
typedef bool(__stdcall* switchToSCPIControlType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* switchToSCPIControlWithoutPotentiostatStateChangeType)(
    void* handle, char* retval, int* retvalLen
);
typedef bool(__stdcall* getSerialNumberType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* getDeviceNameType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readSetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* calibrateOffsetsType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* enablePotentiostatType)(void* handle, char* retval, int* retvalLen, bool enabled);
typedef bool(__stdcall* disablePotentiostatType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setPotentiostatModeType)(
    void* handle, char* retval, int* retvalLen, char const* potentiostatMode
);
typedef bool(__stdcall* enableRuleFileUsageType)(void* handle, char* retval, int* retvalLen, bool enabled);
typedef bool(__stdcall* disableRuleFileUsageType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setupPad4ChannelType)(
    void* handle, char* retval, int* retvalLen, int card, int channel, bool enabled
);
typedef bool(__stdcall* setupPad4ChannelWithVoltageRangeType)(
    void* handle, char* retval, int* retvalLen, int card, int channel, bool enabled, double voltageRange
);
typedef bool(__stdcall* setupPad4ChannelWithShuntResistorType)(
    void* handle, char* retval, int* retvalLen, int card, int channel, bool enabled, double shuntResistor
);
typedef bool(__stdcall* setupPad4ModeGlobalType)(void* handle, char* retval, int* retvalLen, char const* mode);
typedef bool(__stdcall* enablePad4GlobalType)(void* handle, char* retval, int* retvalLen, bool enabled);
typedef bool(__stdcall* disablePad4GlobalType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readPAD4SetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setFrequencyType)(void* handle, char* retval, int* retvalLen, double frequency);
typedef bool(__stdcall* setAmplitudeType)(void* handle, char* retval, int* retvalLen, double amplitude);
typedef bool(__stdcall* setNumberOfPeriodsType)(void* handle, char* retval, int* retvalLen, int numberOfPeriods);
typedef bool(__stdcall* setUpperFrequencyLimitType)(void* handle, char* retval, int* retvalLen, double frequency);
typedef bool(__stdcall* setLowerFrequencyLimitType)(void* handle, char* retval, int* retvalLen, double frequency);
typedef bool(__stdcall* setStartFrequencyType)(void* handle, char* retval, int* retvalLen, double frequency);
typedef bool(__stdcall* setUpperStepsPerDecadeType)(void* handle, char* retval, int* retvalLen, int steps);
typedef bool(__stdcall* setLowerStepsPerDecadeType)(void* handle, char* retval, int* retvalLen, int steps);
typedef bool(__stdcall* setUpperNumberOfPeriodsType)(void* handle, char* retval, int* retvalLen, int periods);
typedef bool(__stdcall* setLowerNumberOfPeriodsType)(void* handle, char* retval, int* retvalLen, int periods);
typedef bool(__stdcall* setScanStrategyType)(void* handle, char* retval, int* retvalLen, char const* strategy);
typedef bool(__stdcall* setScanDirectionType)(void* handle, char* retval, int* retvalLen, char const* direction);
typedef bool(__stdcall* getImpedanceType)(void* handle, double* real, double* imag);
typedef bool(__stdcall* getImpedancePad4Type)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setEISNamingType)(void* handle, char* retval, int* retvalLen, char const* naming);
typedef bool(__stdcall* setEISCounterType)(void* handle, char* retval, int* retvalLen, int number);
typedef bool(__stdcall* setEISOutputPathType)(void* handle, char* retval, int* retvalLen, const char* path);
typedef bool(__stdcall* setEISOutputFileNameType)(void* handle, char* retval, int* retvalLen, const char* name);
typedef bool(__stdcall* measureEISType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setCVStartPotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setCVUpperReversingPotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setCVLowerReversingPotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setCVEndPotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setCVStartHoldTimeType)(void* handle, char* retval, int* retvalLen, double time);
typedef bool(__stdcall* setCVEndHoldTimeType)(void* handle, char* retval, int* retvalLen, double time);
typedef bool(__stdcall* setCVScanRateType)(void* handle, char* retval, int* retvalLen, double scanRate);
typedef bool(__stdcall* setCVCyclesType)(void* handle, char* retval, int* retvalLen, double cycles);
typedef bool(__stdcall* setCVSamplesPerCycleType)(void* handle, char* retval, int* retvalLen, double samples);
typedef bool(__stdcall* setCVMaximumCurrentType)(void* handle, char* retval, int* retvalLen, double current);
typedef bool(__stdcall* setCVMinimumCurrentType)(void* handle, char* retval, int* retvalLen, double current);
typedef bool(__stdcall* setCVOhmicDropType)(void* handle, char* retval, int* retvalLen, double ohmicDrop);
typedef bool(__stdcall* enableCVAutoRestartAtCurrentOverflowType)(
    void* handle, char* retval, int* retvalLen, bool enabled
);
typedef bool(__stdcall* disableCVAutoRestartAtCurrentOverflowType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* enableCVAutoRestartAtCurrentUnderflowType)(
    void* handle, char* retval, int* retvalLen, bool enabled
);
typedef bool(__stdcall* disableCVAutoRestartAtCurrentUnderflowType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* enableCVAnalogFunctionGeneratorType)(void* handle, char* retval, int* retvalLen, bool enabled);
typedef bool(__stdcall* disableCVAnalogFunctionGeneratorType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setCVNamingType)(void* handle, char* retval, int* retvalLen, char const* naming);
typedef bool(__stdcall* setCVCounterType)(void* handle, char* retval, int* retvalLen, int number);
typedef bool(__stdcall* setCVOutputPathType)(void* handle, char* retval, int* retvalLen, const char* path);
typedef bool(__stdcall* setCVOutputFileNameType)(void* handle, char* retval, int* retvalLen, const char* name);
typedef bool(__stdcall* checkCVSetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readCVSetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* measureCVType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setIEFirstEdgePotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setIESecondEdgePotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setIEThirdEdgePotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setIEFourthEdgePotentialType)(void* handle, char* retval, int* retvalLen, double potential);
typedef bool(__stdcall* setIEFirstEdgePotentialRelationType)(
    void* handle, char* retval, int* retvalLen, char const* relation
);
typedef bool(__stdcall* setIESecondEdgePotentialRelationType)(
    void* handle, char* retval, int* retvalLen, char const* relation
);
typedef bool(__stdcall* setIEThirdEdgePotentialRelationType)(
    void* handle, char* retval, int* retvalLen, char const* relation
);
typedef bool(__stdcall* setIEFourthEdgePotentialRelationType)(
    void* handle, char* retval, int* retvalLen, char const* relation
);
typedef bool(__stdcall* setIEPotentialResolutionType)(void* handle, char* retval, int* retvalLen, double resolution);
typedef bool(__stdcall* setIEMinimumWaitingTimeType)(void* handle, char* retval, int* retvalLen, double time);
typedef bool(__stdcall* setIEMaximumWaitingTimeType)(void* handle, char* retval, int* retvalLen, double time);
typedef bool(__stdcall* setIERelativeToleranceType)(void* handle, char* retval, int* retvalLen, double tolerance);
typedef bool(__stdcall* setIEAbsoluteToleranceType)(void* handle, char* retval, int* retvalLen, double tolerance);
typedef bool(__stdcall* setIEOhmicDropType)(void* handle, char* retval, int* retvalLen, double ohmicDrop);
typedef bool(__stdcall* setIESweepModeType)(void* handle, char* retval, int* retvalLen, char const* sweepMode);
typedef bool(__stdcall* setIEScanRateType)(void* handle, char* retval, int* retvalLen, double scanRate);
typedef bool(__stdcall* setIEMaximumCurrentType)(void* handle, char* retval, int* retvalLen, double current);
typedef bool(__stdcall* setIEMinimumCurrentType)(void* handle, char* retval, int* retvalLen, double current);
typedef bool(__stdcall* setIENamingType)(void* handle, char* retval, int* retvalLen, char const* naming);
typedef bool(__stdcall* setIECounterType)(void* handle, char* retval, int* retvalLen, int number);
typedef bool(__stdcall* setIEOutputPathType)(void* handle, char* retval, int* retvalLen, const char* path);
typedef bool(__stdcall* setIEOutputFileNameType)(void* handle, char* retval, int* retvalLen, const char* name);
typedef bool(__stdcall* checkIESetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readIESetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* measureIEType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* selectSequenceType)(void* handle, char* retval, int* retvalLen, int number);
typedef bool(__stdcall* setSequenceNamingType)(void* handle, char* retval, int* retvalLen, char const* naming);
typedef bool(__stdcall* setSequenceCounterType)(void* handle, char* retval, int* retvalLen, int number);
typedef bool(__stdcall* setSequenceOutputPathType)(void* handle, char* retval, int* retvalLen, const char* path);
typedef bool(__stdcall* setSequenceOutputFileNameType)(void* handle, char* retval, int* retvalLen, const char* name);
typedef bool(__stdcall* enableSequenceAcqGlobalType)(void* handle, char* retval, int* retvalLen, bool state);
typedef bool(__stdcall* disableSequenceAcqGlobalType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* enableSequenceAcqChannelType)(
    void* handle, char* retval, int* retvalLen, int channel, bool state
);
typedef bool(__stdcall* disableSequenceAcqChannelType)(void* handle, char* retval, int* retvalLen, int channel);
typedef bool(__stdcall* readSequenceAcqSetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* runSequenceType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* setSequenceOhmicDropType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequenceMaximumRuntimeType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequenceUpperPotentialLimitType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequenceLowerPotentialLimitType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequenceUpperCurrentLimitType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequenceLowerCurrentLimitType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequenceCurrentRangeType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setSequencePotentialLatencyWindowType)(
    void* handle, char* retval, int* retvalLen, double value
);
typedef bool(__stdcall* setSequenceCurrentLatencyWindowType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* enableFraModeType)(void* handle, char* retval, int* retvalLen, bool enabled);
typedef bool(__stdcall* setFraVoltageInputGainType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraVoltageInputOffsetType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraVoltageOutputGainType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraVoltageOutputOffsetType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraVoltageMinimumType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraVoltageMaximumType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraCurrentInputGainType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraCurrentInputOffsetType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraCurrentOutputGainType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraCurrentOutputOffsetType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraCurrentMinimumType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraCurrentMaximumType)(void* handle, char* retval, int* retvalLen, double value);
typedef bool(__stdcall* setFraPotentiostatModeType)(
    void* handle, char* retval, int* retvalLen, char const* potentiostatMode
);
typedef bool(__stdcall* readFraSetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readAcqSetupType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readAllAcqChannelsType)(void* handle, char* retval, int* retvalLen);
typedef bool(__stdcall* readAcqChannelType)(void* handle, double* retval, int channel);
typedef bool(__stdcall* enableAcqType)(void* handle, char* retval, int* retvalLen, bool enabled);
typedef bool(__stdcall* disableAcqType)(void* handle, char* retval, int* retvalLen);

bool runThreads          = true;
constexpr auto ipAddress = "localhost";
HINSTANCE lib;

/** Function which is used as thread.
 *
 */
void watchThread() {
    createZenniumConnectionType createZenniumConnection =
        (createZenniumConnectionType)GetProcAddress(lib, "createZenniumConnection");
    deleteZenniumConnectionType deleteZenniumConnection =
        (deleteZenniumConnectionType)GetProcAddress(lib, "deleteZenniumConnection");
    connectToTermType connectToTerm           = (connectToTermType)GetProcAddress(lib, "connectToTerm");
    disconnectFromTermType disconnectFromTerm = (disconnectFromTermType)GetProcAddress(lib, "disconnectFromTerm");

    createThalesRemoteScriptWrapperType createThalesRemoteScriptWrapper =
        (createThalesRemoteScriptWrapperType)GetProcAddress(lib, "createThalesRemoteScriptWrapper");
    deleteThalesRemoteScriptWrapperType deleteThalesRemoteScriptWrapper =
        (deleteThalesRemoteScriptWrapperType)GetProcAddress(lib, "deleteThalesRemoteScriptWrapper");
    getWorkstationHeartBeatType getWorkstationHeartBeat =
        (getWorkstationHeartBeatType)GetProcAddress(lib, "getWorkstationHeartBeat");

    void* connectionHandle = createZenniumConnection();
    bool state             = connectToTerm(connectionHandle, ipAddress, "Watch");
    if (not state) {
        std::cout << "could not connect to Term: Watch Thread" << std::endl;
        return;
    } else {
        std::cout << "connected: Watch Thread" << std::endl;
    }
    void* scriptHandle = createThalesRemoteScriptWrapper(connectionHandle);

    while (runThreads == true) {
        int count;
        getWorkstationHeartBeat(scriptHandle, &count);
        std::cout << "Beat count: " << count << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    std::cout << "disconnect Watch" << std::endl;
    disconnectFromTerm(connectionHandle);

    deleteThalesRemoteScriptWrapper(scriptHandle);
    deleteZenniumConnection(connectionHandle);
    std::cout << "finish Watch" << std::endl;
}

void onlineDataThread() {
    createZenniumConnectionType createZenniumConnection =
        (createZenniumConnectionType)GetProcAddress(lib, "createZenniumConnection");
    deleteZenniumConnectionType deleteZenniumConnection =
        (deleteZenniumConnectionType)GetProcAddress(lib, "deleteZenniumConnection");
    connectToTermType connectToTerm           = (connectToTermType)GetProcAddress(lib, "connectToTerm");
    disconnectFromTermType disconnectFromTerm = (disconnectFromTermType)GetProcAddress(lib, "disconnectFromTerm");
    waitForBinaryTelegramTimeoutType waitForBinaryTelegramTimeout =
        (waitForBinaryTelegramTimeoutType)GetProcAddress(lib, "waitForBinaryTelegramTimeout");
    getErrorMessageType getErrorMessage = (getErrorMessageType)GetProcAddress(lib, "getErrorMessage");

    void* connectionHandle = createZenniumConnection();
    bool state             = connectToTerm(connectionHandle, ipAddress, "Logging");
    if (not state) {
        std::cout << "could not connect to Term: Online Thread" << std::endl;
        return;
    } else {
        std::cout << "connected: Online Thread" << std::endl;
    }
    constexpr int MAX_LEN = 1024;
    char data[MAX_LEN];
    int dataLen = MAX_LEN;
    while (runThreads == true) {
        bool state = waitForBinaryTelegramTimeout(connectionHandle, 2, 1000, data, &dataLen);
        if (not state) {
            // char errorMessage[MAX_LEN];
            // int errorLen = MAX_LEN;
            // getErrorMessage(connectionHandle,errorMessage,&errorLen);
            // std::cout << "ERROR or TIMEOUT:" << std::endl;
            // if(errorLen > 0){
            //     std::cout << errorMessage << std::endl;
            // }
        } else {
            if (dataLen > 0) {
                std::set<char> relevantPackets = {1, 2, 4, 5, 6};
                if (relevantPackets.contains(data[0])) {
                    // only print relevant packets index 0 contains the type of the packet.
                    std::cout << data << std::endl;
                }
            }
        }

        dataLen = MAX_LEN;
    }

    std::cout << "disconnect Logging" << std::endl;
    disconnectFromTerm(connectionHandle);
    deleteZenniumConnection(connectionHandle);
    std::cout << "finish Logging" << std::endl;
}

int main(int argc, char* argv[]) {
    const std::string libPath =
        "C:/Programmieren/gitlabs/ThalesRemoteCpp/thales-remote-cppp/build/MinGW32-Release/ThalesRemoteExternalLibrary/"
        "libThalesRemoteExternalLibrary.dll";
    // const std::string libPath =
    //     "C:/Programmieren/gitlabs/ThalesRemoteCpp/thales-remote-cppp/build/Desktop_Qt_6_8_1_MinGW_64_bit-Release/"
    //     "ThalesRemoteExternalLibrary/"
    //     "libThalesRemoteExternalLibrary.dll";

    if (std::filesystem::exists(libPath)) {
        std::cout << "lib exists" << std::endl;
    } else {
        std::cout << "lib not found" << std::endl;
    }

    std::cout << "try to LoadLibrary" << std::endl;
    lib = LoadLibrary(libPath.c_str());
    char replyBuffer[1000];
    int replySize;
    if (lib == NULL) {
        std::cout << "LoadLibrary error" << std::endl;
        DWORD error         = GetLastError();
        LPSTR messageBuffer = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&messageBuffer,
            0,
            nullptr
        );
        std::cout << messageBuffer << std::endl;
        LocalFree(messageBuffer);
    } else {
        std::cout << "LoadLibrary ok" << std::endl;

        /*
         * Loading all functions from the DLL
         */

        getErrorMessageType getErrorMessage = (getErrorMessageType)GetProcAddress(lib, "getErrorMessage");
        createZenniumConnectionType createZenniumConnection =
            (createZenniumConnectionType)GetProcAddress(lib, "createZenniumConnection");
        deleteZenniumConnectionType deleteZenniumConnection =
            (deleteZenniumConnectionType)GetProcAddress(lib, "deleteZenniumConnection");
        connectToTermType connectToTerm           = (connectToTermType)GetProcAddress(lib, "connectToTerm");
        setTimeoutType setTimeout                 = (setTimeoutType)GetProcAddress(lib, "setTimeout");
        getTimeoutType getTimeout                 = (getTimeoutType)GetProcAddress(lib, "getTimeout");
        disconnectFromTermType disconnectFromTerm = (disconnectFromTermType)GetProcAddress(lib, "disconnectFromTerm");

        createThalesRemoteScriptWrapperType createThalesRemoteScriptWrapper =
            (createThalesRemoteScriptWrapperType)GetProcAddress(lib, "createThalesRemoteScriptWrapper");
        deleteThalesRemoteScriptWrapperType deleteThalesRemoteScriptWrapper =
            (deleteThalesRemoteScriptWrapperType)GetProcAddress(lib, "deleteThalesRemoteScriptWrapper");

        createThalesFileInterfaceType createThalesFileInterface =
            (createThalesFileInterfaceType)GetProcAddress(lib, "createThalesFileInterface");
        deleteThalesFileInterfaceType deleteThalesFileInterface =
            (deleteThalesFileInterfaceType)GetProcAddress(lib, "deleteThalesFileInterface");
        enableAutomaticFileExchangeType enableAutomaticFileExchange =
            (enableAutomaticFileExchangeType)GetProcAddress(lib, "enableAutomaticFileExchange");
        disableAutomaticFileExchangeType disableAutomaticFileExchange =
            (disableAutomaticFileExchangeType)GetProcAddress(lib, "disableAutomaticFileExchange");
        appendFilesToSkipType appendFilesToSkip = (appendFilesToSkipType)GetProcAddress(lib, "appendFilesToSkip");
        setSavePathType setSavePath             = (setSavePathType)GetProcAddress(lib, "setSavePath");
        enableSaveReceivedFilesToDiskType enableSaveReceivedFilesToDisk =
            (enableSaveReceivedFilesToDiskType)GetProcAddress(lib, "enableSaveReceivedFilesToDisk");
        disableSaveReceivedFilesToDiskType disableSaveReceivedFilesToDisk =
            (disableSaveReceivedFilesToDiskType)GetProcAddress(lib, "disableSaveReceivedFilesToDisk");

        forceThalesIntoRemoteScriptType forceThalesIntoRemoteScript =
            (forceThalesIntoRemoteScriptType)GetProcAddress(lib, "forceThalesIntoRemoteScript");
        hideWindowType hideWindow             = (hideWindowType)GetProcAddress(lib, "hideWindow");
        showWindowType showWindow             = (showWindowType)GetProcAddress(lib, "showWindow");
        getThalesVersionType getThalesVersion = (getThalesVersionType)GetProcAddress(lib, "getThalesVersion");
        getWorkstationHeartBeatType getWorkstationHeartBeat =
            (getWorkstationHeartBeatType)GetProcAddress(lib, "getWorkstationHeartBeat");
        getCurrentType getCurrent     = (getCurrentType)GetProcAddress(lib, "getCurrent");
        getPotentialType getPotential = (getPotentialType)GetProcAddress(lib, "getPotential");
        getVoltageType getVoltage     = (getVoltageType)GetProcAddress(lib, "getVoltage");
        setCurrentType setCurrent     = (setCurrentType)GetProcAddress(lib, "setCurrent");
        setPotentialType setPotential = (setPotentialType)GetProcAddress(lib, "setPotential");
        setMaximumShuntIndexType setMaximumShuntIndex =
            (setMaximumShuntIndexType)GetProcAddress(lib, "setMaximumShuntIndex");
        setMinimumShuntIndexType setMinimumShuntIndex =
            (setMinimumShuntIndexType)GetProcAddress(lib, "setMinimumShuntIndex");
        setShuntIndexType setShuntIndex = (setShuntIndexType)GetProcAddress(lib, "setShuntIndex");
        setVoltageRangeIndexType setVoltageRangeIndex =
            (setVoltageRangeIndexType)GetProcAddress(lib, "setVoltageRangeIndex");
        selectPotentiostatType selectPotentiostat = (selectPotentiostatType)GetProcAddress(lib, "selectPotentiostat");
        selectPotentiostatWithoutPotentiostatStateChangeType selectPotentiostatWithoutPotentiostatStateChange =
            (selectPotentiostatWithoutPotentiostatStateChangeType
            )GetProcAddress(lib, "selectPotentiostatWithoutPotentiostatStateChange");
        switchToSCPIControlType switchToSCPIControl =
            (switchToSCPIControlType)GetProcAddress(lib, "switchToSCPIControl");
        switchToSCPIControlWithoutPotentiostatStateChangeType switchToSCPIControlWithoutPotentiostatStateChange =
            (switchToSCPIControlWithoutPotentiostatStateChangeType
            )GetProcAddress(lib, "switchToSCPIControlWithoutPotentiostatStateChange");
        getSerialNumberType getSerialNumber       = (getSerialNumberType)GetProcAddress(lib, "getSerialNumber");
        getDeviceNameType getDeviceName           = (getDeviceNameType)GetProcAddress(lib, "getDeviceName");
        readSetupType readSetup                   = (readSetupType)GetProcAddress(lib, "readSetup");
        calibrateOffsetsType calibrateOffsets     = (calibrateOffsetsType)GetProcAddress(lib, "calibrateOffsets");
        enablePotentiostatType enablePotentiostat = (enablePotentiostatType)GetProcAddress(lib, "enablePotentiostat");
        disablePotentiostatType disablePotentiostat =
            (disablePotentiostatType)GetProcAddress(lib, "disablePotentiostat");
        setPotentiostatModeType setPotentiostatMode =
            (setPotentiostatModeType)GetProcAddress(lib, "setPotentiostatMode");
        enableRuleFileUsageType enableRuleFileUsage =
            (enableRuleFileUsageType)GetProcAddress(lib, "enableRuleFileUsage");
        disableRuleFileUsageType disableRuleFileUsage =
            (disableRuleFileUsageType)GetProcAddress(lib, "disableRuleFileUsage");
        setupPad4ChannelType setupPad4Channel = (setupPad4ChannelType)GetProcAddress(lib, "setupPad4Channel");
        setupPad4ChannelWithVoltageRangeType setupPad4ChannelWithVoltageRange =
            (setupPad4ChannelWithVoltageRangeType)GetProcAddress(lib, "setupPad4ChannelWithVoltageRange");
        setupPad4ChannelWithShuntResistorType setupPad4ChannelWithShuntResistor =
            (setupPad4ChannelWithShuntResistorType)GetProcAddress(lib, "setupPad4ChannelWithShuntResistor");
        setupPad4ModeGlobalType setupPad4ModeGlobal =
            (setupPad4ModeGlobalType)GetProcAddress(lib, "setupPad4ModeGlobal");
        enablePad4GlobalType enablePad4Global     = (enablePad4GlobalType)GetProcAddress(lib, "enablePad4Global");
        disablePad4GlobalType disablePad4Global   = (disablePad4GlobalType)GetProcAddress(lib, "disablePad4Global");
        readPAD4SetupType readPAD4Setup           = (readPAD4SetupType)GetProcAddress(lib, "readPAD4Setup");
        setFrequencyType setFrequency             = (setFrequencyType)GetProcAddress(lib, "setFrequency");
        setAmplitudeType setAmplitude             = (setAmplitudeType)GetProcAddress(lib, "setAmplitude");
        setNumberOfPeriodsType setNumberOfPeriods = (setNumberOfPeriodsType)GetProcAddress(lib, "setNumberOfPeriods");
        setUpperFrequencyLimitType setUpperFrequencyLimit =
            (setUpperFrequencyLimitType)GetProcAddress(lib, "setUpperFrequencyLimit");
        setLowerFrequencyLimitType setLowerFrequencyLimit =
            (setLowerFrequencyLimitType)GetProcAddress(lib, "setLowerFrequencyLimit");
        setStartFrequencyType setStartFrequency = (setStartFrequencyType)GetProcAddress(lib, "setStartFrequency");
        setUpperStepsPerDecadeType setUpperStepsPerDecade =
            (setUpperStepsPerDecadeType)GetProcAddress(lib, "setUpperStepsPerDecade");
        setLowerStepsPerDecadeType setLowerStepsPerDecade =
            (setLowerStepsPerDecadeType)GetProcAddress(lib, "setLowerStepsPerDecade");
        setUpperNumberOfPeriodsType setUpperNumberOfPeriods =
            (setUpperNumberOfPeriodsType)GetProcAddress(lib, "setUpperNumberOfPeriods");
        setLowerNumberOfPeriodsType setLowerNumberOfPeriods =
            (setLowerNumberOfPeriodsType)GetProcAddress(lib, "setLowerNumberOfPeriods");
        setScanStrategyType setScanStrategy   = (setScanStrategyType)GetProcAddress(lib, "setScanStrategy");
        setScanDirectionType setScanDirection = (setScanDirectionType)GetProcAddress(lib, "setScanDirection");
        getImpedanceType getImpedance         = (getImpedanceType)GetProcAddress(lib, "getImpedance");
        getImpedancePad4Type getImpedancePad4 = (getImpedancePad4Type)GetProcAddress(lib, "getImpedancePad4");
        setEISNamingType setEISNaming         = (setEISNamingType)GetProcAddress(lib, "setEISNaming");
        setEISCounterType setEISCounter       = (setEISCounterType)GetProcAddress(lib, "setEISCounter");
        setEISOutputPathType setEISOutputPath = (setEISOutputPathType)GetProcAddress(lib, "setEISOutputPath");
        setEISOutputFileNameType setEISOutputFileName =
            (setEISOutputFileNameType)GetProcAddress(lib, "setEISOutputFileName");
        measureEISType measureEIS = (measureEISType)GetProcAddress(lib, "measureEIS");
        setCVStartPotentialType setCVStartPotential =
            (setCVStartPotentialType)GetProcAddress(lib, "setCVStartPotential");
        setCVUpperReversingPotentialType setCVUpperReversingPotential =
            (setCVUpperReversingPotentialType)GetProcAddress(lib, "setCVUpperReversingPotential");
        setCVLowerReversingPotentialType setCVLowerReversingPotential =
            (setCVLowerReversingPotentialType)GetProcAddress(lib, "setCVLowerReversingPotential");
        setCVEndPotentialType setCVEndPotential   = (setCVEndPotentialType)GetProcAddress(lib, "setCVEndPotential");
        setCVStartHoldTimeType setCVStartHoldTime = (setCVStartHoldTimeType)GetProcAddress(lib, "setCVStartHoldTime");
        setCVEndHoldTimeType setCVEndHoldTime     = (setCVEndHoldTimeType)GetProcAddress(lib, "setCVEndHoldTime");
        setCVScanRateType setCVScanRate           = (setCVScanRateType)GetProcAddress(lib, "setCVScanRate");
        setCVCyclesType setCVCycles               = (setCVCyclesType)GetProcAddress(lib, "setCVCycles");
        setCVSamplesPerCycleType setCVSamplesPerCycle =
            (setCVSamplesPerCycleType)GetProcAddress(lib, "setCVSamplesPerCycle");
        setCVMaximumCurrentType setCVMaximumCurrent =
            (setCVMaximumCurrentType)GetProcAddress(lib, "setCVMaximumCurrent");
        setCVMinimumCurrentType setCVMinimumCurrent =
            (setCVMinimumCurrentType)GetProcAddress(lib, "setCVMinimumCurrent");
        setCVOhmicDropType setCVOhmicDrop = (setCVOhmicDropType)GetProcAddress(lib, "setCVOhmicDrop");
        enableCVAutoRestartAtCurrentOverflowType enableCVAutoRestartAtCurrentOverflow =
            (enableCVAutoRestartAtCurrentOverflowType)GetProcAddress(lib, "enableCVAutoRestartAtCurrentOverflow");
        disableCVAutoRestartAtCurrentOverflowType disableCVAutoRestartAtCurrentOverflow =
            (disableCVAutoRestartAtCurrentOverflowType)GetProcAddress(lib, "disableCVAutoRestartAtCurrentOverflow");
        enableCVAutoRestartAtCurrentUnderflowType enableCVAutoRestartAtCurrentUnderflow =
            (enableCVAutoRestartAtCurrentUnderflowType)GetProcAddress(lib, "enableCVAutoRestartAtCurrentUnderflow");
        disableCVAutoRestartAtCurrentUnderflowType disableCVAutoRestartAtCurrentUnderflow =
            (disableCVAutoRestartAtCurrentUnderflowType)GetProcAddress(lib, "disableCVAutoRestartAtCurrentUnderflow");
        enableCVAnalogFunctionGeneratorType enableCVAnalogFunctionGenerator =
            (enableCVAnalogFunctionGeneratorType)GetProcAddress(lib, "enableCVAnalogFunctionGenerator");
        disableCVAnalogFunctionGeneratorType disableCVAnalogFunctionGenerator =
            (disableCVAnalogFunctionGeneratorType)GetProcAddress(lib, "disableCVAnalogFunctionGenerator");
        setCVNamingType setCVNaming         = (setCVNamingType)GetProcAddress(lib, "setCVNaming");
        setCVCounterType setCVCounter       = (setCVCounterType)GetProcAddress(lib, "setCVCounter");
        setCVOutputPathType setCVOutputPath = (setCVOutputPathType)GetProcAddress(lib, "setCVOutputPath");
        setCVOutputFileNameType setCVOutputFileName =
            (setCVOutputFileNameType)GetProcAddress(lib, "setCVOutputFileName");
        checkCVSetupType checkCVSetup = (checkCVSetupType)GetProcAddress(lib, "checkCVSetup");
        readCVSetupType readCVSetup   = (readCVSetupType)GetProcAddress(lib, "readCVSetup");
        measureCVType measureCV       = (measureCVType)GetProcAddress(lib, "measureCV");
        setIEFirstEdgePotentialType setIEFirstEdgePotential =
            (setIEFirstEdgePotentialType)GetProcAddress(lib, "setIEFirstEdgePotential");
        setIESecondEdgePotentialType setIESecondEdgePotential =
            (setIESecondEdgePotentialType)GetProcAddress(lib, "setIESecondEdgePotential");
        setIEThirdEdgePotentialType setIEThirdEdgePotential =
            (setIEThirdEdgePotentialType)GetProcAddress(lib, "setIEThirdEdgePotential");
        setIEFourthEdgePotentialType setIEFourthEdgePotential =
            (setIEFourthEdgePotentialType)GetProcAddress(lib, "setIEFourthEdgePotential");
        setIEFirstEdgePotentialRelationType setIEFirstEdgePotentialRelation =
            (setIEFirstEdgePotentialRelationType)GetProcAddress(lib, "setIEFirstEdgePotentialRelation");
        setIESecondEdgePotentialRelationType setIESecondEdgePotentialRelation =
            (setIESecondEdgePotentialRelationType)GetProcAddress(lib, "setIESecondEdgePotentialRelation");
        setIEThirdEdgePotentialRelationType setIEThirdEdgePotentialRelation =
            (setIEThirdEdgePotentialRelationType)GetProcAddress(lib, "setIEThirdEdgePotentialRelation");
        setIEFourthEdgePotentialRelationType setIEFourthEdgePotentialRelation =
            (setIEFourthEdgePotentialRelationType)GetProcAddress(lib, "setIEFourthEdgePotentialRelation");
        setIEPotentialResolutionType setIEPotentialResolution =
            (setIEPotentialResolutionType)GetProcAddress(lib, "setIEPotentialResolution");
        setIEMinimumWaitingTimeType setIEMinimumWaitingTime =
            (setIEMinimumWaitingTimeType)GetProcAddress(lib, "setIEMinimumWaitingTime");
        setIEMaximumWaitingTimeType setIEMaximumWaitingTime =
            (setIEMaximumWaitingTimeType)GetProcAddress(lib, "setIEMaximumWaitingTime");
        setIERelativeToleranceType setIERelativeTolerance =
            (setIERelativeToleranceType)GetProcAddress(lib, "setIERelativeTolerance");
        setIEAbsoluteToleranceType setIEAbsoluteTolerance =
            (setIEAbsoluteToleranceType)GetProcAddress(lib, "setIEAbsoluteTolerance");
        setIEOhmicDropType setIEOhmicDrop = (setIEOhmicDropType)GetProcAddress(lib, "setIEOhmicDrop");
        setIESweepModeType setIESweepMode = (setIESweepModeType)GetProcAddress(lib, "setIESweepMode");
        setIEScanRateType setIEScanRate   = (setIEScanRateType)GetProcAddress(lib, "setIEScanRate");
        setIEMaximumCurrentType setIEMaximumCurrent =
            (setIEMaximumCurrentType)GetProcAddress(lib, "setIEMaximumCurrent");
        setIEMinimumCurrentType setIEMinimumCurrent =
            (setIEMinimumCurrentType)GetProcAddress(lib, "setIEMinimumCurrent");
        setIENamingType setIENaming         = (setIENamingType)GetProcAddress(lib, "setIENaming");
        setIECounterType setIECounter       = (setIECounterType)GetProcAddress(lib, "setIECounter");
        setIEOutputPathType setIEOutputPath = (setIEOutputPathType)GetProcAddress(lib, "setIEOutputPath");
        setIEOutputFileNameType setIEOutputFileName =
            (setIEOutputFileNameType)GetProcAddress(lib, "setIEOutputFileName");
        checkIESetupType checkIESetup             = (checkIESetupType)GetProcAddress(lib, "checkIESetup");
        readIESetupType readIESetup               = (readIESetupType)GetProcAddress(lib, "readIESetup");
        measureIEType measureIE                   = (measureIEType)GetProcAddress(lib, "measureIE");
        selectSequenceType selectSequence         = (selectSequenceType)GetProcAddress(lib, "selectSequence");
        setSequenceNamingType setSequenceNaming   = (setSequenceNamingType)GetProcAddress(lib, "setSequenceNaming");
        setSequenceCounterType setSequenceCounter = (setSequenceCounterType)GetProcAddress(lib, "setSequenceCounter");
        setSequenceOutputPathType setSequenceOutputPath =
            (setSequenceOutputPathType)GetProcAddress(lib, "setSequenceOutputPath");
        setSequenceOutputFileNameType setSequenceOutputFileName =
            (setSequenceOutputFileNameType)GetProcAddress(lib, "setSequenceOutputFileName");
        enableSequenceAcqGlobalType enableSequenceAcqGlobal =
            (enableSequenceAcqGlobalType)GetProcAddress(lib, "enableSequenceAcqGlobal");
        disableSequenceAcqGlobalType disableSequenceAcqGlobal =
            (disableSequenceAcqGlobalType)GetProcAddress(lib, "disableSequenceAcqGlobal");
        enableSequenceAcqChannelType enableSequenceAcqChannel =
            (enableSequenceAcqChannelType)GetProcAddress(lib, "enableSequenceAcqChannel");
        disableSequenceAcqChannelType disableSequenceAcqChannel =
            (disableSequenceAcqChannelType)GetProcAddress(lib, "disableSequenceAcqChannel");
        readSequenceAcqSetupType readSequenceAcqSetup =
            (readSequenceAcqSetupType)GetProcAddress(lib, "readSequenceAcqSetup");
        runSequenceType runSequence = (runSequenceType)GetProcAddress(lib, "runSequence");
        setSequenceOhmicDropType setSequenceOhmicDrop =
            (setSequenceOhmicDropType)GetProcAddress(lib, "setSequenceOhmicDrop");
        setSequenceMaximumRuntimeType setSequenceMaximumRuntime =
            (setSequenceMaximumRuntimeType)GetProcAddress(lib, "setSequenceMaximumRuntime");
        setSequenceUpperPotentialLimitType setSequenceUpperPotentialLimit =
            (setSequenceUpperPotentialLimitType)GetProcAddress(lib, "setSequenceUpperPotentialLimit");
        setSequenceLowerPotentialLimitType setSequenceLowerPotentialLimit =
            (setSequenceLowerPotentialLimitType)GetProcAddress(lib, "setSequenceLowerPotentialLimit");
        setSequenceUpperCurrentLimitType setSequenceUpperCurrentLimit =
            (setSequenceUpperCurrentLimitType)GetProcAddress(lib, "setSequenceUpperCurrentLimit");
        setSequenceLowerCurrentLimitType setSequenceLowerCurrentLimit =
            (setSequenceLowerCurrentLimitType)GetProcAddress(lib, "setSequenceLowerCurrentLimit");
        setSequenceCurrentRangeType setSequenceCurrentRange =
            (setSequenceCurrentRangeType)GetProcAddress(lib, "setSequenceCurrentRange");
        setSequencePotentialLatencyWindowType setSequencePotentialLatencyWindow =
            (setSequencePotentialLatencyWindowType)GetProcAddress(lib, "setSequencePotentialLatencyWindow");
        setSequenceCurrentLatencyWindowType setSequenceCurrentLatencyWindow =
            (setSequenceCurrentLatencyWindowType)GetProcAddress(lib, "setSequenceCurrentLatencyWindow");
        enableFraModeType enableFraMode = (enableFraModeType)GetProcAddress(lib, "enableFraMode");
        setFraVoltageInputGainType setFraVoltageInputGain =
            (setFraVoltageInputGainType)GetProcAddress(lib, "setFraVoltageInputGain");
        setFraVoltageInputOffsetType setFraVoltageInputOffset =
            (setFraVoltageInputOffsetType)GetProcAddress(lib, "setFraVoltageInputOffset");
        setFraVoltageOutputGainType setFraVoltageOutputGain =
            (setFraVoltageOutputGainType)GetProcAddress(lib, "setFraVoltageOutputGain");
        setFraVoltageOutputOffsetType setFraVoltageOutputOffset =
            (setFraVoltageOutputOffsetType)GetProcAddress(lib, "setFraVoltageOutputOffset");
        setFraVoltageMinimumType setFraVoltageMinimum =
            (setFraVoltageMinimumType)GetProcAddress(lib, "setFraVoltageMinimum");
        setFraVoltageMaximumType setFraVoltageMaximum =
            (setFraVoltageMaximumType)GetProcAddress(lib, "setFraVoltageMaximum");
        setFraCurrentInputGainType setFraCurrentInputGain =
            (setFraCurrentInputGainType)GetProcAddress(lib, "setFraCurrentInputGain");
        setFraCurrentInputOffsetType setFraCurrentInputOffset =
            (setFraCurrentInputOffsetType)GetProcAddress(lib, "setFraCurrentInputOffset");
        setFraCurrentOutputGainType setFraCurrentOutputGain =
            (setFraCurrentOutputGainType)GetProcAddress(lib, "setFraCurrentOutputGain");
        setFraCurrentOutputOffsetType setFraCurrentOutputOffset =
            (setFraCurrentOutputOffsetType)GetProcAddress(lib, "setFraCurrentOutputOffset");
        setFraCurrentMinimumType setFraCurrentMinimum =
            (setFraCurrentMinimumType)GetProcAddress(lib, "setFraCurrentMinimum");
        setFraCurrentMaximumType setFraCurrentMaximum =
            (setFraCurrentMaximumType)GetProcAddress(lib, "setFraCurrentMaximum");
        setFraPotentiostatModeType setFraPotentiostatMode =
            (setFraPotentiostatModeType)GetProcAddress(lib, "setFraPotentiostatMode");
        readFraSetupType readFraSetup             = (readFraSetupType)GetProcAddress(lib, "readFraSetup");
        readAcqSetupType readAcqSetup             = (readAcqSetupType)GetProcAddress(lib, "readAcqSetup");
        readAllAcqChannelsType readAllAcqChannels = (readAllAcqChannelsType)GetProcAddress(lib, "readAllAcqChannels");
        readAcqChannelType readAcqChannel         = (readAcqChannelType)GetProcAddress(lib, "readAcqChannel");
        enableAcqType enableAcq                   = (enableAcqType)GetProcAddress(lib, "enableAcq");
        disableAcqType disableAcq                 = (disableAcqType)GetProcAddress(lib, "disableAcq");

        std::cout << "all GetProcAddress done" << std::endl;

        void* connectionHandle = createZenniumConnection();
        bool state             = connectToTerm(connectionHandle, ipAddress, "ScriptRemote");
        if (not state) {
            std::cout << "could not connect to Term" << std::endl;
            return 0;
        } else {
            std::cout << "connected" << std::endl;
        }
        void* scriptHandle = createThalesRemoteScriptWrapper(connectionHandle);

        auto watchWorker  = new std::thread(&watchThread);
        auto onlineWorker = new std::thread(&onlineDataThread);

        replySize = 1000;
        state     = forceThalesIntoRemoteScript(scriptHandle, replyBuffer, &replySize);

        replySize = 1000;
        state     = calibrateOffsets(scriptHandle, replyBuffer, &replySize);

        replySize = 1000;
        state     = setCurrent(scriptHandle, replyBuffer, &replySize, 1e12);
        if (not state) {
            // error
            replySize = 1000;
            getErrorMessage(scriptHandle, replyBuffer, &replySize);
            std::cout << "example error string:" << std::endl;
            std::cout << replyBuffer << std::endl << std::flush;
        } else {
            // no error
            replySize = 1000;
            getErrorMessage(scriptHandle, replyBuffer, &replySize);
            std::cout << "example no error string:" << std::endl;
            std::cout << replyBuffer << std::endl << std::flush;
        }

        replySize = 1000;
        state     = setCurrent(scriptHandle, replyBuffer, &replySize, 0.01);
        if (not state) {
            // error
            replySize = 1000;
            getErrorMessage(scriptHandle, replyBuffer, &replySize);
            std::cout << "example error string:" << std::endl;
            std::cout << replyBuffer << std::endl << std::flush;
        } else {
            // no error
            replySize = 1000;
            getErrorMessage(scriptHandle, replyBuffer, &replySize);
            std::cout << "example no error string:" << std::endl;
            std::cout << replyBuffer << std::endl << std::flush;
        }

        double value;
        state = getCurrent(scriptHandle, &value);
        std::cout << state << value << std::endl;
        state = getPotential(scriptHandle, &value);
        std::cout << state << value << std::endl;
        state = getCurrent(scriptHandle, &value);
        std::cout << state << value << std::endl;
        state = getPotential(scriptHandle, &value);
        std::cout << state << value << std::endl;

        replySize = 1000;
        checkCVSetup(scriptHandle, replyBuffer, &replySize);
        std::cout << "CV setup:" << replyBuffer << std::endl;
        replySize = 1000;
        setPotentiostatMode(scriptHandle, replyBuffer, &replySize, "POTENTIOSTATIC");
        replySize = 1000;
        setEISNaming(scriptHandle, replyBuffer, &replySize, "COUNTER");
        replySize = 1000;
        setEISCounter(scriptHandle, replyBuffer, &replySize, 1);
        replySize = 1000;
        setEISOutputPath(scriptHandle, replyBuffer, &replySize, "c:\\thales\\temp\\test1");
        replySize = 1000;
        setEISOutputFileName(scriptHandle, replyBuffer, &replySize, "spectras_dll");
        replySize = 1000;
        setLowerFrequencyLimit(scriptHandle, replyBuffer, &replySize, 1000);
        setUpperFrequencyLimit(scriptHandle, replyBuffer, &replySize, 10000);

        std::cout << "EIS Start" << std::endl;

        replySize = 1000;
        state     = measureEIS(scriptHandle, replyBuffer, &replySize);
        if (not state) {
            replySize = 1000;
            getErrorMessage(scriptHandle, replyBuffer, &replySize);
            std::cout << "EIS error:" << replyBuffer << std::endl;
        }

        std::cout << "EIS End" << std::endl;

        replySize = 1000;

        state = getCurrent(scriptHandle, &value);
        std::cout << state << value << std::endl;
        state = getPotential(scriptHandle, &value);
        std::cout << state << value << std::endl;
        state = getCurrent(scriptHandle, &value);
        std::cout << state << value << std::endl;
        state = getPotential(scriptHandle, &value);
        std::cout << state << value << std::endl;

        replySize  = 1000;
        runThreads = false;

        std::cout << "disconnect script" << std::endl;
        disconnectFromTerm(connectionHandle);
        deleteThalesRemoteScriptWrapper(scriptHandle);
        deleteZenniumConnection(connectionHandle);

        watchWorker->join();
        onlineWorker->join();

        std::cout << "free DLL" << std::endl;
        BOOL fFreeResult = FreeLibrary(lib);
    }


    std::cout << "finish" << std::endl;
    std::cout.flush();
    return 0;
}
