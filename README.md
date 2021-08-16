# Thales-Remote-Cpp
Thales-Remote-Python is a Python extension which uses the Zahner [Remote2](http://zahner.de/pdf/Remote2.pdf) to control [Zahner Zennium Potentiostats](http://zahner.de/products/electrochemical-workstation.html).  
It was developed to **easily integrate** Zahner Zennium Potentiostats into Python scripts for more **complex measurement** tasks and for **automation purposes**.

The measurement methods **EIS**, **IE**, **CV** and **DC sequences** are supported. Also constant current or constant voltage can be output and current and voltage can be measured. Single frequency impedance measurement is also possible.

Additional detailed documentation of the individual commands can be found in the python modules.  
The complete documentation of the individual functions can be found on the [API documentation website](https://doc.zahner.de/thales_remote_cpp/). 

In the GitHub repository [Thales-Remote-Python](https://github.com/Zahner-elektrik/Thales-Remote-Python) are more detailed examples which can be easily implemented in C++.  
The setter and getter in C++ and Python are always named the same and behave the same.

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
ThalesRemoteScriptWrapper ZahnerZennium(&ThalesRemoteConnection);
ZahnerZennium.forceThalesIntoRemoteScript();

/*
 * Read the measured voltage and current.
 */
 std::cout << "Potential " << ZahnerZennium.getPotential() << std::endl;
 std::cout << "Current " << ZahnerZennium.getCurrent()   << std::endl;

/*
 * Single frequency impedance measurement at 1 V DC and 2 kHz
 * with 10mV amplitude for 3 periods.
 */
ZahnerZennium.setPotentiostatMode(PotentiostatMode::POTENTIOSTATIC);
ZahnerZennium.setPotential(0);
    
ZahnerZennium.setFrequency(2000);
ZahnerZennium.setAmplitude(10e-3);
ZahnerZennium.setNumberOfPeriods(3);

ZahnerZennium.enablePotentiostat();
ZahnerZennium.getImpedance();
ZahnerZennium.disablePotentiostat();
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
* Measurement of an impedance spectrum (EIS) on a stack with single cells connected to the [PAD4](http://zahner.de/products/addon-cards/pad4.html) card.
* Configuration of the [PAD4](http://zahner.de/products/addon-cards/pad4.html) card

# 📧 Haveing a question?
Send an <a href="mailto:support@zahner.de?subject=Thales-Remote-Python Question&body=Your Message">e-mail</a> to our support team.

# ⁉️ Found a bug or missing a specific feature?
Feel free to **create a new issue** with a respective title and description on the the [Thales-Remote-Cpp](https://github.com/Zahner-elektrik/Thales-Remote-Cpp/issues) repository. If you already found a solution to your problem, **we would love to review your pull request**!

# ✅ Requirements
The library was developed and tested with C++17 without additional libraries.
