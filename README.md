# Thales-Remote-Cpp
Thales-Remote-Cpp is a C++ extension which uses the Zahner [Remote2](https://doc.zahner.de/Remote.pdf) to control [Zahner Zennium Potentiostats](https://zahner.de/products#potentiostats).  
It was developed to **easily integrate** Zahner Zennium Potentiostats into Python scripts for more **complex measurement** tasks and for **automation purposes**.

The measurement methods **EIS**, **IE**, **CV** and **DC sequences** are supported. Also constant current or constant voltage can be output and current and voltage can be measured. Single frequency impedance measurement is also possible.

In the GitHub repository [Thales-Remote-Python](https://github.com/Zahner-elektrik/Thales-Remote-Python) are more detailed examples which can be easily implemented in C++.  
The setter and getter in C++ and Python are always named the same and behave the same.

# 📚 Documentation

The complete documentation of the individual functions can be found on the [API documentation website](https://doc.zahner.de/thales_remote_cpp/).  

# 🔧 Installation
Download the folder ThalesRemoteCppLibrary and compile the library into your project.  
The examples were created with cmake, this can be used as a template.

# 🔨 Basic Usage

```cpp
/*
 * Connect to the Zahner Zennium Potentiostat
 */
ZenniumConnection ThalesRemoteConnection;
ThalesRemoteConnection.connectToTerm("localhost", "ScriptRemote");
ThalesRemoteScriptWrapper zahnerZennium(&ThalesRemoteConnection);
zahnerZennium.forceThalesIntoRemoteScript();

/*
 * Read the measured voltage and current.
 */
 std::cout << "Potential " << zahnerZennium.getPotential() << std::endl;
 std::cout << "Current " << zahnerZennium.getCurrent()   << std::endl;

/*
 * Single frequency impedance measurement at 1 V DC and 2 kHz
 * with 10mV amplitude for 3 periods.
 */
zahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
zahnerZennium.setPotential(0);
    
zahnerZennium.setFrequency(2000);
zahnerZennium.setAmplitude(10e-3);
zahnerZennium.setNumberOfPeriods(3);

zahnerZennium.enablePotentiostat();
zahnerZennium.getImpedance();
zahnerZennium.disablePotentiostat();
```

# 📖 Examples
There are several examples available on different topics.  
In the GitHub repository [Thales-Remote-Python](https://github.com/Zahner-elektrik/Thales-Remote-Python) are more detailed examples which can be easily implemented in C++.  
The setter and getter in C++ and Python are always named the same and behave the same.

### [GeneralExample](GeneralExample/main.cpp)

* Switch potentiostat on or off
* Setting potentiostat potentiostatic or galvanostatic
* Setting output potential or current
* Read potential and current
* Measure impedance

### [EisPad4Example](EisPad4Example/main.cpp)

* Switch potentiostat on or off
* Setting potentiostat potentiostatic or galvanostatic
* Measurement of an impedance spectrum (EIS) on a stack with single cells connected to the [PAD4](https://zahner.de/products-details/addon-cards/pad4) card.
* Configuration of the [PAD4](https://zahner.de/products-details/addon-cards/pad4) card

### [FileExchangeExample](FileExchangeExample/main.cpp)

* Measurement of an impedance spectrum
* Monitor activity with the HeartBeat
* **Acquiring the measurement files with C++ via network**

### [ExternalDeviceFRA](ExternalDeviceFRA/main.cpp)

* Configure FRA Probe measurement
* Measure EIS with FRA Probe

### [EisDLLExample](EisDLLExample/main.cpp)

* Switch potentiostat on or off
* Setting potentiostat potentiostatic or galvanostatic
* Setting output potential or current
* Read potential and current
* Measure impedance

### [EisDLLExample](EisDLLExample/main.cpp)

* Switch potentiostat on or off
* Setting potentiostat potentiostatic or galvanostatic
* Setting output potential or current
* Read potential and current
* Measure impedance

### [DCSequencerExample](DCSequencerExample/main.cpp)

* The [Zahner sequencer](https://zahner.de/media-files/support_downloads/files/sequencer_1.pdf) outputs current and voltage curves defined in a text file.
* Setting output file naming for sequence measurements
* Parametrizing an sequence measurement

This example uses a DLL which was created from the library. The DLL is loaded from the C++ code in the example with WinAPI at runtime. But in C++ the library itself should be used this is easier.
The DLL and the source and header files of the DLL generated.cpp and generated.h are located in the subfolder [ThalesRemoteExternalLibrary](ThalesRemoteExternalLibrary).
The DLL is built with CMAKE and MinGW and does not contain any debug information. The repository contains all files to generate the DLL from the generated.cpp and generated.h files.


# 📧 Haveing a question?
Send an <a href="mailto:support@zahner.de?subject=Thales-Remote-Python Question&body=Your Message">e-mail</a> to our support team.

# ⁉️ Found a bug or missing a specific feature?
Feel free to **create a new issue** with a respective title and description on the the [Thales-Remote-Cpp](https://github.com/Zahner-elektrik/Thales-Remote-Cpp/issues) repository. If you already found a solution to your problem, **we would love to review your pull request**!

# ✅ Requirements
The library was developed and tested with C++20 without additional libraries.
