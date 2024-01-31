/******************************************************************
 *  ____       __                        __    __   __      _ __
 * /_  / ___ _/ /  ___  ___ ___________ / /__ / /__/ /_____(_) /__
 *  / /_/ _ `/ _ \/ _ \/ -_) __/___/ -_) / -_)  '_/ __/ __/ /  '_/
 * /___/\_,_/_//_/_//_/\__/_/      \__/_/\__/_/\_\\__/_/ /_/_/\_\
 *
 * Copyright 2024 ZAHNER-elektrik I. Zahner-Schiller GmbH & Co. KG
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

#ifndef THALESREMOTESCRIPTWRAPPER_H
#define THALESREMOTESCRIPTWRAPPER_H

#include <regex>
#include <complex>

#include "thalesremoteconnection.h"

enum class PotentiostatMode {
    POTENTIOSTATIC,         /**< Potentiostatic operation of the potentiostat, as a voltage source. */
    GALVANOSTATIC,          /**< Galvanostatic operation of the potentiostat, as a current source. */
    PSEUDOGALVANOSTATIC     /**< Potentiostatic operation only for EIS, the potential for a target current is determined automatically. Measurement is done with voltage amplitude. */
};

enum class ScanStrategy{
    SINGLE_SINE,    /**< Measurement of individual frequency points in the low-frequency range. */
    MULTI_SINE,     /**< Measurement with multisine in the low-frequency range. */
    TABLE           /**< Specify a table with frequency points at which the measurements are carried out. */
};

enum class ScanDirection{
    START_TO_MIN,   /**< Scan at first from start to lower frequency. */
    START_TO_MAX    /**< Scan at first from start to maximum frequency. */
};

enum class NamingRule{
    DATETIME,    /**< Extension of the specified name with date and time. */
    COUNTER,     /**< Extension of the specified name with an sequential number. */
    INDIVIDUAL   /**< No extension each file must be named individually, otherwise it will be overwritten. */
};

enum class PotentialRelation{
    ABSOLUTE_RELATED, /**< Absolute relation of the potential. */
    RELATIVE_RELATED  /**< Relative relation of the potential. */
};

enum class IESweepMode{
    STEADYSTATE,    /**< The explanation of the modes can be found in the IE manual. */
    FIXEDSAMPLING,  /**< The explanation of the modes can be found in the IE manual. */
    DYNAMICSCAN     /**< The explanation of the modes can be found in the IE manual. */
};

enum class Pad4Mode{
    VOLTAGE,    /**< Voltage measurement */
    CURRENT     /**< The explanation of the modes can be found in the IE manual. */
};

/** The ThalesRemoteScriptWrapper class
 *
 *  Wrapper that uses the ThalesRemoteConnection class.
 *  The commands are explained in https://doc.zahner.de/manuals/remote2.pdf .
 *  In the document you can also find a table with error numbers which are returned.
 */
class ThalesRemoteScriptWrapper
{
public:

    /** Constructor. Needs a connected ThalesRemoteConnection */
    ThalesRemoteScriptWrapper(ZenniumConnection * const remoteConnection);

    /** Directly execute a query to Remote Script.
     *
     * \param  command The query string, e.g. "IMPEDANCE" or "Pset=0"
     *
     * \return The response string from the device.
     */
    std::string executeRemoteCommand(std::string command);

    /** Prompts Thales to start the Remote Script
     *
     * Will switch a running Thales from anywhere like the main menu after
     * startup to running measurements into Remote Script so it can process
     * further requests.
     *
     * \warning This happens rather quickly if Thales is idle in main menu but
     *          may take some time if it's perofming an EIS measurement or doing
     *          something else. For high stability applications 20 seconds would
     *          probably be a save bet.
     */
    std::string forceThalesIntoRemoteScript();

    /** Hide Thales window
     *
     * This is for remote integrations to prevent operation of the GUI.
     * Thales is still visible in the taskbar but can no longer be maximized.
     *
     * \return The response string from the device.
     */
    std::string hideWindow();

    /** Show Thales window
     *
     * If the Thales window has been hidden, it can be displayed again with this command.
     *
     * \return The response string from the device.
     */
    std::string showWindow();

    /** Get Thales Version.
     *
     * Reads the current Thales version. Same version as in the application title line.
     *
     * \return The response string from the device.
     */
    std::string getThalesVersion();

    /** Read the HeartBeat from the Term.
     *
     * Query the HeartBeat time from the Term software for the workstation and the Thales software accordingly.
     * The complete documentation can be found in the DevCli-Manual Page 8.
     *
     * The timeout is not set by default and the command blocks indefinitely.
     * However, a time in seconds can optionally be specified for the timeout. When the timeout expires, an exception is thrown by the socket.
     *
     * \return The HeartBeat time in milli seconds.
     */
    int getWorkstationHeartBeat();

    /** Read the measured current from the device.
     *
     * \return The current current value.
     */
    double getCurrent();

    /** Read the measured voltage from the device.
     *
     * \return The current voltage value.
     */
    double getPotential();

    /** Read the measured voltage from the device.
     *
     * \return The current voltage value.
     */
    double getVoltage();

    /** Set the output current.
     *
     * \param  current The output current to set.
     *
     * \return The response string from the device.
     */
    std::string setCurrent(double current);

    /** Set the output potential.
     *
     * \param  potential The output potential to set.
     *
     * \return The response string from the device.
     */
    std::string setPotential(double potential);

    /** Set the maximum shunt for measurement.
     *
     * Set the maximum shunt index for impedance measurements.
     *
     * \param  shunt The number of the shunt.
     *
     * \return The response string from the device.
     */
    std::string setMaximumShuntIndex(int shunt);

