#ifndef UTMCONVERTER_H
#define UTMCONVERTER_H

#include <QPointF>
#include <QString>
#include <cmath>

struct UTMCooordinates {
    int zone;
    bool is_north;
    double easting;
    double northing;
};

class UTMConverter {
public:
    static UTMCooordinates wgs84ToUTM(double lat, double lon);
    static QString utmToString(const UTMCooordinates &utm);
};

#endif // UTMCONVERTER_H
