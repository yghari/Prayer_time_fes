#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD(deg) (deg * (PI / 180.0))
#define RAD_TO_DEG(rad) (rad * (180.0 / PI))
#define J2000 2451545.0
#define JULIAN_CENTURY(jd) ((jd - J2000) / 36525.0)
#define SUN_ALTITUDE -0.833333333333333

struct Coordinates {
    double latitude;
    double longitude;
};

struct Time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

struct PrayerTimes {
    struct Time fajr;
    struct Time sunrise;
    struct Time dhuhr;
    struct Time asr;
    struct Time maghrib;
    struct Time isha;
};

double get_julian_date(int year, int month, int day, int hour, int minute, int second) {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    int jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    double jd = jdn + (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;
    return jd;
}

double get_sun_declination(double julian_century) {
    double a = DEG_TO_RAD(23.439291);
    double b = DEG_TO_RAD(0.0000004);
    double c = DEG_TO_RAD(0.0000004);
    double d = DEG_TO_RAD(-0.0000006);
    double e = DEG_TO_RAD(0.0000005);
    double obliquity = a - (b * julian_century) - (c * pow(julian_century, 2)) - (d * pow(julian_century, 3)) + (e * pow(julian_century, 4));
    double declination = asin(sin(obliquity) * sin(DEG_TO_RAD(0.0)));
    return declination;
}

double get_hour_angle(double latitude, double declination, double altitude) {
    double cosha = (sin(DEG_TO_RAD(altitude)) - (sin(DEG_TO_RAD(latitude)) * sin(declination))) / (cos(DEG_TO_RAD(latitude)) * cos(declination));
    double ha = RAD_TO_DEG(acos(cosha));
    return ha;
}

struct Time get_prayer_time(double julian_date, double latitude, double longitude, double timezone, double altitude, double ha) {
    double time_correction = longitude / 15.0;
    double local_time = (12.0 - time_correction) + (ha / 15.0);
    double utc_time = local_time - (timezone);
    double prayer_time = utc_time + (double) (J2000 - julian_date) / 36525.0;
    int year, month, day, hour, minute, second;
    double jd = J2000 + (prayer_time * 36525

