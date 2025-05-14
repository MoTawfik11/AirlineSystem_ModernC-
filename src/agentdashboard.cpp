#include "agentdashboard.h"
#include <QDateTime>
#include <QDebug>
#include <QDate>
#include <memory> // for shared_ptr
using namespace std;
SystemDatabase database;


AgentDashboard::AgentDashboard(QWidget *parent) : QWidget(parent) {
    database.loadFlights();
    database.loadReservations();
    setupUI();
}

void AgentDashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Header
    QLabel *header = new QLabel("Booking Agent Dashboard");
    header->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50;");
    header->setAlignment(Qt::AlignCenter);
    
    // Tab Widget
    QTabWidget *tabWidget = new QTabWidget;
    
    // Search Flights Tab
    QWidget *searchTab = new QWidget;
    setupSearchTab(searchTab);
    
    // Reservations Tab
    QWidget *reservationsTab = new QWidget;
    setupReservationsTab(reservationsTab);
    
    tabWidget->addTab(searchTab, "Search Flights");
    tabWidget->addTab(reservationsTab, "Reservations");
    
    // Logout Button
    QPushButton *logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 8px; border-radius: 4px;");
    
    mainLayout->addWidget(header);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(logoutButton);
    
    connect(logoutButton, &QPushButton::clicked, this, &AgentDashboard::logout);
}

void AgentDashboard::setupSearchTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Search Form
    QGroupBox *searchGroup = new QGroupBox("Flight Search");
    QFormLayout *formLayout = new QFormLayout;
    
    originEdit = new QLineEdit;
    destinationEdit = new QLineEdit;
    // dateEdit = new QDateEdit(QDate::currentDate());
    // dateEdit->setCalendarPopup(true);
    // dateEdit->setDate(QDate::currentDate());
    // dateEdit->setDisplayFormat("yyyy-MM-dd");
    
    formLayout->addRow("Origin:", originEdit);
    formLayout->addRow("Destination:", destinationEdit);
    // formLayout->addRow("Date:", dateEdit);
    
    searchGroup->setLayout(formLayout);
    
    // Search Button
    QPushButton *searchButton = new QPushButton("Search Flights");
    searchButton->setStyleSheet("background-color: #3498db; color: white;");
    
    // Results Table
    resultsTable = new QTableWidget;
    resultsTable->setColumnCount(6);
    resultsTable->setHorizontalHeaderLabels({"Flight #", "Origin", "Destination", "Departure", "Arrival", "Seats"});
    resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Book Button
    QPushButton *bookButton = new QPushButton("Book Selected Flight");
    bookButton->setStyleSheet("background-color: #2ecc71; color: white;");
    
    layout->addWidget(searchGroup);
    layout->addWidget(searchButton);
    layout->addWidget(resultsTable);
    layout->addWidget(bookButton);
    
    connect(searchButton, &QPushButton::clicked, this, &AgentDashboard::SearchFlights);
    connect(bookButton, &QPushButton::clicked, this, &AgentDashboard::bookFlight);
}

void AgentDashboard::setupReservationsTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Reservations Table
    reservationsTable = new QTableWidget;
    reservationsTable->setColumnCount(6);
    reservationsTable->setHorizontalHeaderLabels({"Reservation ID", "Flight #", "Passenger", "Seats", "Status", "Actions"});
    reservationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Load sample data
    updateReservationsTable();
    
    // Action Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *modifyButton = new QPushButton("Modify");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *refreshButton = new QPushButton("Refresh");
    
    modifyButton->setStyleSheet("background-color: #3498db; color: white;");
    cancelButton->setStyleSheet("background-color: #e74c3c; color: white;");
    refreshButton->setStyleSheet("background-color: #7f8c8d; color: white;");
    
    buttonLayout->addWidget(modifyButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(refreshButton);
    
    layout->addWidget(reservationsTable);
    layout->addLayout(buttonLayout);
    
    connect(modifyButton, &QPushButton::clicked, this, &AgentDashboard::modifyReservation);
    connect(cancelButton, &QPushButton::clicked, this, &AgentDashboard::cancelReservation);
    connect(refreshButton, &QPushButton::clicked, this, &AgentDashboard::updateReservationsTable);
}