    /** Set the minimum shunt for measurement.
     *
     * Set the minimum shunt index for impedance measurements.
     *
     * \param  shunt The number of the shunt.
     *
     * \return The response string from the device.
     */
    std::string setMinimumShuntIndex(int shunt);

    /** Set the shunt for measurement.
     *
     * Fixes the shunt to the passed index.
     *
     * \param  index The number of the shunt.
     *
     * \return The response string from the device.
     */
    std::string setShuntIndex(int index);

    /** Set the voltage range for measurement.
     *
     * If a Zennium, Zennium E, Zennium E4 or a device from the IM6 series is used,
     * the set index must match the U-buffer.
     * If the U-buffer does not match the set value, the measurement is wrong.
     * The Zennium pro, Zennium X and Zennium XC series automatically change the range.
     *
     * \param  index The number of the voltage range.
     *
     * \return The response string from the device.
     */
    std::string setVoltageRangeIndex(int index);

    /** Set the device for output.
     *
     * Device which is to be selected, on which the settings are output.
     * First, the device must be selected.
     * Only then can devices other than the internal main potentiostat be configured.
     * It is only possible to measure with one device at a time.
     * For example, impedance cannot be measured on two devices at once.
     *
     * \param  device Number of the device. 0 = Main. 1 = EPC channel 1 and so on.
     *
     * \return The response string from the device.
     */
    std::string selectPotentiostat(int device);

    /** Set the device for output.
     *
     * Device which is to be selected, on which the settings are output.
     * First, the device must be selected.
     * Only then can devices other than the internal main potentiostat be configured.
     * It is only possible to measure with one device at a time.
     * For example, impedance cannot be measured on two devices at once.
     *
     * Selects the device in the current state without changing it. So when the potentiostat is on it stays on.
     * With this command, the potentiostat is still switched off for about 50 ms.
     *
     * \param  device Number of the device. 0 = Main. 1 = EPC channel 1 and so on.
     *
     * \return The response string from the device.
     */
    std::string selectPotentiostatWithoutPotentiostatStateChange(int device);

    /** Change away from operation as EPC device to SCPI operation.
     *
     * This command works only with external potentiostats of the latest generation PP212, PP222, PP242 and XPOT2.
     * After this command they are no longer accessible with the EPC interface.
     * Then you can connect to the potentiostat with USB via the Comports.
     * The change back to EPC operation is also done explicitly from the USB side.
     *
     * This command switches off the potentiostat.
     *
     * \return The response string from the device.
     */
    std::string switchToSCPIControl();

    /** Change away from operation as EPC device to SCPI operation.
     *
     * This command works only with external potentiostats of the latest generation PP212, PP222, PP242 and XPOT2.
     * After this command they are no longer accessible with the EPC interface.
     * Then you can connect to the potentiostat with USB via the Comports.
     * The change back to EPC operation is also done explicitly from the USB side.
     *
     * This command does not change the state of the potentiostat. If it is on, the potentiostat remains on.
     *
     * \return The response string from the device.
     */
    std::string switchToSCPIControlWithoutPotentiostatStateChange();

    /** Get the serialnumber of the active device.
     *
     * The active device is selected using the ThalesRemoteScriptWrapper::selectPotentiostat method.
     *
     * \return The device serial number.
     */
    std::string getSerialNumber();

    /** Get the name of the active device.
     *
     * The active device is selected using the ThalesRemoteScriptWrapper::selectPotentiostat method.
     *
     * \return The device name.
     */
    std::string getDeviceName();

    /** Read the currently set parameters
     *
     * A string containing the configuration is returned.
     *   For Example: OK;SETUP;Pset=1.0000e-05;Cset=1.0000e-06;Frq=1.0000e+03;Ampl=0.0000e+00;Nw=1;Pot=0;Gal=0;GAL=0;Cmin=-3.0000e+00;Cmax=3.0000e+00;Pmin=-5.2377e+00;Pmax=5.2377e+00;DEV=0;EPC=0;MAXDEV=4;ENDSETUP
     *
     * \return The response string from the device.
     */
    std::string readSetup();

    /** Perform offset calibration on the device.
     *
     * When the instrument has warmed up for about 30 minutes,
     * this command can be used to perform the offset calibration again.
     *
     * \return The response string from the device.
     */
    std::string calibrateOffsets();

    /** Switch the potentiostat on or off.
     *
     * \param  enabled Switches the potentiostat on if true and off if false.
     *
     * \return The response string from the device.
     */
    std::string enablePotentiostat(bool enabled = true);

    /** Switch the potentiostat off.
     *
     * \return The response string from the device.
     */
    std::string disablePotentiostat();

    /** Set the coupling of the potentiostat.
     *
     * \param  potentiostatMode The coupling of the potentiostat.
     *
     * \return The response string from the device.
     */
    std::string setPotentiostatMode(PotentiostatMode potentiostatMode);

    /** Enable the usage of a rule file.
     *
     *  If the usage of the rule file is activated all the parameters required
     *  for the EIS, CV, and/or IE are taken from the rule file.
     *  The exact usage can be found in the remote manual.
     *
     * \param  enabled Switches the potentiostat on if true and off if false.
     *
     * \return The response string from the device.
     */
    std::string enableRuleFileUsage(bool enabled = true);

    /** Disable the usage of a rule file.
     *
     *  If the usage of the rule file is activated all the parameters required
     *  for the EIS, CV, and/or IE are taken from the rule file.
     *  The exact usage can be found in the remote manual.
     *
     * \return The response string from the device.
     */
    std::string disableRuleFileUsage();

