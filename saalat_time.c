#include <stdio.h>
#include <math.h>
#include <time.h>

double radians(double degree) {
    return (M_PI/180)*degree;
}

double degrees(double radian) {
    return (180/M_PI)*radian;
}

double calculate_julian_date(int year, int month, int day) {
    int a, b;
    if (month < 3) {
        year--;
        month += 12;
    }
    a = year / 100;
    b = 2 - a + (a / 4);
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

double calculate_sunrise_sunset(double jd, double lat, double lon, double alt, double zenith) {
    double rad_lat, rad_lon, rad_declination, rad_altitude, cos_hour_angle, hour_angle, time, sunset_time, sunrise_time;

    rad_lat = radians(lat);
    rad_lon = radians(lon);
    rad_declination = radians(23.45) * sin(radians(360 * (284 + jd) / 365.25));
    cos_hour_angle = (sin(radians(zenith)) - sin(rad_lat) * sin(rad_declination)) / (cos(rad_lat) * cos(rad_declination));
    if (cos_hour_angle > 1 || cos_hour_angle < -1) {
        return -1;
    }
    hour_angle = degrees(acos(cos_hour_angle));
    time = hour_angle / 15 + (lon / 15) - (alt / 3600);
    sunrise_time = 12 - time;
    sunset_time = 12 + time;
    return sunrise_time;
}

void print_time(double time) {
    int hours = (int)time;
    int minutes = (int)((time - hours) * 60);
    printf("%02d:%02d", hours, minutes);
}

int main() {
    double jd, fajr, sunrise, dhuhr, asr, maghrib, isha;
    time_t now;
    struct tm *timeinfo;
    int year, month, day;

    time(&now);
    timeinfo = localtime(&now);
    year = timeinfo->tm_year + 1900;
    month = timeinfo->tm_mon + 1;
    day = timeinfo->tm_mday;

    jd = calculate_julian_date(year, month, day);

    // Fajr
    fajr = calculate_sunrise_sunset(jd, 34.03, -5.02, -19.5/60, 18);

    // Sunrise
    sunrise = calculate_sunrise_sunset(jd, 34.03, -5.02, 0, 0);

    // Dhuhr
    dhuhr = 12 + (-5.02 / 15);

    // Asr
    asr = calculate_sunrise_sunset(jd, 34.03, -5.02, 0, 14.6);

    // Maghrib
    maghrib = calculate_sunrise_sunset(jd, 34.03, -5.02, 0, 0);

    // Isha
    isha = calculate_sunrise_sunset(jd, 34.03, -5.02, 18.0/60, 18);

    printf("Prayer Times for Fes, Morocco:\n");
    printf("Fajr: ");
    if (fajr == -1) {
	    printf("Not Found\n");
    } else {
	    print_time(fajr);
	    printf("\n");
    }
    printf("Sunrise: ");
    print_time(sunrise);
    printf("\n");
    printf("Dhuhr: ");
    print_time(dhuhr);
    printf("\n");
    printf("Asr: ");
    if (asr == -1) {
	    printf("Not Found\n");
    } else {
	    print_time(asr);
	    printf("\n");
    }
    printf("Maghrib: ");
    print_time(maghrib);
    printf("\n");
    printf("Isha: ");
    if (isha == -1) {
	    printf("Not Found\n");
    } else {
	    print_time(isha);
	    printf("\n");
    }
    return 0;
}
