#include "Simulator.h"
#include "Nav.h"

void Simulator::init() {
    setSyncProvider(simGetTime);
    setSyncInterval(60*60);
    simSerial.begin(9600);
}

void Simulator::update() {
    if(1 != timer.check()) return;

    bool debug = debugTimer.check();

    // Send firmware state to simulator
    StaticJsonBuffer<1024> outputBuff;
    JsonObject &output = outputBuff.createObject();
    output["ml"] = Motors::instance().getLeft();
    output["mr"] = Motors::instance().getRight();
    Vec3d pos = Nav::instance().getPosition();
    Point where = Point(pos);
    output["lat"] = where.getLat();
    output["lon"] = where.getLon();
    output["z"] = pos.getZ();
    // if(wpTimer.check() && Nav::instance().waypoints.isNew()) {
    if(0) {
        Nav *nav = &Nav::instance();
        JsonArray& waypoints = output.createNestedArray("wp");
        for(size_t i=0; i<nav->waypoints.length(); i++) {
            Point coord = nav->waypoints.get(i);
            JsonObject& wp = waypoints.createNestedObject();
            wp["lat"] = coord.getLat();
            wp["lon"] = coord.getLon();
            waypoints.add(wp);
        }
        output["wp"] = waypoints;
    }
    // if(debug) {
    //     String jsonOut;
    //     output.printTo(jsonOut);
    //     LogInfo("%s\n", jsonOut.c_str);
    // }
    output.printTo(simSerial);
    simSerial.print("\n");
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
}

time_t simGetTime() {
    tmElements_t tm;
    Simulator *sim = &Simulator::instance();
    if(sim->year != 0) {
        tm.Year = sim->year;
        tm.Month = sim->month;
        tm.Day = sim->day;
        tm.Hour = sim->hour;
        tm.Minute = sim->min;
        tm.Second = sim->sec;
        return makeTime(tm);
    } else {
        return 0;
    }
}