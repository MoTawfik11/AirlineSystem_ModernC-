#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <json-develop/include/nlohmann/json.hpp>

class Reservation {
public:
    std::string reservationID;
    std::string passengerID;
    std::string flightNumber;
    std::string seatNumber;
    std::string paymentMethod;
    std::string paymentDetails;
    std::string passengerName;
    int seats;
    double totalCost;
    bool checkedIn;

    Reservation(std::string fn, std::string name, int s);
    Reservation(std::string rid, std::string pid, std::string fn, 
                std::string seat, std::string payM, std::string payD, 
                double cost);
    Reservation();

    nlohmann::json toJson() const;
};

#endif // RESERVATION_H