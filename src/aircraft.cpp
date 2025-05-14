// aircraft.cpp

#include "aircraft.h"
#include <string>
#include <iostream>
#include <vector>
#include <json-develop/include/nlohmann/json.hpp>

using json = nlohmann::json;

Aircraft::Aircraft() {}

Aircraft::Aircraft(std::string id, std::string mod, std::string manuf, int cap, std::string stat)
    : aircraftID(id), model(mod), manufacturer(manuf), capacity(cap), status(stat) {}

json Aircraft::toJson() const {
    return {
        {"aircraftID", aircraftID},
        {"model", model},
        {"manufacturer", manufacturer},
        {"capacity", capacity},
        {"status", status}
    };
}

// ✅ Added fromJson() implementation
Aircraft Aircraft::fromJson(const json& j) {
    return Aircraft(
        j.at("aircraftID").get<std::string>(),
        j.at("model").get<std::string>(),
        j.at("manufacturer").get<std::string>(),
        j.at("capacity").get<int>(),           // Ensure this is parsed as int
        j.at("status").get<std::string>()
    );
}