    /*
     * Section with PAD4 Commands
     */

    /** Setting a single channel of a PAD4 card for an EIS measurement.
     *
     *  Each channel of the Pad4 card must be configured separately and then the PAD4 must be activated with ThalesRemoteScriptWrapper::enablePAD4.
     *
     * \param  card The number of the card starting at 1 and up to 4.
     * \param  channel The channel of the card starting at 1 and up to 4.
     * \param  enabled True to enable the channel.
     *
     * \return The response string from the device.
     */
    std::string setupPad4Channel(int card, int channel, bool enabled);

    /** Setting a single channel of a PAD4 card for an EIS measurement.
     *
     *  Each channel of the Pad4 card must be configured separately and then the PAD4 must be activated with ThalesRemoteScriptWrapper::enablePAD4.
     *  Each channel can be given a different voltage range or shunt.
     *  The user can switch the type of PAD4 channels between voltage sense (standard configuration) and current sense (with additional shunt resistor).
     *
     * \param  card The number of the card starting at 1 and up to 4.
     * \param  channel The channel of the card starting at 1 and up to 4.
     * \param  enabled True to enable the channel.
     * \param  voltageRange input voltage range, if this differs from 4 V
     *
     * \return The response string from the device.
     */
    std::string setupPad4ChannelWithVoltageRange(int card, int channel, bool enabled, double voltageRange);

    /** Setting a single channel of a PAD4 card for an EIS measurement.
     *
     *  Each channel of the Pad4 card must be configured separately and then the PAD4 must be activated with ThalesRemoteScriptWrapper::enablePAD4.
     *  Each channel can be given a different voltage range or shunt.
     *  The user can switch the type of PAD4 channels between voltage sense (standard configuration) and current sense (with additional shunt resistor).
     *
     * \param  card The number of the card starting at 1 and up to 4.
     * \param  channel The channel of the card starting at 1 and up to 4.
     * \param  enabled True to enable the channel.
     * \param  shuntResistor shunt resistor, which is used. Only used if ThalesRemoteScriptWrapper::setupPad4ModeGlobal is set to Pad4Mode::CURRENT
     *
     * \return The response string from the device.
     */
    std::string setupPad4ChannelWithShuntResistor(int card, int channel, bool enabled, double shuntResistor);

    /** Switch between current and voltage measurement
     *
     * The user can switch the type of PAD4 channels between voltage sense (standard configuration) and current sense (with additional shunt resistor).
     *
     * \param  mode current or voltage measurement
     *
     * \return The response string from the device.
     */
    std::string setupPad4ModeGlobal(Pad4Mode mode);

    /** Enable the configured PAD4 channels.
     *
     *  Each channel of the Pad4 card must be configured separately.
     *
     * \param  enabled True to enable PAD4 measurements.
     *
     * \return The response string from the device.
     */
    std::string enablePad4Global(bool enabled = true);

    /** Disable the configured PAD4 channels.
     *
     *  Each channel of the Pad4 card must be configured separately.
     *
     * \return The response string from the device.
     */
    std::string disablePad4Global();

    /** Read the set parameters.
     *
     *  Reading the set PAD4 configuration.
     *
     * \return The response string from the device.
     */
    std::string readPAD4Setup();

    /*
     * Section with settings for single impedance and EIS measurements.
     */


    /** Set the output frequency for single frequency impedance.
     *
     * \param  frequency The output frequency for Impedance measurement to set.
     *
     * \return The response string from the device.
     */
    std::string setFrequency(double frequency);

    /** Set the output amplitude.
     *
     *  The amplitude is valid for one impedance measurement and for EIS.
     *  The amplitude is specified in the basic unit V or A.
     *
     * \param  amplitude The amplitude in A or V.
     *
     * \return The response string from the device.
     */
    std::string setAmplitude(double amplitude);

    /** Set the number of periods to average for one impedance measurement.
     *
     * \param  numberOfPeriods The number of periods / waves to average.
     *
     * \return The response string from the device.
     */
    std::string setNumberOfPeriods(int numberOfPeriods);

    /** Set the upper frequency limit for EIS measurements.
     *
     * \param  frequency The upper frequency limit.
     *
     * \return The response string from the device.
     */
    std::string setUpperFrequencyLimit(double frequency);

    /** Set the lower frequency limit for EIS measurements.
     *
     * \param  frequency The lower frequency limit.
     *
     * \return The response string from the device.
     */
    std::string setLowerFrequencyLimit(double frequency);

    /** Set the start frequency for EIS measurements.
     *
     * \param  frequency The start frequency.
     *
     * \return The response string from the device.
     */
    std::string setStartFrequency(double frequency);

    /** Set the number of steps per decade in frequency range above 66 Hz for EIS measurements.
     *
     * \param  steps The number of steps per decade.
     *
     * \return The response string from the device.
     */
    std::string setUpperStepsPerDecade(int steps);

    /** Set the number of steps per decade in frequency range below 66 Hz for EIS measurements.
     *
     * \param  steps The number of steps per decade.
     *
     * \return The response string from the device.
     */
    std::string setLowerStepsPerDecade(int steps);

    /** Set the number of periods to measure in frequency range above 66 Hz for EIS measurements.
     *
     *  Must be greater than ThalesRemoteScriptWrapper::setLowerNumberOfPeriods.
     *
     * \param  periods The number of periods.
     *
     * \return The response string from the device.
     */
    std::string setUpperNumberOfPeriods(int periods);

