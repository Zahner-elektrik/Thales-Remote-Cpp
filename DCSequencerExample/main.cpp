#include "thalesremoteconnection.h"
#include "thalesremotescriptwrapper.h"
#include "thalesremoteerror.h"

int main(int argc, char *argv[]) {

    ZenniumConnection zenniumConnection;

    zenniumConnection.connectToTerm("localhost", "ScriptRemote");

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
