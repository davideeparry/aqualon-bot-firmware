#if !defined(PATHS_H)
#define PATHS_H

// Some preset paths for testing

#include "Nav.h"

void loadPathsAQ() {
    // AQ Pond
    Nav *nav = &Nav::instance();
    nav->waypoints.add(49.279297,-122.916940);
    nav->waypoints.add(49.279272,-122.916784);
    nav->waypoints.add(49.279229,-122.916828);
    nav->waypoints.add(49.279146,-122.916821);
    nav->waypoints.add(49.279006,-122.916880);
    nav->waypoints.add(49.279003,-122.917036);
    nav->waypoints.add(49.279065,-122.916997);
}

void loadPathsAQBack() {
    // AQ Pond counter clockwise
    Nav *nav = &Nav::instance();
    nav->waypoints.add(49.279006, -122.916880);
    nav->waypoints.add(49.279146, -122.916821);
    nav->waypoints.add(49.279229, -122.916828);
    nav->waypoints.add(49.279272, -122.916784);
    nav->waypoints.add(49.279297, -122.916940);
    nav->waypoints.add(49.279065, -122.916997);
    nav->waypoints.add(49.279003, -122.917036);
}

void loadPathsTroutLakeDiamond() {
    Nav *nav = &Nav::instance();
    nav->waypoints.add(49.257182,-123.062937);
    nav->waypoints.add(49.257042,-123.062331);
    nav->waypoints.add(49.256752,-123.062331);
    nav->waypoints.add(49.256907,-123.062930);

}

void loadPathsTroutLakeShort() {
    Nav *nav = &Nav::instance();
    nav->waypoints.add(49.257192, -123.062945);
    nav->waypoints.add(49.257193, -123.062686);
    nav->waypoints.add(49.257062, -123.062804);
    nav->waypoints.add(49.257064, -123.063068);
}

// Trout Lake North to South
/*
waypoints.add(49.257185,-123.062932);
waypoints.add(49.256627,-123.063015);
waypoints.add(49.255988,-123.062730);
waypoints.add(49.255630,-123.062317);
waypoints.add(49.255049,-123.061638);
waypoints.add(49.254868,-123.061605);
waypoints.add(49.254665,-123.061807);
waypoints.add(49.254379,-123.061705);
*/
#endif