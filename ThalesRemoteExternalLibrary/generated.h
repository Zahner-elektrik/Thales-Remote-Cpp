
#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"
#include "thalesfileinterface.h"

extern "C"
{
    __declspec(dllexport) bool __stdcall  getErrorMessage(void* handle, char* retval, int* retvalLen);
	__declspec(dllexport) ZenniumConnection* __stdcall  createZenniumConnection();
	__declspec(dllexport) void __stdcall  deleteZenniumConnection(ZenniumConnection* handle);
	__declspec(dllexport) bool __stdcall  disconnectFromTerm(ZenniumConnection* handle);
	__declspec(dllexport) bool __stdcall  connectToTerm(ZenniumConnection* handle, char const * address, char const* connectionName);
	__declspec(dllexport) bool __stdcall  setTimeout(ZenniumConnection* handle, int timeout);
	__declspec(dllexport) bool __stdcall  getTimeout(ZenniumConnection* handle, int* timeout);
	__declspec(dllexport) ThalesRemoteScriptWrapper* __stdcall  createThalesRemoteScriptWrapper(ZenniumConnection* handle);
    __declspec(dllexport) void __stdcall deleteThalesRemoteScriptWrapper(ThalesRemoteScriptWrapper* handle);
    __declspec(dllexport) ThalesFileInterface* __stdcall  createThalesFileInterface(ZenniumConnection* handle);
    __declspec(dllexport) void __stdcall deleteThalesFileInterface(ThalesFileInterface* handle);
    __declspec(dllexport) bool __stdcall enableAutomaticFileExchange(ThalesFileInterface* handle, char* retval, int* retvalLen, bool enable, char const* fileExtensions);
    __declspec(dllexport) bool __stdcall disableAutomaticFileExchange(ThalesFileInterface* handle, char* retval, int* retvalLen);
    __declspec(dllexport) bool __stdcall appendFilesToSkip(ThalesFileInterface* handle, char const* filename);
    __declspec(dllexport) bool __stdcall setSavePath(ThalesFileInterface* handle, char const* path);
    __declspec(dllexport) bool __stdcall enableSaveReceivedFilesToDisk(ThalesFileInterface* handle, char const* path, bool enable);
    __declspec(dllexport) bool __stdcall disableSaveReceivedFilesToDisk(ThalesFileInterface* handle);

	
__declspec(dllexport) bool __stdcall forceThalesIntoRemoteScript(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall getWorkstationHeartBeat(ThalesRemoteScriptWrapper* handle, int* retval );
	
__declspec(dllexport) bool __stdcall getCurrent(ThalesRemoteScriptWrapper* handle, double* retval );
	
__declspec(dllexport) bool __stdcall getPotential(ThalesRemoteScriptWrapper* handle, double* retval );
	
__declspec(dllexport) bool __stdcall setCurrent(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double current);
	
__declspec(dllexport) bool __stdcall setPotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setMaximumShuntIndex(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int shunt);
	
__declspec(dllexport) bool __stdcall setMinimumShuntIndex(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int shunt);
	
__declspec(dllexport) bool __stdcall setShuntIndex(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int index);
	
__declspec(dllexport) bool __stdcall setVoltageRangeIndex(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int index);
	
__declspec(dllexport) bool __stdcall selectPotentiostat(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int device);
	
__declspec(dllexport) bool __stdcall switchToSCPIControl(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall getSerialNumber(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall getDeviceName(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall calibrateOffsets(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall enablePotentiostat(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall disablePotentiostat(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall setPotentiostatMode(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* potentiostatMode);
	
__declspec(dllexport) bool __stdcall enableRuleFileUsage(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall disableRuleFileUsage(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall setupPAD4(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int card, int channel, bool enabled);
	
__declspec(dllexport) bool __stdcall enablePAD4(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall disablePAD4(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall readPAD4Setup(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall setFrequency(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double frequency);
	
__declspec(dllexport) bool __stdcall setAmplitude(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double amplitude);
	
__declspec(dllexport) bool __stdcall setNumberOfPeriods(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int numberOfPeriods);
	
__declspec(dllexport) bool __stdcall setUpperFrequencyLimit(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double frequency);
	
__declspec(dllexport) bool __stdcall setLowerFrequencyLimit(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double frequency);
	
__declspec(dllexport) bool __stdcall setStartFrequency(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double frequency);
	
__declspec(dllexport) bool __stdcall setUpperStepsPerDecade(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int steps);
	
__declspec(dllexport) bool __stdcall setLowerStepsPerDecade(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int steps);
	
__declspec(dllexport) bool __stdcall setUpperNumberOfPeriods(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int periods);
	
__declspec(dllexport) bool __stdcall setLowerNumberOfPeriods(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int periods);
	
__declspec(dllexport) bool __stdcall setScanStrategy(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* strategy);
	
__declspec(dllexport) bool __stdcall setScanDirection(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* direction);
	
__declspec(dllexport) bool __stdcall getImpedance(ThalesRemoteScriptWrapper* handle, double* real, double* imag );
	
__declspec(dllexport) bool __stdcall setEISNaming(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* naming);
	
__declspec(dllexport) bool __stdcall setEISCounter(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int number);
	
__declspec(dllexport) bool __stdcall setEISOutputPath(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* path);
	
__declspec(dllexport) bool __stdcall setEISOutputFileName(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* name);
	
__declspec(dllexport) bool __stdcall measureEIS(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall setCVStartPotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setCVUpperReversingPotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setCVLowerReversingPotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setCVEndPotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setCVStartHoldTime(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double time);
	
__declspec(dllexport) bool __stdcall setCVEndHoldTime(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double time);
	
__declspec(dllexport) bool __stdcall setCVScanRate(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double scanRate);
	
__declspec(dllexport) bool __stdcall setCVCycles(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double cycles);
	
__declspec(dllexport) bool __stdcall setCVSamplesPerCycle(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double samples);
	
__declspec(dllexport) bool __stdcall setCVMaximumCurrent(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double current);
	
__declspec(dllexport) bool __stdcall setCVMinimumCurrent(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double current);
	
__declspec(dllexport) bool __stdcall setCVOhmicDrop(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double ohmicDrop);
	
__declspec(dllexport) bool __stdcall enableCVAutoRestartAtCurrentOverflow(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall disableCVAutoRestartAtCurrentOverflow(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall enableCVAutoRestartAtCurrentUnderflow(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall disableCVAutoRestartAtCurrentUnderflow(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall enableCVAnalogFunctionGenerator(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall disableCVAnalogFunctionGenerator(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall setCVNaming(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* naming);
	
__declspec(dllexport) bool __stdcall setCVCounter(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int number);
	
__declspec(dllexport) bool __stdcall setCVOutputPath(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* path);
	
__declspec(dllexport) bool __stdcall setCVOutputFileName(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* name);
	
__declspec(dllexport) bool __stdcall checkCVSetup(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall readCVSetup(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall measureCV(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall setIEFirstEdgePotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setIESecondEdgePotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setIEThirdEdgePotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setIEFourthEdgePotential(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double potential);
	
__declspec(dllexport) bool __stdcall setIEFirstEdgePotentialRelation(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* relation);
	
__declspec(dllexport) bool __stdcall setIESecondEdgePotentialRelation(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* relation);
	
__declspec(dllexport) bool __stdcall setIEThirdEdgePotentialRelation(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* relation);
	
__declspec(dllexport) bool __stdcall setIEFourthEdgePotentialRelation(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* relation);
	
__declspec(dllexport) bool __stdcall setIEPotentialResolution(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double resolution);
	
__declspec(dllexport) bool __stdcall setIEMinimumWaitingTime(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double time);
	
__declspec(dllexport) bool __stdcall setIEMaximumWaitingTime(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double time);
	
__declspec(dllexport) bool __stdcall setIERelativeTolerance(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double tolerance);
	
__declspec(dllexport) bool __stdcall setIEAbsoluteTolerance(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double tolerance);
	
__declspec(dllexport) bool __stdcall setIEOhmicDrop(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double ohmicDrop);
	
__declspec(dllexport) bool __stdcall setIESweepMode(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* sweepMode);
	
__declspec(dllexport) bool __stdcall setIEScanRate(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double scanRate);
	
__declspec(dllexport) bool __stdcall setIEMaximumCurrent(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double current);
	
__declspec(dllexport) bool __stdcall setIEMinimumCurrent(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double current);
	
__declspec(dllexport) bool __stdcall setIENaming(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* naming);
	
__declspec(dllexport) bool __stdcall setIECounter(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int number);
	
__declspec(dllexport) bool __stdcall setIEOutputPath(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* path);
	
__declspec(dllexport) bool __stdcall setIEOutputFileName(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* name);
	
__declspec(dllexport) bool __stdcall checkIESetup(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall readIESetup(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall measureIE(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall selectSequence(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int number);
	
__declspec(dllexport) bool __stdcall setSequenceNaming(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* naming);
	
__declspec(dllexport) bool __stdcall setSequenceCounter(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , int number);
	
__declspec(dllexport) bool __stdcall setSequenceOutputPath(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* path);
	
__declspec(dllexport) bool __stdcall setSequenceOutputFileName(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , const char* name);
	
__declspec(dllexport) bool __stdcall runSequence(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen );
	
__declspec(dllexport) bool __stdcall enableFraMode(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , bool enabled);
	
__declspec(dllexport) bool __stdcall setFraVoltageInputGain(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraVoltageOutputGain(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraVoltageMinimum(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraVoltageMaximum(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraCurrentInputGain(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraCurrentOutputGain(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraCurrentMinimum(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraCurrentMaximum(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , double value);
	
__declspec(dllexport) bool __stdcall setFraPotentiostatMode(ThalesRemoteScriptWrapper* handle, char* retval, int* retvalLen , char const* potentiostatMode);

}
