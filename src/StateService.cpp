#include "StateService.h"

int StateService::addWaypoint(double lat, double lon) {
    if(numWaypoints < MAX_WAYPOINTS) {
        waypoints[numWaypoints] = Point(lat, lon);
        numWaypoints++;
        return 0;
    } else {
        LOG("Max waypoints filled");
        return 1;
    }
}

// Get closest waypoint to current position
int StateService::getClosestWaypoint() {
    if(numWaypoints == 0) return -1;
    int closest = -1;
    float closestDist = MAXFLOAT;
    for(int i=0; i<numWaypoints; i++) {
        Point position = Point(position);
        Point wp = waypoints[i];
        float dist = (wp - position).getDist();
        if(dist < closestDist) {
            closestDist = dist;
            closest = i;
        }
    }
    return closest;
}

void StateService::incrementNextWaypoint() {
    nextWaypoint = (nextWaypoint + 1) % numWaypoints;
}