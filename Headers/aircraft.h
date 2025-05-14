#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <json-develop/include/nlohmann/json.hpp>

class Aircraft {
    public:
        std::string aircraftID;
        std::string model;
        std::string manufacturer;
        int capacity;
        std::string status; // Available, Maintenance, Retired
    
    
    Aircraft();
    Aircraft(std::string id, std::string mod, std::string manuf, int cap, std::string stat);
    nlohmann::json toJson() const;

    static Aircraft fromJson(const nlohmann::json& j);

};
#endif // AIRCRAFT_H