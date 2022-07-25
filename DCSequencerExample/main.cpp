#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"
#include "thalesremoteerror.h"

int main(int argc, char *argv[]) {

    ZenniumConnection zenniumConnection;

    bool connectionSuccessful = zenniumConnection.connectToTerm("localhost", "ScriptRemote");

    if (not connectionSuccessful) {

        std::cout << "could not connect to Term" << std::endl;
        return 0;
    }

    ThalesRemoteScriptWrapper zahnerZennium(&zenniumConnection);

    zahnerZennium.forceThalesIntoRemoteScript();

    zahnerZennium.calibrateOffsets();

    zahnerZennium.setSequenceNaming(NamingRule::DATETIME);
    zahnerZennium.setSequenceOutputPath("c:\\THALES\\temp\\test1");
    zahnerZennium.setSequenceOutputFileName("batterysequence");

    zahnerZennium.selectSequence(0);

    zahnerZennium.runSequence();

    zenniumConnection.disconnectFromTerm();

    std::cout << "finish" << std::endl;
    return 0;
}
