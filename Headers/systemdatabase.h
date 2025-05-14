#ifndef SYSTEMDATABASE_H
#define SYSTEMDATABASE_H

#include <vector>
#include <memory>
#include <string>
#include "flight.h"
#include "aircraft.h"
#include "reservation.h"

class SystemDatabase {
public:
    void loadFlights();
    void saveFlights();
    void loadAircrafts();
    void saveAircrafts();
    void addFlight(std::shared_ptr<Flight> flight);
    void addAircraft(std::shared_ptr<Aircraft> aircraft);
    void loadReservations();
    void saveReservations();
    void addReservation(std::shared_ptr<Reservation> reservation);
    bool isSeatTaken(const std::string& flightNum, const std::string& seatNum);
    std::vector<std::shared_ptr<Flight>> SearchFlights(const std::string& origin, 
                                                     const std::string& destination 
                                                     );
    std::vector<std::shared_ptr<Flight>> flights;
    std::vector<std::shared_ptr<Aircraft>> aircrafts;
    std::vector<std::shared_ptr<Reservation>> reservations;
};

#endif // SYSTEMDATABASE_H