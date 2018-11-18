#include "Gps.h"

void Gps::init() {  // GPS TX/RX are plugged into gpsSerial
    LOG("Initializing GPS");
    gpsSerial.begin(9600);
};

char gps_tmp_buff[128];

void Gps::update() 
{
    if(1 != timer.check()) return;

    while(gpsSerial.available() > 0) gps.encode(gpsSerial.read());

    // Did a new valid sentence come in?
    if(gps.location.isUpdated() && gps.location.isValid()) { 
        // CommunicationCommunications::instance().sendMessage(c);s::instance().sendMessage("encoded");
        lat = gps.location.lat();
        lon = gps.location.lng();
        newData = 1;
    }

    if(gps.course.isUpdated() && gps.course.isValid()) {
        // LOG("GPS: New course (heading)")
        courseRadians = PI * gps.course.deg() / 180.0;
        newData = 1;
    }

    if(gps.speed.isUpdated() && gps.speed.isValid()) {
        // LOG("GPS: New speed")
        speed = gps.speed.mps();
        newData = 1;
    }
    //Communications::instance().sendMessage("hit SERIAL");
};
