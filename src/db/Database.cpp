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

void Database::writeSingleDataToFile(String filename, String data)
{
    SD.remove(filename.getBytes);
    File tmpLog = SD.open(filename.getBytes, FILE_WRITE); // needs callback
    tmpLog.print(data);
    tmpLog.close();
};

String Database::getSingleDataFromFile(String filename)
{
    String dataDump = "";
    if (SD.exists(filename.c_str()))
    {
        File tmpLog = SD.open(filename.c_str()); // callback

        while (tmpLog.available())
        {
            char tmp = tmpLog.read();
            dataDump.concat(tmp);
        }
        tmpLog.close();
        return dataDump;
    } 
    else 
    {
        return "0";
    }
};

void Database::writeToSystemLog(String msg)
{
    systemLog = SD.open(systemLogId, FILE_WRITE); // needs callback
    systemLog.println(msg);
    systemLog.close();
};

String Database::dumpDataLogs()
{
    String dataDump = "";
    dataLog = SD.open(dataLogId); // callback
    while (dataLog.available())
    {
        char tmp = dataLog.read();
        dataDump.concat(tmp);
    }
    dataLog.close();
    return dataDump;
    // This needs a limit so that it doesn't dump too much memory and will partition it.
};

String Database::dumpSystemLogs()
{ // WILL WANT TO MAKE THIS ONLY DUMP AS MUCH AS MEMORY WILL ALLOW, AND IT SHOULD BE CALLED MULTIPLE TIMES BY THE CALLER
    String systemDump;
    systemLog = SD.open(systemLogId, FILE_WRITE);
    while (systemLog.available())
    {
        systemDump.append(systemLog.read());
    }
    dataLog.close();
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
