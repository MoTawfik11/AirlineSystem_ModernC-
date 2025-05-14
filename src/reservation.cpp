#include "reservation.h"
#include <json-develop/include/nlohmann/json.hpp>

using json = nlohmann::json;

Reservation::Reservation(std::string fn, std::string name, int s)
    : flightNumber(fn), passengerName(name), seats(s) {}

Reservation::Reservation(std::string rid, std::string pid, std::string fn, 
                         std::string seat, std::string payM, std::string payD, 
                         double cost)
    : reservationID(rid), passengerID(pid), flightNumber(fn),
      seatNumber(seat), paymentMethod(payM), paymentDetails(payD),
      totalCost(cost), checkedIn(false) {}

Reservation::Reservation() {}

json Reservation::toJson() const {
    return {
        {"reservationID", reservationID},
        {"passengerID", passengerID},
        {"flightNumber", flightNumber},
        {"seatNumber", seatNumber},
        {"paymentMethod", paymentMethod},
        {"paymentDetails", paymentDetails},
        {"totalCost", totalCost}
    };
}