    /** Set the number of periods to measure in frequency range below 66 Hz for EIS measurements.
     *
     *  Must be smaller than ThalesRemoteScriptWrapper::setUpperNumberOfPeriods.
     *
     * \param  periods The number of periods.
     *
     * \return The response string from the device.
     */
    std::string setLowerNumberOfPeriods(int periods);

    /** Set the scan strategy for EIS measurements.
     *
     * \param  strategy The scan strategy for EIS measurements.
     *
     * \return The response string from the device.
     */
    std::string setScanStrategy(ScanStrategy strategy);

    /** Set the scan direction for EIS measurements.
     *
     * \param  direction The scan direction for EIS measurements.
     *
     * \return The response string from the device.
     */
    std::string setScanDirection(ScanDirection direction);

    /** Measure the impedance at the set frequency, amplitude and averages.
     *
     * \return The complex impedance at the measured point.
     */
    std::complex<double> getImpedance();

    /** Measure the impedance at the set amplitude with set averages.
     *
     * \param  frequency the frequency to measure the impedance at.
     *
     * \return The complex impedance at the measured point.
     */
    std::complex<double> getImpedance(double frequency);

    /** Measure the impedace with activated PAD4 channels with all possible parameters
     *
     * \param  frequency The frequency to measure the impedance at.
     * \param  amplitude The amplitude to measure the impedance with. In Volt if potentiostatic mode or Ampere for galvanostatic mode.
     * \param  numberOfPeriods The number of periods / waves to average.
     *
     * \return The complex impedance at the measured point.
     */
    std::complex<double> getImpedance(double frequency, double amplitude, int numberOfPeriods = 1);

     /** Measure the impedance with activated PAD4 channels at the set frequency, amplitude and averages.
      * 
      * The function returns a string containing all impedance results. impedance is the MAIN channel all other padXX= are the PAD4 channels. Channels which are deactivated have the value 0.
      *
      * \return String containing the impedance results.
      */
    std::string getImpedancePad4();

    /** Measure the impedance at the set amplitude with set averages.
     *
     * \param  frequency the frequency to measure the impedance at.
     *
     * \return String containing the impedance results.
     */
    std::string getImpedancePad4(double frequency);

    /** Measure the impedace with activated PAD4 channels with all possible parameters
     *
     * \param  frequency The frequency to measure the impedance at.
     * \param  amplitude The amplitude to measure the impedance with. In Volt if potentiostatic mode or Ampere for galvanostatic mode.
     * \param  numberOfPeriods The number of periods / waves to average.
     *
     * \return String containing the impedance results.
     */
    std::string getImpedancePad4(double frequency, double amplitude, int numberOfPeriods = 1);

    /** Set the measurement naming rule.
     *
     * \param  naming The measurement naming rule.
     *
     * \return The response string from the device.
     */
    std::string setEISNaming(NamingRule naming);

    /** Set the current number of measurement for filename.
     *
     *  Current number for the file name for measurements which is used next and then incremented.
     *
     * \param  number The next measurement number.
     *
     * \return The response string from the device.
     */
    std::string setEISCounter(int number);

    /** Set the path where the EIS measurements should be stored.
     *
     *  The directory must exist.
     *  The results must be stored on the C hard disk.
     *  If an error occurs test an alternative path or "C:\THALES\temp".
     *
     * \param  path The path to the directory.
     *
     * \return The response string from the device.
     */
    std::string setEISOutputPath(std::string path);

    /** Set the basic output filename.
     *
     *  The basic name of the file, which is extended by a sequential number or the date and time.
     *  Only numbers, underscores and letters from a-Z may be used.
     *  If the name is set to "individual", the file with the same name must not yet exist.
     *  Existing files are not overwritten.
     *
     * \param  name The basic name of the file.
     *
     * \return The response string from the device.
     */
    std::string setEISOutputFileName(std::string name);

    /** Measure EIS
     *
     *  For the measurement all parameters must be specified before.
     *
     * \return The response string from the device.
     */
    std::string measureEIS();



    /*
     * Section with settings for CV measurements.
     */

    /** Set the start potential of a CV measurment.
     *
     * \param  potential The start potential.
     *
     * \return The response string from the device.
     */
    std::string setCVStartPotential(double potential);

    /** Set the upper reversal potential of a CV measurment.
     *
     * \param  potential The upper reversal potential.
     *
     * \return The response string from the device.
     */
    std::string setCVUpperReversingPotential(double potential);

    /** Set the lower reversal potential of a CV measurment.
     *
     * \param  potential The lower reversal potential.
     *
     * \return The response string from the device.
     */
    std::string setCVLowerReversingPotential(double potential);

    /** Set the end potential of a CV measurment.
     *
     * \param  potential The end potential.
     *
     * \return The response string from the device.
     */
    std::string setCVEndPotential(double potential);

    /** Setting the holding time at the start potential.
     *
     *  The time must be given in seconds.
     *
     * \param  time The waiting time at start potential in s
     *
     * \return The response string from the device.
     */
    std::string setCVStartHoldTime(double time);

    /** Setting the holding time at the end potential.
     *
     *  The time must be given in seconds.
     *
     * \param  time The waiting time at end potential in s
     *
     * \return The response string from the device.
     */
    std::string setCVEndHoldTime(double time);

    /** Set the scan rate.
     *
     *  The scan rate must be specified in V/s.
     *
     * \param  scanRate The scan rate in V/s.
     *
     * \return The response string from the device.
     */
    std::string setCVScanRate(double scanRate);

