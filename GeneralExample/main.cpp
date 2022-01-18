#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"


void spectrum(ThalesRemoteScriptWrapper &scriptHandle, double lower_frequency, double upper_frequency, int number_of_points);
void printImpedance(std::complex<double> impedance);

int main(int argc, char *argv[]) {

    ZenniumConnection ZenniumConnection;

    bool connectionSuccessful = ZenniumConnection.connectToTerm("localhost", "ScriptRemote");

    if (connectionSuccessful == false) {

        std::cout << "could not connect to Term" << std::endl;
        return 0;
    }

    ThalesRemoteScriptWrapper ZahnerZennium(&ZenniumConnection);

    ZahnerZennium.forceThalesIntoRemoteScript();

    ZahnerZennium.calibrateOffsets();

    ZahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
    ZahnerZennium.setPotential(1.0);
    ZahnerZennium.enablePotentiostat();

    for (int i = 0; i < 5 ; i ++) {
        std::cout << ZahnerZennium.getPotential() << std::endl;
        std::cout << ZahnerZennium.getCurrent() << std::endl;
    }

    ZahnerZennium.disablePotentiostat();
    ZahnerZennium.setPotentiostatMode(PotentiostatMode::GALVANOSTATIC);
    ZahnerZennium.setCurrent(20e-9);
    ZahnerZennium.enablePotentiostat();

    for (int i = 0; i < 5 ; i ++) {
        std::cout << ZahnerZennium.getPotential() << std::endl;
        std::cout << ZahnerZennium.getCurrent() << std::endl;
    }

    ZahnerZennium.disablePotentiostat();
    ZahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
    ZahnerZennium.setPotential(1.0);
    ZahnerZennium.enablePotentiostat();
    ZahnerZennium.setFrequency(2000);
    ZahnerZennium.setAmplitude(10e-3);
    ZahnerZennium.setNumberOfPeriods(3);

    printImpedance(ZahnerZennium.getImpedance());
    printImpedance(ZahnerZennium.getImpedance(2000));
    printImpedance(ZahnerZennium.getImpedance(2000, 10e-3, 3));

    spectrum(ZahnerZennium, 1000, 2e5, 10);

    ZahnerZennium.disablePotentiostat();
    ZahnerZennium.setAmplitude(0);

    ZenniumConnection.disconnectFromTerm();

    std::cout << "finish" << std::endl;
    return 0;
}

void spectrum(ThalesRemoteScriptWrapper &scriptHandle, double lower_frequency, double upper_frequency, int number_of_points) {

    double log_lower_frequency = std::log(lower_frequency);
    double log_upper_frequency = std::log(upper_frequency);

    double log_interval_spacing = (log_upper_frequency - log_lower_frequency) / static_cast<double>(number_of_points - 1);

    for (int i = 0; i < number_of_points; i++) {

        // calculating logarithmic equidistant frequencies
        double current_frequency = std::exp(log_lower_frequency + log_interval_spacing * static_cast<double>(i));

        std::cout << "Frequency " << current_frequency << std::endl;
        printImpedance(scriptHandle.getImpedance(current_frequency));
    }
}

void printImpedance(std::complex<double> impedance) {

    std::cout << std::abs(impedance) << " ohm, " << std::arg(impedance) << " rad" << std::endl;
}
