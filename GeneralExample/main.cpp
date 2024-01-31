#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"


void spectrum(ThalesRemoteScriptWrapper &scriptHandle, double lower_frequency, double upper_frequency, int number_of_points);
void printImpedance(std::complex<double> impedance);

int main(int argc, char *argv[]) {

    ZenniumConnection ZenniumConnection;

    ZenniumConnection.connectToTerm("127.0.0.1", "ScriptRemote");
    ThalesRemoteScriptWrapper zahnerZennium(&ZenniumConnection);

    zahnerZennium.forceThalesIntoRemoteScript();

    zahnerZennium.calibrateOffsets();

    zahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
    zahnerZennium.setPotential(1.0);
    zahnerZennium.enablePotentiostat();

    for (int i = 0; i < 5 ; i ++) {
        std::cout << zahnerZennium.getPotential() << std::endl;
        std::cout << zahnerZennium.getCurrent() << std::endl;
    }

    zahnerZennium.disablePotentiostat();
    zahnerZennium.setPotentiostatMode(PotentiostatMode::GALVANOSTATIC);
    zahnerZennium.setCurrent(20e-9);
    zahnerZennium.enablePotentiostat();

    for (int i = 0; i < 5 ; i ++) {
        std::cout << zahnerZennium.getPotential() << std::endl;
        std::cout << zahnerZennium.getCurrent() << std::endl;
    }

    zahnerZennium.disablePotentiostat();
    zahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
    zahnerZennium.setPotential(1.0);
    zahnerZennium.enablePotentiostat();
    zahnerZennium.setFrequency(2000);
    zahnerZennium.setAmplitude(10e-3);
    zahnerZennium.setNumberOfPeriods(3);

    printImpedance(zahnerZennium.getImpedance());
    printImpedance(zahnerZennium.getImpedance(2000));
    printImpedance(zahnerZennium.getImpedance(2000, 10e-3, 3));

    spectrum(zahnerZennium, 1000, 2e5, 10);

    zahnerZennium.disablePotentiostat();
    zahnerZennium.setAmplitude(0);

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