    /** Set the number of cycles.
     *
     *  At least 0.5 cycles are necessary.
     *  The number of cycles must be a multiple of 0.5. 3.5 are also possible, for example.
     *
     * \param  cycles The number of CV cycles, at least 0.5.
     *
     * \return The response string from the device.
     */
    std::string setCVCycles(double cycles);

    /** Set the number of measurements per CV cycle.
     *
     * \param  samples The number of measurments per cycle.
     *
     * \return The response string from the device.
     */
    std::string setCVSamplesPerCycle(double samples);

    /** Set the maximum current.
     *
     *  The maximum positive current at which the measurement is interrupted.
     *
     * \param  current The maximum current for measurement in A.
     *
     * \return The response string from the device.
     */
    std::string setCVMaximumCurrent(double current);

    /** Set the minimum current.
     *
     *  The maximum negative current at which the measurement is interrupted.
     *
     * \param  current The minimum current for measurement in A.
     *
     * \return The response string from the device.
     */
    std::string setCVMinimumCurrent(double current);

    /** Set the ohmic drop for CV measurement.
     *
     *  The maximum negative current at which the measurement is interrupted.
     *
     * \param  ohmicDrop The ohmic drop for measurement.
     *
     * \return The response string from the device.
     */
    std::string setCVOhmicDrop(double ohmicDrop);

    /** Automatically restart if current is exceeded.
     *
     *  A new measurement is automatically started with a different
     *  reverse potential at which the current limit is not exceeded.
     *
     * \param  enabled If enabled = true the auto restart is enabled.
     *
     * \return The response string from the device.
     */
    std::string enableCVAutoRestartAtCurrentOverflow(bool enabled = true);

    /** Disable automatically restart if current is exceeded.
     *
     * \return The response string from the device.
     */
    std::string disableCVAutoRestartAtCurrentOverflow();

    /** Automatically restart if the current drops below the limit.
     *
     *  A new measurement is automatically started with a smaller
     *  current range than that determined by the minimum and maximum current.
     *
     * \param  enabled If enabled = true the auto restart is enabled.
     *
     * \return The response string from the device.
     */
    std::string enableCVAutoRestartAtCurrentUnderflow(bool enabled = true);

    /** Disable automatically restart if the current drops below the limit.
     *
     * \return The response string from the device.
     */
    std::string disableCVAutoRestartAtCurrentUnderflow();

    /** Switch on the analog function generator (AFG).
     *
     *  The analog function generator can only be used if it was purchased with the device.
     *  If the device has the AFG function, you will see a button in the CV software to activate this function.
     *
     * \param  enabled If enabled = true the AFG is switched on.
     *
     * \return The response string from the device.
     */
    std::string enableCVAnalogFunctionGenerator(bool enabled = true);

    /** Switch off the analog function generator (AFG).
     *
     * \return The response string from the device.
     */
    std::string disableCVAnalogFunctionGenerator();

    /** Set the measurement naming rule.
     *
     * \param  naming The measurement naming rule.
     *
     * \return The response string from the device.
     */
    std::string setCVNaming(NamingRule naming);

    /** Set the current number of measurement for filename.
     *
     *  Current number for the file name for measurements which is used next and then incremented.
     *
     * \param  number The next measurement number.
     *
     * \return The response string from the device.
     */
    std::string setCVCounter(int number);

    /** Set the path where the EIS measurements should be stored.
     *
     *  The directory must exist.
     *  The results must be stored on the C hard disk.
     *  If an error occurs test an alternative path or "C:\THALES\temp".
     *
     * \param  path The path to the directory.
     *
     * \return The response string from the device.
     */
    std::string setCVOutputPath(std::string path);

    /** Set the basic output filename.
     *
     *  The basic name of the file, which is extended by a sequential number or the date and time.
     *  Only numbers, underscores and letters from a-Z may be used.
     *  If the name is set to "individual", the file with the same name must not yet exist.
     *  Existing files are not overwritten.
     *
     * \param  name The basic name of the file.
     *
     * \return The response string from the device.
     */
    std::string setCVOutputFileName(std::string name);

    /** Check the set parameters.
     *
     *  With the error number the wrong parameter can be found.
     *  The error numbers are listed in the Remote2 manual.
     *
     * \return The response string from the device.
     */
    std::string checkCVSetup();

    /** Read the set parameters.
     *
     *  After checking with ThalesRemoteScriptWrapper::checkCVSetup the parameters can be read back from the workstation.
     *
     * \return The response string from the device.
     */
    std::string readCVSetup();

    /** Measure CV.
     *
     *  Before measurement, all parameters must be checked with ThalesRemoteScriptWrapper::checkCVSetup.
     *
     * \return The response string from the device.
     */
    std::string measureCV();


    /*
     * Section with settings for IE measurements.
     * Additional informations can be found in the IE manual.
     */

    /** Set the first edge potential.
     *
     * \param  potential The potential of the first edge in V.
     *
     * \return The response string from the device.
     */
    std::string setIEFirstEdgePotential(double potential);

    /** Set the fourth edge potential.
     *
     * \param  potential The potential of the fourth edge in V.
     *
     * \return The response string from the device.
     */
    std::string setIESecondEdgePotential(double potential);

    /** Set the first edge potential.
     *
     * \param  potential The potential of the first edge in V.
     *
     * \return The response string from the device.
     */
    std::string setIEThirdEdgePotential(double potential);

    /** Set the first edge potential.
     *
     * \param  potential The potential of the first edge in V.
     *
     * \return The response string from the device.
     */
    std::string setIEFourthEdgePotential(double potential);

