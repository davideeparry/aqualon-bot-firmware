#include "Database.h"
#include "../comms/Communications.h"

void Database::init()
{
    if (!SD.begin(chipSelect))
    { 
        Communications::instance().sendError("SD Initialization Failed");
        initFailed = true;
        return; // a callback would be good here
    }
};

void Database::writeToDataLog(String msg)
{
    dataLog = SD.open(dataLogId, FILE_WRITE); // needs callback
    dataLog.println(msg);
    dataLog.close();
};

void Database::writeToSystemLog(String msg)
{
    systemLog = SD.open(systemLogId, FILE_WRITE); // needs callback
    systemLog.println(msg);
    systemLog.close();
};

String Database::dumpDataLogs(int chunkSize)
{ 
    String dataDump = "";
    if (transferInProgress == false) {
        dataLog = SD.open(dataLogId);
    }
     // callback
    while (dataLog.available())
    {
        char tmp = dataLog.read();
        dataDump.concat(tmp);
        if (dataDump.length() >= chunkSize) {
            transferInProgress = true;
            return dataDump;
        }
    }
    transferInProgress = false;
    dataLog.close();
    return dataDump;
    // This needs a limit so that it doesn't dump too much memory and will partition it. 
};

String Database::dumpSystemLogs()
{ // WILL WANT TO MAKE THIS ONLY DUMP AS MUCH AS MEMORY WILL ALLOW, AND IT SHOULD BE CALLED MULTIPLE TIMES BY THE CALLER
    String systemDump;
    systemLog = SD.open(dataLogId);

    while (systemLog.available())
    {
        systemDump.append(systemLog.read());
       /* if (systemDump.length() >= 10) {
            systemLog.close();
            return systemDump;
        } */
    }
    systemLog.close();
    return systemDump;
};

void Database::clearSystemLog()
{
    SD.remove(systemLogId);
};

void Database::clearDatalog()
{
    SD.remove(dataLogId);
};
