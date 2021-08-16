#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"
#include "thalesremoteerror.h"

int main(int argc, char *argv[]) {

    ZenniumConnection ZenniumConnection;

    bool connectionSuccessful = ZenniumConnection.connectToTerm("localhost", "ScriptRemote");

    if (connectionSuccessful == false) {

        std::cout << "could not connect to Term" << std::endl;
        return 0;
    }

    ThalesRemoteScriptWrapper ZahnerZennium(&ZenniumConnection);

    ZahnerZennium.forceThalesIntoRemoteScript();

    /*
     * Measure EIS spectra with a sequential number in the file name that has been specified.
     * Starting with number 1.
     */
    ZahnerZennium.setEISNaming(NamingRule::COUNTER);
    ZahnerZennium.setEISCounter(1);
    ZahnerZennium.setEISOutputPath("c:\\thales\\temp\\test1");
    ZahnerZennium.setEISOutputFileName("spectra_cells");

    /*
     * Setting the parameters for the spectra.
     * Alternatively a rule file can be used as a template.
     */
    ZahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
    ZahnerZennium.setAmplitude(50e-3);
    ZahnerZennium.setPotential(0);
    ZahnerZennium.setLowerFrequencyLimit(100);
    ZahnerZennium.setStartFrequency(1000);
    ZahnerZennium.setUpperFrequencyLimit(10000);
    ZahnerZennium.setLowerNumberOfPeriods(3);
    ZahnerZennium.setLowerStepsPerDecade(5);
    ZahnerZennium.setUpperNumberOfPeriods(20);
    ZahnerZennium.setUpperStepsPerDecade(5);
    ZahnerZennium.setScanDirection(ScanDirection::START_TO_MAX);
    ZahnerZennium.setScanStrategy(ScanStrategy::SINGLE_SINE);

    /*
     * Setup PAD4 Channels.
     * The PAD4 setup is encapsulated with try and catch to catch the exception if no PAD4 card is present.
     */
    try {
        ZahnerZennium.setupPAD4(1,1,true);
        ZahnerZennium.setupPAD4(1,2,true);
        ZahnerZennium.enablePAD4();
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
    ZahnerZennium.enablePotentiostat();
    ZahnerZennium.measureEIS();
    ZahnerZennium.disablePotentiostat();
    ZenniumConnection.disconnectFromTerm();

    std::cout << "finish" << std::endl;
    return 0;
}