    /** Set the relation of the first edge potential.
     *
     * \param  relation The relation of the edge potential absolute or relative.
     *
     * \return The response string from the device.
     */
    std::string setIEFirstEdgePotentialRelation(PotentialRelation relation);

    /** Set the relation of the second edge potential.
     *
     * \param  relation The relation of the edge potential absolute or relative.
     *
     * \return The response string from the device.
     */
    std::string setIESecondEdgePotentialRelation(PotentialRelation relation);

    /** Set the relation of the third edge potential.
     *
     * \param  relation The relation of the edge potential absolute or relative.
     *
     * \return The response string from the device.
     */
    std::string setIEThirdEdgePotentialRelation(PotentialRelation relation);

    /** Set the relation of the fourth edge potential.
     *
     * \param  relation The relation of the edge potential absolute or relative.
     *
     * \return The response string from the device.
     */
    std::string setIEFourthEdgePotentialRelation(PotentialRelation relation);

    /** Set the potential resolution.
     *
     *  The potential step size for IE measurements in V.
     *
     * \param  resolution The resolution for the measurement in V.
     *
     * \return The response string from the device.
     */
    std::string setIEPotentialResolution(double resolution);

    /** Set the minimum waiting time.
     *
     *  The minimum waiting time on each step of the IE measurement.
     *  This time is at least waited, even if the tolerance abort criteria are met.
     *
     * \param  time The waiting time in seconds.
     *
     * \return The response string from the device.
     */
    std::string setIEMinimumWaitingTime(double time);

    /** Set the maximum waiting time.
     *
     *  The maximum waiting time on each step of the IE measurement.
     *  After this time the measurement is stopped at this potential
     *  and continued with the next potential even if the tolerances are not reached.
     *
     * \param  time The waiting time in seconds.
     *
     * \return The response string from the device.
     */
    std::string setIEMaximumWaitingTime(double time);

    /** Set the relative tolerance criteria.
     *
     *  This parameter is only used in sweep mode steady state.
     *  The relative tolerance to wait in percent.
     *  The explanation can be found in the IE manual.
     *
     * \param  tolerance The tolerance to wait until break, 0.01 = 1%.
     *
     * \return The response string from the device.
     */
    std::string setIERelativeTolerance(double tolerance);

    /** Set the absolute tolerance criteria.
     *
     *  This parameter is only used in sweep mode steady state.
     *  The absolute tolerance to wait in A.
     *  The explanation can be found in the IE manual.
     *
     * \param  tolerance The tolerance to wait until break, 0.01 = 1%.
     *
     * \return The response string from the device.
     */
    std::string setIEAbsoluteTolerance(double tolerance);

    /** Set the ohmic drop for IE measurement.
     *
     * \param  ohmicDrop The ohmic drop for measurement.
     *
     * \return The response string from the device.
     */
    std::string setIEOhmicDrop(double ohmicDrop);

    /** Set the sweep mode.
     *
     *  The explanation of the modes can be found in the IE manual.
     *
     * \param  sweepMode The sweep mode for measurement.
     *
     * \return The response string from the device.
     */
    std::string setIESweepMode(IESweepMode sweepMode);

    /** Set the scan rate.
     *
     *  This parameter is only used in sweep mode dynamic scan.
     *  The scan rate must be specified in V/s.
     *
     * \param  scanRate The scan rate in V/s.
     *
     * \return The response string from the device.
     */
    std::string setIEScanRate(double scanRate);

    /** Set the maximum current.
     *
     *  The maximum positive current at which the measurement is interrupted.
     *
     * \param  current The maximum current for measurement in A.
     *
     * \return The response string from the device.
     */
    std::string setIEMaximumCurrent(double current);

    /** Set the minimum current.
     *
     *  The maximum negative current at which the measurement is interrupted.
     *
     * \param  current The minimum current for measurement in A.
     *
     * \return The response string from the device.
     */
    std::string setIEMinimumCurrent(double current);

    /** Set the measurement naming rule.
     *
     * \param  naming The measurement naming rule.
     *
     * \return The response string from the device.
     */
    std::string setIENaming(NamingRule naming);

    /** Set the current number of measurement for filename.
     *
     *  Current number for the file name for measurements which is used next and then incremented.
     *
     * \param  number The next measurement number.
     *
     * \return The response string from the device.
     */
    std::string setIECounter(int number);

    /** Set the path where the EIS measurements should be stored.
     *
     *  The directory must exist.
     *  The results must be stored on the C hard disk.
     *  If an error occurs test an alternative path or "C:\THALES\temp".
     *
     * \param  path The path to the directory.
     *
     * \return The response string from the device.
     */
    std::string setIEOutputPath(std::string path);

    /** Set the basic output filename.
     *
     *  The basic name of the file, which is extended by a sequential number or the date and time.
     *  Only numbers, underscores and letters from a-Z may be used.
     *  If the name is set to "individual", the file with the same name must not yet exist.
     *  Existing files are not overwritten.
     *
     * \param  name The basic name of the file.
     *
     * \return The response string from the device.
     */
    std::string setIEOutputFileName(std::string name);

    /** Check the set parameters.
     *
     *  With the error number the wrong parameter can be found.
     *  The error numbers are listed in the Remote2 manual.
     *
     * \return The response string from the device.
     */
    std::string checkIESetup();

    /** Read the set parameters.
     *
     *  After checking with ThalesRemoteScriptWrapper::checkIESetup the parameters can be read back from the workstation.
     *
     * \return The response string from the device.
     */
    std::string readIESetup();

    /** Measure IE.
    *
    *  Before measurement, all parameters must be checked with ThalesRemoteScriptWrapper::checkIESetup.
    *
    * \return The response string from the device.
    */
    std::string measureIE();

