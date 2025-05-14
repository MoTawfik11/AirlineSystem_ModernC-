#include "systemdatabase.h"
#include <fstream>
#include <algorithm>

void SystemDatabase::loadFlights() {
    std::ifstream inFile("flights.json");
    if (!inFile) return;
    nlohmann::json j;
    inFile >> j;
    for (auto& f: j){
        flights.push_back(std::make_shared<Flight>(
            f["flightNumber"], f["origin"], f["destination"],
            f["departureDateTime"], f["arrivalDateTime"],
            f["aircraftType"], f["status"], f["totalSeats"]
        ));
    }
    inFile.close();
}

void SystemDatabase::saveFlights() {
    nlohmann::json j;
    for (auto& f : flights) {
        j.push_back(f->toJson());
    }
    std::ofstream outFile("flights.json");
    outFile << j.dump(4);
}

void SystemDatabase::loadAircrafts() {
    std::ifstream inFile("aircrafts.json");
    if (!inFile) return;
    nlohmann::json j;
    inFile >> j;
    for (auto& a : j) {
        aircrafts.push_back(std::make_shared<Aircraft>(
            a["aircraftID"], a["model"], a["manufacturer"],
            a["capacity"], a["status"]
        ));
    }
}

void SystemDatabase::saveAircrafts() {
    nlohmann::json j;
    for (auto& a : aircrafts) {
        j.push_back(a->toJson());
    }
    std::ofstream outFile("aircrafts.json");
    outFile << j.dump(4);
}

void SystemDatabase::addAircraft(std::shared_ptr<Aircraft> aircraft) {
    aircrafts.push_back(aircraft);
    saveAircrafts();
}

void SystemDatabase::addFlight(std::shared_ptr<Flight> flight) {
    flights.push_back(flight);
    saveFlights();
}

void SystemDatabase::loadReservations() {
    std::ifstream inFile("reservations.json");
    if (!inFile) return;
    nlohmann::json j;
    inFile >> j;
    for (auto& r : j) {
        reservations.push_back(std::make_shared<Reservation>(
            r["reservationID"], r["passengerID"], r["flightNumber"],
            r["seatNumber"], r["paymentMethod"], r["paymentDetails"], r["totalCost"]
        ));
    }
}

void SystemDatabase::saveReservations() {
    nlohmann::json j;
    for (auto& r : reservations) {
        j.push_back(r->toJson());
    }
    std::ofstream outFile("reservations.json");
    outFile << j.dump(4);
}

void SystemDatabase::addReservation(std::shared_ptr<Reservation> reservation) {
    reservations.push_back(reservation);
    saveReservations();
}

bool SystemDatabase::isSeatTaken(const std::string& flightNum, const std::string& seatNum) {
    for (auto& r : reservations) {
        if (r->flightNumber == flightNum && r->seatNumber == seatNum) {
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<Flight>> SystemDatabase::SearchFlights(
    const std::string& origin, const std::string& destination) {
    std::vector<std::shared_ptr<Flight>> results;
    for (auto& f : flights) {
        if (f->origin == origin && f->destination == destination && f->status == "Scheduled") {
            results.push_back(f);
        }
    }
    return results;
}