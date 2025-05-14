#include "passengerdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDateTime>
#include <QTextEdit>
#include <QLabel>
#include <memory> // for shared_ptr
using namespace std;

PassengerDashboard::PassengerDashboard(QWidget *parent) : QWidget(parent) {
    database.loadFlights();
    database.loadReservations();
    setupUI();
}

void PassengerDashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Header
    QLabel *header = new QLabel("Passenger Dashboard");
    header->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50;");
    header->setAlignment(Qt::AlignCenter);
    
    // Tab Widget
    QTabWidget *tabWidget = new QTabWidget;
    
    // Search Flights Tab
    QWidget *searchTab = new QWidget;
    setupSearchTab(searchTab);
    
    // My Reservations Tab
    QWidget *reservationsTab = new QWidget;
    setupReservationsTab(reservationsTab);
    
    tabWidget->addTab(searchTab, "Search Flights");
    tabWidget->addTab(reservationsTab, "My Reservations");
    
    // Logout Button
    QPushButton *logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 8px; border-radius: 4px;");
    
    mainLayout->addWidget(header);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(logoutButton);
    
    connect(logoutButton, &QPushButton::clicked, this, &PassengerDashboard::logout);
}

void PassengerDashboard::setupSearchTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Search Form
    QGroupBox *searchGroup = new QGroupBox("Flight Search");
    QFormLayout *formLayout = new QFormLayout;
    
    originEdit = new QLineEdit;
    destinationEdit = new QLineEdit;
    // dateEdit = new QDateEdit(QDate::currentDate());
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
    
    layout->addWidget(searchGroup);
    layout->addWidget(searchButton);
    layout->addWidget(resultsTable);
    
    connect(searchButton, &QPushButton::clicked, this, &PassengerDashboard::searchFlights);
}

void PassengerDashboard::setupReservationsTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Reservations Table
    reservationsTable = new QTableWidget;
    reservationsTable->setColumnCount(5);
    reservationsTable->setHorizontalHeaderLabels({"Reservation ID", "Flight #", "Seat", "Status", "Check-In"});
    reservationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Load sample data
    updateReservationsTable();
    
    // Check-In Button
    QPushButton *checkInButton = new QPushButton("Check-In");
    checkInButton->setStyleSheet("background-color: #2ecc71; color: white;");
    
    layout->addWidget(reservationsTable);
    layout->addWidget(checkInButton);
    
    connect(checkInButton, &QPushButton::clicked, this, &PassengerDashboard::checkIn);
}

void PassengerDashboard::searchFlights() {
    std::string origin = originEdit->text().toStdString();
    std::string destination = destinationEdit->text().toStdString();
    // std::string date = dateEdit->date().toString("yyyy-MM-dd").toStdString();
    
    std::vector<std::shared_ptr<Flight>> results = database.SearchFlights(origin, destination);
    
    resultsTable->setRowCount(results.size());
    
    for (size_t i = 0; i < results.size(); ++i) {
        std::shared_ptr<Flight> flight = results[i];
        
        resultsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(flight->flightNumber)));
        resultsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(flight->origin)));
        resultsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(flight->destination)));
        resultsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(flight->departureDateTime)));
        resultsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(flight->arrivalDateTime)));
        // resultsTable->setItem(i, 5, new QTableWidgetItem(QString::number(flight->seatsAvailable)));
    }
}

void PassengerDashboard::updateReservationsTable() {
    // For demo purposes, show all reservations
    // In a real app, filter by passenger ID
    reservationsTable->setRowCount(database.reservations.size());
    
    for (size_t i = 0; i < database.reservations.size(); ++i) {
        std::shared_ptr<Reservation> res = database.reservations[i];
        
        reservationsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(res->reservationID)));
        reservationsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(res->flightNumber)));
        reservationsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(res->seatNumber)));
        reservationsTable->setItem(i, 3, new QTableWidgetItem(res->checkedIn ? "Checked In" : "Not Checked In"));
        
        // Add a checkbox for check-in status
        QWidget *checkWidget = new QWidget;
        QHBoxLayout *checkLayout = new QHBoxLayout(checkWidget);
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setChecked(res->checkedIn);
        checkBox->setEnabled(!res->checkedIn);
        checkLayout->addWidget(checkBox);
        checkLayout->setAlignment(Qt::AlignCenter);
        checkLayout->setContentsMargins(0, 0, 0, 0);
        checkWidget->setLayout(checkLayout);
        
        reservationsTable->setCellWidget(i, 4, checkWidget);
    }
}

void PassengerDashboard::checkIn() {
    int row = reservationsTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select a reservation to check-in.");
        return;
    }
    
    if (database.reservations[row]->checkedIn) {
        QMessageBox::information(this, "Info", "You are already checked in.");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm", 
                            "Are you sure you want to check in for this flight?",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        database.reservations[row]->checkedIn = true;
        database.saveReservations();
        updateReservationsTable();
        QMessageBox::information(this, "Success", "Check-in completed!");
    }
}

void PassengerDashboard::logout() {
    emit logoutRequested();
}