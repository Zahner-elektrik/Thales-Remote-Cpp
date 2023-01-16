
#include "thalesremoteerror.h"
#include "generated.h"
#include <map>
#include <string>

std::map<ZenniumConnection *, std::shared_ptr<ZenniumConnection>> zenniumConnections;
std::map<ThalesRemoteScriptWrapper *, std::shared_ptr<ThalesRemoteScriptWrapper>> scriptWrappers;
std::map<ThalesFileInterface *, std::shared_ptr<ThalesFileInterface>> fileInterfaces;
std::map<void *, std::string> errorMessages;

extern "C"
{
    __declspec(dllexport) bool __stdcall getErrorMessage(void *handle, char *retval, int *retvalLen)
    {
        *retvalLen = errorMessages.at(handle).copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
        retval[*retvalLen] = '\0';
        *retvalLen += 1;
        return true;
    }

    __declspec(dllexport) ZenniumConnection *__stdcall createZenniumConnection()
    {
        auto connection = std::make_shared<ZenniumConnection>();
        zenniumConnections.insert({connection.get(), connection});
        errorMessages.insert({connection.get(), ""});

        return connection.get();
    }

    __declspec(dllexport) void __stdcall deleteZenniumConnection(ZenniumConnection *handle)
    {
        zenniumConnections.erase(handle);
        errorMessages.erase(handle);
    }

    __declspec(dllexport) bool __stdcall disconnectFromTerm(ZenniumConnection *handle)
    {
        try
        {
            zenniumConnections.at(handle)->disconnectFromTerm();
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall connectToTerm(ZenniumConnection *handle, char const *address, char const *connectionName)
    {
        try
        {
            return zenniumConnections.at(handle)->connectToTerm(std::string(address), std::string(connectionName));
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setTimeout(ZenniumConnection *handle, int timeout)
    {
        zenniumConnections.at(handle)->setTimeout(std::chrono::duration<int, std::milli>(timeout));
        return true;
    }
    __declspec(dllexport) bool __stdcall getTimeout(ZenniumConnection *handle, int *timeout)
    {
        auto retval = zenniumConnections.at(handle)->getTimeout();
        *timeout = std::chrono::milliseconds(retval).count();
        return true;
    }

    __declspec(dllexport) ThalesRemoteScriptWrapper *__stdcall createThalesRemoteScriptWrapper(ZenniumConnection *handle)
    {

        auto wrapper = std::make_shared<ThalesRemoteScriptWrapper>(handle);
        scriptWrappers.insert({wrapper.get(), wrapper});
        errorMessages.insert({wrapper.get(), ""});

        return wrapper.get();
    }

    __declspec(dllexport) void __stdcall deleteThalesRemoteScriptWrapper(ThalesRemoteScriptWrapper *handle)
    {
        scriptWrappers.erase(handle);
        errorMessages.erase(handle);
    }

    __declspec(dllexport) ThalesFileInterface *__stdcall createThalesFileInterface(ZenniumConnection *handle)
    {

        auto wrapper = std::make_shared<ThalesFileInterface>(handle);
        fileInterfaces.insert({wrapper.get(), wrapper});
        errorMessages.insert({wrapper.get(), ""});

        return wrapper.get();
    }

    __declspec(dllexport) void __stdcall deleteThalesFileInterface(ThalesFileInterface *handle)
    {
        fileInterfaces.erase(handle);
        errorMessages.erase(handle);
    }

    __declspec(dllexport) bool __stdcall enableAutomaticFileExchange(ThalesFileInterface *handle, char *retval, int *retvalLen, bool enable, char const *fileExtensions)
    {
        try
        {
            auto returned = fileInterfaces.at(handle)->enableAutomaticFileExchange(enable, std::string(fileExtensions));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disableAutomaticFileExchange(ThalesFileInterface *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = fileInterfaces.at(handle)->disableAutomaticFileExchange();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall appendFilesToSkip(ThalesFileInterface *handle, char const *filename)
    {
        try
        {
            fileInterfaces.at(handle)->appendFilesToSkip(std::string(filename));
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setSavePath(ThalesFileInterface *handle, char const *path)
    {
        try
        {
            fileInterfaces.at(handle)->setSavePath(std::string(path));
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enableSaveReceivedFilesToDisk(ThalesFileInterface *handle, char const *path, bool enable)
    {
        try
        {
            fileInterfaces.at(handle)->enableSaveReceivedFilesToDisk(std::string(path), enable);
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disableSaveReceivedFilesToDisk(ThalesFileInterface *handle)
    {
        try
        {
            fileInterfaces.at(handle)->disableSaveReceivedFilesToDisk();
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    PotentiostatMode stringToPotentiostatMode(std::string string)
    {
        if (string == "")
        {
            throw ThalesRemoteError("enum member does not exist.");
        }

        else if (string == "POTENTIOSTATIC")
        {
            return PotentiostatMode::POTENTIOSTATIC;
        }

        else if (string == "GALVANOSTATIC")
        {
            return PotentiostatMode::GALVANOSTATIC;
        }

        else if (string == "PSEUDOGALVANOSTATIC")
        {
            return PotentiostatMode::PSEUDOGALVANOSTATIC;
        }

        else
        {
            throw ThalesRemoteError("enum member does not exist.");
        }
    }

    ScanStrategy stringToScanStrategy(std::string string)
    {
        if (string == "")
        {
            throw ThalesRemoteError("enum member does not exist.");
        }

        else if (string == "SINGLE_SINE")
        {
            return ScanStrategy::SINGLE_SINE;
        }

        else if (string == "MULTI_SINE")
        {
            return ScanStrategy::MULTI_SINE;
        }

        else if (string == "TABLE")
        {
            return ScanStrategy::TABLE;
        }

        else
        {
            throw ThalesRemoteError("enum member does not exist.");
        }
    }

    ScanDirection stringToScanDirection(std::string string)
    {
        if (string == "")
        {
            throw ThalesRemoteError("enum member does not exist.");
        }

        else if (string == "START_TO_MIN")
        {
            return ScanDirection::START_TO_MIN;
        }

        else if (string == "START_TO_MAX")
        {
            return ScanDirection::START_TO_MAX;
        }

        else
        {
            throw ThalesRemoteError("enum member does not exist.");
        }
    }

    NamingRule stringToNamingRule(std::string string)
    {
        if (string == "")
        {
            throw ThalesRemoteError("enum member does not exist.");
        }

        else if (string == "DATETIME")
        {
            return NamingRule::DATETIME;
        }

        else if (string == "COUNTER")
        {
            return NamingRule::COUNTER;
        }

        else if (string == "INDIVIDUAL")
        {
            return NamingRule::INDIVIDUAL;
        }

        else
        {
            throw ThalesRemoteError("enum member does not exist.");
        }
    }

    PotentialRelation stringToPotentialRelation(std::string string)
    {
        if (string == "")
        {
            throw ThalesRemoteError("enum member does not exist.");
        }

        else if (string == "ABSOLUTE_RELATED")
        {
            return PotentialRelation::ABSOLUTE_RELATED;
        }

        else if (string == "RELATIVE_RELATED")
        {
            return PotentialRelation::RELATIVE_RELATED;
        }

        else
        {
            throw ThalesRemoteError("enum member does not exist.");
        }
    }

    IESweepMode stringToIESweepMode(std::string string)
    {
        if (string == "")
        {
            throw ThalesRemoteError("enum member does not exist.");
        }

        else if (string == "STEADYSTATE")
        {
            return IESweepMode::STEADYSTATE;
        }

        else if (string == "FIXEDSAMPLING")
        {
            return IESweepMode::FIXEDSAMPLING;
        }

        else if (string == "DYNAMICSCAN")
        {
            return IESweepMode::DYNAMICSCAN;
        }

        else
        {
            throw ThalesRemoteError("enum member does not exist.");
        }
    }

    __declspec(dllexport) bool __stdcall forceThalesIntoRemoteScript(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->forceThalesIntoRemoteScript();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall getWorkstationHeartBeat(ThalesRemoteScriptWrapper *handle, int *retval)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->getWorkstationHeartBeat();
            *retval = returned;
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall getCurrent(ThalesRemoteScriptWrapper *handle, double *retval)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->getCurrent();
            *retval = returned;
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall getPotential(ThalesRemoteScriptWrapper *handle, double *retval)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->getPotential();
            *retval = returned;
            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCurrent(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double current)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCurrent(current);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setPotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setPotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setMaximumShuntIndex(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int shunt)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setMaximumShuntIndex(shunt);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setMinimumShuntIndex(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int shunt)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setMinimumShuntIndex(shunt);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setShuntIndex(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int index)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setShuntIndex(index);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setVoltageRangeIndex(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int index)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setVoltageRangeIndex(index);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall selectPotentiostat(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int device)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->selectPotentiostat(device);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall selectPotentiostatWithoutPotentiostatStateChange(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int device)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->selectPotentiostatWithoutPotentiostatStateChange(device);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall switchToSCPIControl(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->switchToSCPIControl();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall switchToSCPIControlWithoutPotentiostatStateChange(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->switchToSCPIControlWithoutPotentiostatStateChange();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall getSerialNumber(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->getSerialNumber();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall getDeviceName(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->getDeviceName();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall calibrateOffsets(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->calibrateOffsets();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enablePotentiostat(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enablePotentiostat(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disablePotentiostat(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->disablePotentiostat();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setPotentiostatMode(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *potentiostatMode)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setPotentiostatMode(stringToPotentiostatMode(std::string(potentiostatMode)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enableRuleFileUsage(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enableRuleFileUsage(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disableRuleFileUsage(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->disableRuleFileUsage();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setupPAD4(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int card, int channel, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setupPAD4(card, channel, enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enablePAD4(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enablePAD4(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disablePAD4(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->disablePAD4();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall readPAD4Setup(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->readPAD4Setup();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFrequency(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double frequency)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFrequency(frequency);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setAmplitude(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double amplitude)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setAmplitude(amplitude);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setNumberOfPeriods(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int numberOfPeriods)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setNumberOfPeriods(numberOfPeriods);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setUpperFrequencyLimit(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double frequency)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setUpperFrequencyLimit(frequency);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setLowerFrequencyLimit(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double frequency)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setLowerFrequencyLimit(frequency);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setStartFrequency(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double frequency)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setStartFrequency(frequency);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setUpperStepsPerDecade(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int steps)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setUpperStepsPerDecade(steps);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setLowerStepsPerDecade(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int steps)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setLowerStepsPerDecade(steps);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setUpperNumberOfPeriods(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int periods)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setUpperNumberOfPeriods(periods);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setLowerNumberOfPeriods(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int periods)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setLowerNumberOfPeriods(periods);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setScanStrategy(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *strategy)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setScanStrategy(stringToScanStrategy(std::string(strategy)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setScanDirection(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *direction)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setScanDirection(stringToScanDirection(std::string(direction)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall getImpedance(ThalesRemoteScriptWrapper *handle, double *real, double *imag)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->getImpedance();

            *real = returned.real();
            *imag = returned.imag();

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setEISNaming(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *naming)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setEISNaming(stringToNamingRule(std::string(naming)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setEISCounter(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int number)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setEISCounter(number);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setEISOutputPath(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *path)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setEISOutputPath(std::string(path));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setEISOutputFileName(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *name)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setEISOutputFileName(std::string(name));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall measureEIS(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->measureEIS();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVStartPotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVStartPotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVUpperReversingPotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVUpperReversingPotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVLowerReversingPotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVLowerReversingPotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVEndPotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVEndPotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVStartHoldTime(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double time)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVStartHoldTime(time);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVEndHoldTime(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double time)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVEndHoldTime(time);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVScanRate(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double scanRate)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVScanRate(scanRate);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVCycles(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double cycles)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVCycles(cycles);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVSamplesPerCycle(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double samples)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVSamplesPerCycle(samples);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVMaximumCurrent(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double current)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVMaximumCurrent(current);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVMinimumCurrent(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double current)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVMinimumCurrent(current);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVOhmicDrop(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double ohmicDrop)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVOhmicDrop(ohmicDrop);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enableCVAutoRestartAtCurrentOverflow(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enableCVAutoRestartAtCurrentOverflow(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disableCVAutoRestartAtCurrentOverflow(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->disableCVAutoRestartAtCurrentOverflow();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enableCVAutoRestartAtCurrentUnderflow(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enableCVAutoRestartAtCurrentUnderflow(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disableCVAutoRestartAtCurrentUnderflow(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->disableCVAutoRestartAtCurrentUnderflow();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enableCVAnalogFunctionGenerator(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enableCVAnalogFunctionGenerator(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall disableCVAnalogFunctionGenerator(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->disableCVAnalogFunctionGenerator();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVNaming(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *naming)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVNaming(stringToNamingRule(std::string(naming)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVCounter(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int number)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVCounter(number);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVOutputPath(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *path)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVOutputPath(std::string(path));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setCVOutputFileName(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *name)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setCVOutputFileName(std::string(name));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall checkCVSetup(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->checkCVSetup();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall readCVSetup(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->readCVSetup();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall measureCV(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->measureCV();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEFirstEdgePotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEFirstEdgePotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIESecondEdgePotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIESecondEdgePotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEThirdEdgePotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEThirdEdgePotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEFourthEdgePotential(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double potential)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEFourthEdgePotential(potential);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEFirstEdgePotentialRelation(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *relation)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEFirstEdgePotentialRelation(stringToPotentialRelation(std::string(relation)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIESecondEdgePotentialRelation(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *relation)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIESecondEdgePotentialRelation(stringToPotentialRelation(std::string(relation)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEThirdEdgePotentialRelation(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *relation)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEThirdEdgePotentialRelation(stringToPotentialRelation(std::string(relation)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEFourthEdgePotentialRelation(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *relation)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEFourthEdgePotentialRelation(stringToPotentialRelation(std::string(relation)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEPotentialResolution(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double resolution)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEPotentialResolution(resolution);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEMinimumWaitingTime(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double time)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEMinimumWaitingTime(time);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEMaximumWaitingTime(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double time)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEMaximumWaitingTime(time);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIERelativeTolerance(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double tolerance)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIERelativeTolerance(tolerance);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEAbsoluteTolerance(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double tolerance)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEAbsoluteTolerance(tolerance);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEOhmicDrop(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double ohmicDrop)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEOhmicDrop(ohmicDrop);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIESweepMode(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *sweepMode)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIESweepMode(stringToIESweepMode(std::string(sweepMode)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEScanRate(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double scanRate)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEScanRate(scanRate);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEMaximumCurrent(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double current)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEMaximumCurrent(current);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEMinimumCurrent(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double current)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEMinimumCurrent(current);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIENaming(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *naming)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIENaming(stringToNamingRule(std::string(naming)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIECounter(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int number)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIECounter(number);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEOutputPath(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *path)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEOutputPath(std::string(path));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setIEOutputFileName(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *name)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setIEOutputFileName(std::string(name));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall checkIESetup(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->checkIESetup();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall readIESetup(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->readIESetup();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall measureIE(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->measureIE();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall selectSequence(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int number)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->selectSequence(number);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setSequenceNaming(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *naming)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setSequenceNaming(stringToNamingRule(std::string(naming)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setSequenceCounter(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, int number)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setSequenceCounter(number);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setSequenceOutputPath(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *path)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setSequenceOutputPath(std::string(path));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setSequenceOutputFileName(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, const char *name)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setSequenceOutputFileName(std::string(name));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall runSequence(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->runSequence();

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall enableFraMode(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, bool enabled)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->enableFraMode(enabled);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraVoltageInputGain(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraVoltageInputGain(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraVoltageOutputGain(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraVoltageOutputGain(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraVoltageMinimum(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraVoltageMinimum(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraVoltageMaximum(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraVoltageMaximum(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraCurrentInputGain(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraCurrentInputGain(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraCurrentOutputGain(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraCurrentOutputGain(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraCurrentMinimum(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraCurrentMinimum(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraCurrentMaximum(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, double value)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraCurrentMaximum(value);

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }

    __declspec(dllexport) bool __stdcall setFraPotentiostatMode(ThalesRemoteScriptWrapper *handle, char *retval, int *retvalLen, char const *potentiostatMode)
    {
        try
        {
            auto returned = scriptWrappers.at(handle)->setFraPotentiostatMode(stringToPotentiostatMode(std::string(potentiostatMode)));

            *retvalLen = returned.copy(retval, static_cast<std::basic_string<char>::size_type>(*retvalLen - 1), 0);
            retval[*retvalLen] = '\0';
            *retvalLen += 1;

            return true;
        }
        catch (const ZahnerError &ex)
        {
            errorMessages.at(handle) = ex.getMessage();
            return false;
        }
    }
}
