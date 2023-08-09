#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"
#include "thalesremoteerror.h"

int main(int argc, char *argv[]) {

    ZenniumConnection ZenniumConnection;

    ZenniumConnection.connectToTerm("192.168.2.94", "ScriptRemote");

    ThalesRemoteScriptWrapper zahnerZennium(&ZenniumConnection);

    zahnerZennium.forceThalesIntoRemoteScript();

    /*
     * Measure EIS spectra with a sequential number in the file name that has been specified.
     * Starting with number 1.
     */
    zahnerZennium.setEISNaming(NamingRule::COUNTER);
    zahnerZennium.setEISCounter(1);
    zahnerZennium.setEISOutputPath("c:\\thales\\temp\\test1");
    zahnerZennium.setEISOutputFileName("spectra_cells");

    /*
     * Setting the parameters for the spectra.
     * Alternatively a rule file can be used as a template.
     */
    zahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
    zahnerZennium.setAmplitude(50e-3);
    zahnerZennium.setPotential(0);
    zahnerZennium.setLowerFrequencyLimit(100);
    zahnerZennium.setStartFrequency(1000);
    zahnerZennium.setUpperFrequencyLimit(10000);
    zahnerZennium.setLowerNumberOfPeriods(3);
    zahnerZennium.setLowerStepsPerDecade(5);
    zahnerZennium.setUpperNumberOfPeriods(20);
    zahnerZennium.setUpperStepsPerDecade(5);
    zahnerZennium.setScanDirection(ScanDirection::START_TO_MAX);
    zahnerZennium.setScanStrategy(ScanStrategy::SINGLE_SINE);

    /*
     * Setup PAD4 Channels.
     * The PAD4 setup is encapsulated with try and catch to catch the exception if no PAD4 card is present.
     */
    try {
        zahnerZennium.setupPad4ChannelWithVoltageRange(1,1,true,4.0);
        zahnerZennium.setupPad4ChannelWithVoltageRange(1,2,true,4.0);
        zahnerZennium.setupPad4ModeGlobal(Pad4Mode::VOLTAGE);
        zahnerZennium.enablePad4Global();
    }  catch (ThalesRemoteError error) {
        std::cout << "PAD4 not available:" << std::endl;
        std::cout << error.getMessage() << std::endl;
    }


    /*
     * Switching on the potentiostat before the measurement,
     * so that EIS is measured at the set DC potential.
     * If the potentiostat is off before the measurement,
     * the measurement is performed at the OCP.
     */
    zahnerZennium.enablePotentiostat();
    zahnerZennium.measureEIS();
    zahnerZennium.disablePotentiostat();
    ZenniumConnection.disconnectFromTerm();

    std::cout << "finish" << std::endl;
    return 0;
}
