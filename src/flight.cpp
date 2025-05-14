#include "flight.h"
#include <json-develop/include/nlohmann/json.hpp>

using json = nlohmann::json;

Flight::Flight() {}

Flight::Flight(std::string fNum, std::string org, std::string dest, std::string dep, std::string arr, std::string aircraft, std::string stat, int seats)
    : flightNumber(fNum), origin(org), destination(dest),
      departureDateTime(dep), arrivalDateTime(arr),
      aircraftType(aircraft), status(stat) , 
      totalSeats(seats)
    //   , seatsAvailable(availableSeats) 
      {}



json Flight::toJson() const {
    return {
        {"flightNumber", flightNumber},
        {"origin", origin},
        {"destination", destination},
        {"departureDateTime", departureDateTime},
        {"arrivalDateTime", arrivalDateTime},
        {"aircraftType", aircraftType},
        {"status", status},
        {"totalSeats", totalSeats}
        // {"seatsAvailable", seatsAvailable}
    };
}