    /*
     * Section of remote functions for the sequencer.
     *
     * With the sequencer DC profiles can be described textually.
     * For instructions on how the sequencer file is structured, please refer to the manual of the sequencer.
     */

    /** Select the sequence to run with ThalesRemoteScriptWrapper::runSequence.
     *
     *  The sequences must be stored under "C:\THALES\script\sequencer\sequences".
     *  Sequences from 0 to 9 can be created.
     *  These must have the names from "sequence00.seq" to "sequence09.seq".
     *
     *
     * \param  number The number of the sequence.
     *
     * \return The response string from the device.
     */
    std::string selectSequence(int number);

    /** Set the measurement naming rule.
     *
     * \param  naming The measurement naming rule.
     *
     * \return The response string from the device.
     */
    std::string setSequenceNaming(NamingRule naming);

    /** Set the current number of measurement for filename.
     *
     *  Current number for the file name for measurements which is used next and then incremented.
     *
     * \param  number The next measurement number.
     *
     * \return The response string from the device.
     */
    std::string setSequenceCounter(int number);

    /** Set the path where the EIS measurements should be stored.
     *
     *  The directory must exist.
     *  The results must be stored on the C hard disk.
     *  If an error occurs test an alternative path or "C:\THALES\temp".
     *
     * \param  path The path to the directory.
     *
     * \return The response string from the device.
     */
    std::string setSequenceOutputPath(std::string path);

    /** Set the basic output filename.
     *
     *  The basic name of the file, which is extended by a sequential number or the date and time.
     *  Only numbers, underscores and letters from a-Z may be used.
     *  If the name is set to "individual", the file with the same name must not yet exist.
     *  Existing files are not overwritten.
     *
     * \param  name The basic name of the file.
     *
     * \return The response string from the device.
     */
    std::string setSequenceOutputFileName(std::string name);

    /** Switch on that up to 8 ACQ channels are used in the sequencer.
     *
     * \param state default true to switch on.
     * \return The response string from the device.
     */
    std::string enableSequenceAcqGlobal(bool state = true);

    /** Switch off that up to 8 ACQ channels are used in the sequencer.
     *
     * \return The response string from the device.
     */
    std::string disableSequenceAcqGlobal();

    /** Switch on the set sequence ACQ channel
     *
     * \param  channel ACQ channel index.
     * \param state default true to switch on.
     * \return The response string from the device.
     */
    std::string enableSequenceAcqChannel(int channel, bool state = true);

    /** Switch off the set sequence ACQ channel
     *
     * \param  channel channel index
     * \return The response string from the device.
     */
    std::string disableSequenceAcqChannel(int channel);

    /** Read the currently set sequencer ACQ Setup.
     *
     * \return The response string from the device.
     */
    std::string readSequenceAcqSetup();

    /** Run the selected sequence.
     *
     *  This command executes the selected sequence between 0 and 9.
     *
     * \return The response string from the device.
     */
    std::string runSequence();

    /** Set the Ohmic Drop or IR Drop for the sequencer.
     *
     *  0 to 1 tera ohm.
     *
     *  Parameter name in sequence file: ``rodrop``
     *
     * \param value Ohmic Drop in Ohm.
     *
     * \return The response string from the device.
     */
    std::string setSequenceOhmicDrop(double value);

    /** Set the maximum runtime in hours.
     *
     *  0.1 to 1000 hours.
     *
     *  Parameter name in sequence file: ``max_ti``
     *
     * \param value maximum runtime in hours.
     *
     * \return The response string from the device.
     */
    std::string setSequenceMaximumRuntime(double value);

    /** Set the lower potential limit.
     *
     *  Parameter name in sequence file: ``pot_hi``
     *
     * \param value potential limit.
     *
     * \return The response string from the device.
     */
    std::string setSequenceUpperPotentialLimit(double value);

    /** Set the lower potential limit.
     *
     *  Parameter name in sequence file: ``pot_lo``
     *
     * \param value potential limit.
     *
     * \return The response string from the device.
     */
    std::string setSequenceLowerPotentialLimit(double value);

    /** Set the upper current limit.
     *
     *  Parameter name in sequence file: ``cur_hi``
     *
     * \param value potential limit.
     *
     * \return The response string from the device.
     */
    std::string setSequenceUpperCurrentLimit(double value);

    /** Set the lower current limit.
     *
     *  Parameter name in sequence file: ``cur_lo``
     *
     * \param value potential limit.
     *
     * \return The response string from the device.
     */
    std::string setSequenceLowerCurrentLimit(double value);

    /** Set the current range.
     *
     *  Parameter name in sequence file: ``cur_ra``
     *
     * \param value current range in ampere.
     *
     * \return The response string from the device.
     */
    std::string setSequenceCurrentRange(double value);

    /** Set the potential latency window..
     *
     *  This defines what happens when a limit value is exceeded.
     *  The following values are possible:
     *
     *  1: potentiostat is not switched off
     *  t: potentiostat is switched off after t seconds, maximum 30
     *  2: potentiostat is switched off after 2 seconds
     *  0: potentiostat is switched off immediately
     *
     *  Parameter name in sequence file: ``pot_of``
     *
     *
     * \param value values meaning see in the text.
     *
     * \return The response string from the device.
     */
    std::string setSequencePotentialLatencyWindow(double value);

