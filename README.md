# ✈️ Airline Reservation and Management System

A modern C++ console-based Airline Reservation and Management System designed to simulate the core functionalities of airline ticket booking, passenger management, and flight scheduling. This project utilizes **Object-Oriented Programming (OOP)** principles and **Modern C++** features such as smart pointers, STL containers, file persistence using JSON, and optional multithreading.

---

## 🚀 Features

- 🧳 Passenger Registration & Management
- 📅 Flight Scheduling & Lookup
- 🛫 Ticket Booking & Cancellation
- 📜 Booking History & Records
- 💾 Data Persistence using `nlohmann/json`
- 🧠 Designed with Smart Pointers & STL Containers
- 🔐 Basic Authentication (optional)
- ⚙️ Multithreaded Operations (optional enhancement)

---

## 🛠️ Tech Stack

- **Language:** C++17 or later
- **Libraries:**
  - [`nlohmann/json`](https://github.com/nlohmann/json) for JSON file persistence
  - Standard Template Library (STL) for data structures
  - `<memory>` for smart pointers
  - `<thread>` for multithreading (if used)

---

## 📁 Project Structure

airline-reservation-system/
│
├── src/ # Source code (.cpp files)
│ ├── main.cpp
│ ├── AirlineSystem.cpp
│ ├── Passenger.cpp
│ ├── Flight.cpp
│ └── Booking.cpp
│
├── Header/ # Header files (.hpp files)
│ ├── AirlineSystem.hpp
│ ├── Passenger.hpp
│ ├── Flight.hpp
│ └── Booking.hpp
│
├── AirlineSystem.pro
├── passengers.json
├── flights.json
├── bookings.json
├── bookings.json


---

## 🧪 How to Build and Run

### Requirements

- C++17-compatible compiler (G++, Clang, MSVC)
- CMake or Make
- `nlohmann/json.hpp` (included or via package manager)

### Build Instructions (with CMake)

```bash
git clone https://github.com/yourusername/airline-reservation-system.git
cd airline-reservation-system
mkdir build && cd build
cmake ..
make
./airline_system
```
---

##  📌 Notes
Ensure your JSON data files exist in the data/ directory before running.

Smart pointers (std::shared_ptr, std::unique_ptr) are used to manage memory automatically.

Multithreading is implemented in [XYZ section] for tasks like simultaneous booking or data sync (if applicable).

##  📷 Demo
(Insert screenshots or terminal session gifs here using asciinema or .gif)

##  🤝 Contributions
Pull requests are welcome! For major changes, please open an issue first to discuss what you would like to change.

##  📄 License
This project is open-source and available under the MIT License.

##  👨‍💻 Author
Mohamed Tawfik
Mechatronics Engineer | C++ Developer | Robotics & AI Enthusiast


---

Let me know if you'd like to include specific screenshots, usage examples, or a license file as well!