void AgentDashboard::SearchFlights() {
    std::string origin = originEdit->text().toStdString();
    std::string destination = destinationEdit->text().toStdString();

    if (origin.empty() || destination.empty()) {
        QMessageBox::warning(this, "Error", "Please enter both origin and destination.");
        return;
    }

    resultsTable->setRowCount(0);
    auto results = database.SearchFlights(origin, destination); // Use the already loaded database
    for (const auto& flight : results) {
        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);
        resultsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(flight->flightNumber)));
        resultsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(flight->origin)));
        resultsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(flight->destination)));
        resultsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(flight->departureDateTime)));
        resultsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(flight->arrivalDateTime)));
        resultsTable->setItem(row, 5, new QTableWidgetItem(QString::number(flight->totalSeats)));
    }

    if (resultsTable->rowCount() == 0) {
        QMessageBox::information(this, "No Results", "No flights found for the given criteria.");
    } else {
        resultsTable->selectRow(0); // Select the first row by default
    }
}


void AgentDashboard::bookFlight() {
    int row = resultsTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select a flight to book.");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Book Flight");
    
    QFormLayout form(&dialog);
    
    QLineEdit *passengerNameEdit = new QLineEdit;
    QLineEdit *passengerIDEdit = new QLineEdit;
    QSpinBox *seatsEdit = new QSpinBox;
    seatsEdit->setRange(1, 10);
    seatsEdit->setValue(1);
    
    QComboBox *paymentMethodCombo = new QComboBox;
    paymentMethodCombo->addItems({"Credit Card", "Debit Card", "PayPal"});
    
    form.addRow("Passenger Name:", passengerNameEdit);
    form.addRow("Passenger ID:", passengerIDEdit);
    form.addRow("Number of Seats:", seatsEdit);
    form.addRow("Payment Method:", paymentMethodCombo);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        string flightNumber = resultsTable->item(row, 0)->text().toStdString();
        string passengerName = passengerNameEdit->text().toStdString();
        string passengerID = passengerIDEdit->text().toStdString();
        int seats = seatsEdit->value();
        
        // Generate a random reservation ID
        string reservationID = "RES" + to_string(rand() % 10000);
        
        shared_ptr<Reservation> reservation = make_shared<Reservation>(
            reservationID, 
            "PID" + to_string(rand() % 1000), // Random passenger ID for demo
            flightNumber,
            to_string(rand() % 50 + 1) + "A", // Random seat for demo
            paymentMethodCombo->currentText().toStdString(),
            "****-****-****-1234", // Mock payment details
            150.0 * seats // Mock price calculation
        );
        
        database.addReservation(reservation);
        QMessageBox::information(this, "Success", "Booking confirmed!");
        updateReservationsTable();
    }
}

void AgentDashboard::updateReservationsTable() {
    reservationsTable->setRowCount(database.reservations.size());
    
    for (size_t i = 0; i < database.reservations.size(); ++i) {
        shared_ptr<Reservation> res = database.reservations[i];
        
        reservationsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(res->reservationID)));
        reservationsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(res->flightNumber)));
        reservationsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(res->passengerName)));
        reservationsTable->setItem(i, 3, new QTableWidgetItem(QString::number(res->seats)));
        reservationsTable->setItem(i, 4, new QTableWidgetItem(res->checkedIn ? "Checked In" : "Not Checked In"));
        
        // Action buttons would be added here in a more complete implementation
    }
}

void AgentDashboard::modifyReservation() {
    int row = reservationsTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select a reservation to modify.");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Modify Reservation");
    
    QFormLayout form(&dialog);
    
    QLineEdit *passengerNameEdit = new QLineEdit(reservationsTable->item(row, 2)->text());
    QSpinBox *seatsEdit = new QSpinBox;
    seatsEdit->setValue(reservationsTable->item(row, 3)->text().toInt());
    seatsEdit->setRange(1, 10);
    
    form.addRow("Passenger Name:", passengerNameEdit);
    form.addRow("Number of Seats:", seatsEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Update the reservation in the database
        database.reservations[row]->passengerName = passengerNameEdit->text().toStdString();
        database.reservations[row]->seats = seatsEdit->value();
        database.reservations[row]->totalCost = 150.0 * seatsEdit->value(); // Update price
        
        database.saveReservations();
        updateReservationsTable();
        QMessageBox::information(this, "Success", "Reservation updated!");
    }
}

void AgentDashboard::cancelReservation() {
    int row = reservationsTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select a reservation to cancel.");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm", 
                            "Are you sure you want to cancel this reservation?",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        database.reservations.erase(database.reservations.begin() + row);
        database.saveReservations();
        updateReservationsTable();
        QMessageBox::information(this, "Success", "Reservation canceled.");
    }
}

void AgentDashboard::logout() {
    emit logoutRequested();
}