    /** Set the current latency window..
     *
     *  This defines what happens when a limit value is exceeded.
     *  The following values are possible:
     *
     *  1: potentiostat is not switched off
     *  t: potentiostat is switched off after t seconds, maximum 30
     *  2: potentiostat is switched off after 2 seconds
     *  0: potentiostat is switched off immediately
     *
     *  Parameter name in sequence file: ``cur_of``
     *
     *
     * \param value values meaning see in the text.
     *
     * \return The response string from the device.
     */
    std::string setSequenceCurrentLatencyWindow(double value);

    /** Enables the use of the FRA probe.
     *
     *  With the FRA Probe, external power potentiostats, signal generators, sources, sinks can be
     *  controlled analog for impedance measurements.
     *
     *  Product Page: https://zahner.de/products-details/probes/fra-probe
     *  Manual: https://doc.zahner.de/hardware/fra_probe.pdf
     *
     *  Before this function is called, the analog interface to the external interface must be initialized
     *  with the correct factors. It may be necessary to use + or - as sign, this must be tested to ensure
     *  that potentiostatic and galvanostatic function correctly.
     *
     *  For example, if the device has 100 A output current and the analog signal input range of the
     *  device is 5 V, then the current gain is 100/5.
     *
     * \param  enabled If state = true FRA mode is enabled.
     *
     * \return The response string from the device.
     */
    std::string enableFraMode(bool enabled = true);
    std::string disableFraMode();

    /** Sets the input voltage gain.
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraVoltageInputGain(double value);

    /** Sets the output voltage gain.
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraVoltageOutputGain(double value);

    /** Sets the minimum voltage.
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraVoltageMinimum(double value);

    /** Sets the maximum voltage.
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraVoltageMaximum(double value);

    /** Sets the input current gain
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraCurrentInputGain(double value);

    /** Sets the output current gain
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraCurrentOutputGain(double value);

    /** Sets the minimum current.
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraCurrentMinimum(double value);

    /** Sets the maximum current.
     *
     * \param  value The value to set.
     *
     * \return The response string from the device.
     */
    std::string setFraCurrentMaximum(double value);

    /** Set the coupling of the FRA mode.
     *
     * \param  potentiostatMode The coupling of the FRA mode.
     *
     * \return The response string from the device.
     */
    std::string setFraPotentiostatMode(PotentiostatMode potentiostatMode);

    /** Read the currently set FRA parameters.
     *
     *  Reading the set FRA configuration.
     *  A string containing the configuration is returned.
     *
     * \return The response string from the device.
     */
    std::string readFraSetup();

    /*
     * Section with methods for the ACQ channels
     */

    /** Read the currently set FRA parameters.
     *
     *  Reading the set FRA configuration.
     *  A string containing the configuration is returned.
     *
     * \return The response string from the device.
     */
    std::string readAcqSetup();

    /** Read all active ACQ channels.
     *
     * A string is read from Thales which contains the ACQ channels as follows:
     * "ACQVAL(0)= 2.632052e-01;ACQVAL(1)= 8.413594e-02;ACQVAL(2)= 5.338292e+01"
     *
     * \return The response string from the device.
     */
    std::string readAllAcqChannels();

    /** Read the ACQ channel to the passed index.
     *
     * @param channel Display channel index.
     * @return
     */
    double readAcqChannel(int channel);


protected:


    /** Set an Remote2 parameter or value.
     *
     *  With this command the parameters are transmitted to the Thales Remote2 and the response is read.
     *
     * \param  name Name of the Remote2 parameter.
     * \param  relation The value of the specified type.
     *
     * \return The response string from the device.
     */
    std::string setValue(std::string name, PotentialRelation relation);

    /** Set an Remote2 parameter or value.
     *
     *  With this command the parameters are transmitted to the Thales Remote2 and the response is read.
     *
     * \param  name Name of the Remote2 parameter.
     * \param  value The value of the specified type.
     *
     * \return The response string from the device.
     */
    std::string setValue(std::string name, bool value);

    /** Set an Remote2 parameter or value.
     *
     *  With this command the parameters are transmitted to the Thales Remote2 and the response is read.
     *
     * \param  name Name of the Remote2 parameter.
     * \param  value The value of the specified type.
     *
     * \return The response string from the device.
     */
    std::string setValue(std::string name, double value);

    /** Set an Remote2 parameter or value.
     *
     *  With this command the parameters are transmitted to the Thales Remote2 and the response is read.
     *
     * \param  name Name of the Remote2 parameter.
     * \param  value The value of the specified type.
     *
     * \return The response string from the device.
     */
    std::string setValue(std::string name, int value);

    /** Set an Remote2 parameter or value.
     *
     *  With this command the parameters are transmitted to the Thales Remote2 and the response is read.
     *
     * \param  name Name of the Remote2 parameter.
     * \param  value The value of the specified type.
     *
     * \return The response string from the device.
     */
    std::string setValue(std::string name, std::string value);

    /** Sending a Remote2 command and parsing a double from the response.
     *
     * \param  command Name of the Remote2 command.
     * \param  pattern The regex to extract the value from the response string.
     *
     * \return The received value.
     */
    double requestValueAndParseUsingRegexp(std::string command, std::regex pattern);

    /** Converts a string to double.
     *
     * This needed to be added because the numberical strings delivered
     * by Thales could not be parsed by std::stod in some cases.
     *
     * \return the value which was previously coded as string.
     */
    double stringToDobule(std::string string);


    /** Converts a string to int.
     *
     * This needed to be added because the numberical strings delivered
     * by Thales could not be parsed by std::stod in some cases.
     *
     * \return the value which was previously coded as string.
     */
    int stringToInt(std::string string);

    ZenniumConnection * const remoteConnection;
};

#endif // THALESREMOTESCRIPTWRAPPER_H
