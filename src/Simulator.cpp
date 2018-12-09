#include "Simulator.h"
#include "Nav.h"

void Simulator::init() {
    setSyncProvider(simGetTime);
    setSyncInterval(60*60*24);
    simSerial.begin(9600);
}

void Simulator::update() {
    if(1 != timer.check()) return;

    bool debug = debugTimer.check();
    if(debug) LogInfo("Updating simulator input");
    // Process incoming state
    if(simSerial.available()) {
        StaticJsonBuffer<1024> jsonBuff;
        JsonObject &root = jsonBuff.parseObject(simSerial); // Normally the GPS Serial input

        lat = root["lat"];
        lon = root["lon"];
        speed = root["speed"];
        angle = root["angle"];

        gz = root["gz"];
        ax = root["ax"];
        ay = root["ay"];

        year = root["year"];
        month = root["month"];
        day = root["day"];
        hour = root["hour"];
        min = root["minute"];
        sec = root["second"];

        // if (debug) { root.prettyPrintTo(Serial); Serial.print("\n"); }
    }
    // Send firmware state to simulator
    if(debug) LogDebug("Sending state");
    StaticJsonBuffer<256> outputBuff;
    JsonObject &output = outputBuff.createObject();
    output["ml"] = Motors::instance().getLeft();
    output["mr"] = Motors::instance().getRight();
    Vec3d pos = Nav::instance().getPosition();
    output["x"] = pos.getX();
    output["y"] = pos.getY();
    output["z"] = pos.getZ();
    // if(debug) {
    //     String jsonOut;
    //     output.printTo(jsonOut);
    //     LogInfo("%s\n", jsonOut.c_str);
    // }
    output.printTo(simSerial);
    simSerial.print("\n");
}

time_t simGetTime() {
    tmElements_t tm;
    Simulator *sim = &Simulator::instance();
    if(sim->year != 0) {
        LogDebug("Got new time from Simulator");
        tm.Year = sim->year;
        tm.Month = sim->month;
        tm.Day = sim->day;
        tm.Hour = sim->hour;
        tm.Minute = sim->min;
        tm.Second = sim->sec;
        return makeTime(tm);
    } else {
        LogDebug("Sim time sync failed");
        return 0;
    }
}