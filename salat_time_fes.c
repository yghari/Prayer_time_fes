#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>
#include <jansson.h>

#define API_URL "http://api.aladhan.com/v1/timingsByCity?city=Fes&country=Morocco&method=2"

// Callback function for libcurl
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    char *response = (char *)userdata;
    strncat(response, ptr, realsize);
    return realsize;
}

int main() {
    CURL *curl;
    CURLcode res;
    char response[4096] = "";
    json_t *root;
    json_error_t error;
    struct tm prayer_times;
    time_t time;
    
    // Initialize libcurl
    curl = curl_easy_init();
    if (!curl) {
        printf("Error initializing libcurl.\n");
        return 1;
    }
    
    // Set the API endpoint
    curl_easy_setopt(curl, CURLOPT_URL, API_URL);
    
    // Set the callback function to receive the response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
    
    // Perform the API request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        printf("Error fetching data from API: %s\n", curl_easy_strerror(res));
        return 1;
    }
    
    // Clean up libcurl
    curl_easy_cleanup(curl);
    
    // Parse the JSON response
    root = json_loads(response, 0, &error);
    if (!root) {
        printf("Error parsing JSON response: %s\n", error.text);
        return 1;
    }
    
    // Extract the prayer times from the response
    json_t *timings = json_object_get(root, "data");
    timings = json_object_get(timings, "timings");
    
    // Convert the prayer times to struct tm
    const char *prayer_names[] = {"Fajr", "Sunrise", "Dhuhr", "Asr", "Maghrib", "Isha"};
    for (int i = 0; i < 6; i++) {
        json_t *prayer_time = json_object_get(timings, prayer_names[i]);
        strptime(json_string_value(prayer_time), "%I:%M %p", &prayer_times);
        time = mktime(&prayer_times);
        printf("%s: %s", prayer_names[i], asctime(localtime(&time)));
    }
    
    // Clean up json-c
    json_decref(root);
    
    return 0;
}

/*This program uses the libcurl library to fetch the prayer times from the Aladhan API, and the json-c library to parse the JSON response. It then converts the prayer times to struct tm objects using the strptime() function, and then to time_t objects using the mktime() function. Finally, it displays the prayer times using the asctime() function. You can compile and run this program to see the prayer times for today. */
