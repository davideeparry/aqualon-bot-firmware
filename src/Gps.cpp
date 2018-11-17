#include "Gps.h"
#include "GpsISR.h"

void Gps::init(int update_interval) {  // GPS TX/RX are plugged into gpsSerial
    LOG("Initializing GPS");
    gpsSerial.begin(9600);
    // Timer1.initialize(50000);
    // Timer1.attachInterrupt(gpsISRWrapper);
    timer.interval(update_interval);
};


void Gps::update() 
{
    if(1 != timer.check()) return;
    while(gpsSerial.available()) gps.encode(gpsSerial.read());
    StateService *state = &StateService::instance();

    // Did a new valid sentence come in?
    if(gps.location.isUpdated() && gps.location.isValid()) { 
        // CommunicationCommunications::instance().sendMessage(c);s::instance().sendMessage("encoded");
        LOG("GPS: New location")
        state->gpsLat = gps.location.lat();
        state->gpsLong = gps.location.lng();
        state->gpsNew = 1;
    }

    if(gps.course.isUpdated() && gps.course.isValid()) {
        LOG("GPS: New course (heading)")
        state->gpsCourseDegrees = gps.course.deg();
        state->gpsCourseRadians = PI * gps.course.deg() / 180.0;
        state->gpsNew = 1;
    }

    if(gps.speed.isUpdated() && gps.speed.isValid()) {
        LOG("GPS: New speed")
        state->gpsSpeed = gps.speed.mps();
        state->gpsNew = 1;
    }
    //Communications::instance().sendMessage("hit SERIAL");
};

void Gps::isr()
{

}