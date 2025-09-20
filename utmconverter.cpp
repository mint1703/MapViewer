#include "utmconverter.h"
#include <cmath>
#include <QString>

UTMCooordinates UTMConverter::wgs84ToUTM(double lat, double lon) {
    UTMCooordinates result;

    result.zone = static_cast<int>((lon + 180.0) / 6.0) + 1;
    result.is_north = lat >= 0;

    const double a = 6378137.0;
    const double f = 1.0 / 298.257223563;
    const double k0 = 0.9996;

    double lat_rad = lat * M_PI / 180.0;
    double lon_rad = lon * M_PI / 180.0;

    double lon0_rad = ((result.zone - 1) * 6.0 - 180.0 + 3.0) * M_PI / 180.0;

    //double e2 = 2 * f - f * f;
    double n = f / (2 - f);
    double A = a / (1 + n) * (1 + n * n / 4 + n * n * n * n / 64);

    double t = std::sinh(std::atanh(std::sin(lat_rad)) - 2 * std::sqrt(n) / (1 + n) * std::atanh(2 * std::sqrt(n) / (1 + n) * std::sin(lat_rad)));
    double xi_prime = std::atan(t / std::cos(lon_rad - lon0_rad));
    double eta_prime = std::atanh(std::sin(lon_rad - lon0_rad) / std::sqrt(1 + t * t));

    double alpha1 = n / 2.0 - 2.0 * n * n / 3.0 + 5.0 * n * n * n / 16.0;
    double alpha2 = 13.0 * n * n / 48.0 - 3.0 * n * n * n / 5.0;
    double alpha3 = 61.0 * n * n * n / 240.0;

    result.easting = k0 * A * (eta_prime +
                               alpha1 * std::cos(2 * xi_prime) * std::sinh(2 * eta_prime) +
                               alpha2 * std::cos(4 * xi_prime) * std::sinh(4 * eta_prime) +
                               alpha3 * std::cos(6 * xi_prime) * std::sinh(6 * eta_prime)) + 500000.0;

    result.northing = k0 * A * (xi_prime +
                                alpha1 * std::sin(2 * xi_prime) * std::cosh(2 * eta_prime) +
                                alpha2 * std::sin(4 * xi_prime) * std::cosh(4 * eta_prime) +
                                alpha3 * std::sin(6 * xi_prime) * std::cosh(6 * eta_prime));

    if (!result.is_north) {
        result.northing += 10000000.0;
    }

    return result;
}

QString UTMConverter::utmToString(const UTMCooordinates &utm) {
    return QString("UTM Zone %1%2: %3E %4N")
    .arg(utm.zone)
        .arg(utm.is_north ? "N" : "S")
        .arg(utm.easting, 0, 'f', 2)
        .arg(utm.northing, 0, 'f', 2);
}
