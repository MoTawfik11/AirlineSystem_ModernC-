#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <iostream>
#include <json-develop/include/nlohmann/json.hpp>

class Flight {
public:
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureDateTime;
    std::string arrivalDateTime;
    std::string aircraftType;
    std::string status;      // Moved before the ints
    int totalSeats;
    // int seatsAvailable;

    Flight();
    Flight(std::string fNum, std::string org, std::string dest, std::string dep, std::string arr, std::string aircraft, std::string stat, int seats);

    nlohmann::json toJson() const;
};

#endif // FLIGHT_H