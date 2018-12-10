#include "Gps.h"

time_t gpsGetTime() {
    return Gps::instance().getTime();
}

void Gps::init() {  // GPS TX/RX are plugged into gpsSerial
    LogInfo("Initializing GPS");
    #if !defined(SIMULATION)
    gpsSerial.begin(9600);
    setSyncProvider(gpsGetTime);
    setSyncInterval(GPS_TIME_SYNC_RATE);
    #endif
};

char gps_tmp_buff[128];

void Gps::update() 
{
    if(1 != timer.check()) return;

    #if defined(SIMULATION)
    static Simulator *sim = &Simulator::instance();
    lat = sim->lat;
    lon = sim->lon;
    courseRadians = PI * sim->angle / 180.0;
    speed = sim->speed;
    newData = 1;
    if(!timeInit) {
        time_t t = simGetTime();
        if(0 != t) {
            setTime(t);
            timeInit = true;
        }
    }
    #else

    while(gpsSerial.available() > 0) gps.encode(gpsSerial.read());

    // Did a new valid sentence come in?
    if(gps.location.isUpdated() && gps.location.isValid()) { 
        // CommunicationCommunications::instance().sendMessage(c);s::instance().sendMessage("encoded");
        lat = gps.location.lat();
        lon = gps.location.lng();
        newData = 1;
    }

    if(gps.course.isUpdated() && gps.course.isValid()) {
        courseRadians = PI * gps.course.deg() / 180.0;
        newData = 1;
    }

    if(gps.speed.isUpdated() && gps.speed.isValid()) {
        speed = gps.speed.mps();
        newData = 1;
    }

    if(!timeInit && gps.date.isValid()) {
        setTime(getTime());
        timeInit = true;
    }

    #endif

    if(1 == debugTimer.check()) {
        LogDebug("GPS Lat: %f, Lon: %f, Speed: %f, Heading: %f", lat, lon, speed, courseRadians);
    }
    //Communications::instance().sendMessage("hit SERIAL");
};

time_t Gps::getTime() {
    tmElements_t tm;
    if(gps.time.isUpdated() && gps.time.isValid()) {
        tm.Year = CalendarYrToTm(gps.date.year());
        tm.Month = gps.date.month();
        tm.Day = gps.date.day();
        tm.Hour = gps.time.hour();
        tm.Minute = gps.time.minute();
        tm.Second = gps.time.second();
        time_t t = makeTime(tm);
        t += timeZoneOffset * SECS_PER_HOUR;
        return t;
    } else {
        return 0;
    }
}