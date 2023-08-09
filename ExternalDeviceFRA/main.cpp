#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"

int main(int argc, char *argv[]) {

    ZenniumConnection ZenniumConnection;

    ZenniumConnection.connectToTerm("localhost", "ScriptRemote");

    ThalesRemoteScriptWrapper zahnerZennium(&ZenniumConnection);

    zahnerZennium.forceThalesIntoRemoteScript();


    /*
     * Settings on the FRA mode are only allowed to be changed if the FRA mode is deactivated.
     */
    zahnerZennium.disableFraMode();

    zahnerZennium.setFraVoltageMinimum(0);
    zahnerZennium.setFraVoltageMaximum(18);
    zahnerZennium.setFraCurrentMinimum(0);
    zahnerZennium.setFraCurrentMaximum(220);

    zahnerZennium.setFraVoltageInputGain(18.0/5.0);
    zahnerZennium.setFraVoltageOutputGain(18.0/5.0);
    zahnerZennium.setFraCurrentInputGain(-220.0/5.0);
    zahnerZennium.setFraCurrentOutputGain(-220.0/5.0);
    zahnerZennium.setFraPotentiostatMode(PotentiostatMode::GALVANOSTATIC);

    zahnerZennium.enableFraMode();
    zahnerZennium.setCurrent(0);

    /*
     * Now the device which is analog controlled by FRA should be switched on with its own interface.
     * Then you can measure EIS via FRA like with the internal potentiostat.
     */
    for (double i = 1; i < 8 ; i += 1) {
        zahnerZennium.setCurrent(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << zahnerZennium.getPotential() << std::endl;
        std::cout << zahnerZennium.getCurrent() << std::endl;
    }

    zahnerZennium.setEISNaming(NamingRule::COUNTER);
    zahnerZennium.setEISCounter(1);
    zahnerZennium.setEISOutputPath("c:\\thales\\temp\\test1");
    zahnerZennium.setEISOutputFileName("delta");

    zahnerZennium.setAmplitude(0.5);
    zahnerZennium.setCurrent(5);
    zahnerZennium.setLowerFrequencyLimit(0.1);
    zahnerZennium.setStartFrequency(10);
    zahnerZennium.setUpperFrequencyLimit(100);
    zahnerZennium.setLowerNumberOfPeriods(3);
    zahnerZennium.setLowerStepsPerDecade(5);
    zahnerZennium.setUpperNumberOfPeriods(20);
    zahnerZennium.setUpperStepsPerDecade(5);
    zahnerZennium.setScanDirection(ScanDirection::START_TO_MAX);
    zahnerZennium.setScanStrategy(ScanStrategy::SINGLE_SINE);

    zahnerZennium.measureEIS();

    /*
     * Now switch off the device, which is controlled analog with FRA via its own interface.
     */
    zahnerZennium.disableFraMode();

    ZenniumConnection.disconnectFromTerm();

    std::cout << "finish" << std::endl;
    return 0;
}
