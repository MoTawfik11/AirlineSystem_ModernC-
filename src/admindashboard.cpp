#include "admindashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QDateTime>
#include <QTextEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTableWidget>
#include <memory> // for shared_ptr
using namespace std;

AdminDashboard::AdminDashboard(QWidget *parent) : QWidget(parent) {
    database.loadFlights();
    database.loadAircrafts();
    database.loadReservations();
    
    setupUI();
}

void AdminDashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
        
    // Header
    QLabel *header = new QLabel("Administrator Dashboard");
    header->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50;");
    header->setAlignment(Qt::AlignCenter);
    
    // Tab Widget
    QTabWidget *tabWidget = new QTabWidget;
    
    // Flights Tab
    QWidget *flightsTab = new QWidget;
    setupFlightsTab(flightsTab);
    
    // Aircraft Tab
    QWidget *aircraftTab = new QWidget;
    setupAircraftTab(aircraftTab);
    
    // Reports Tab
    QWidget *reportsTab = new QWidget;
    setupReportsTab(reportsTab);
    
    tabWidget->addTab(flightsTab, "Flights");
    tabWidget->addTab(aircraftTab, "Aircraft");
    tabWidget->addTab(reportsTab, "Reports");
    
    // Logout Button
    QPushButton *logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 8px; border-radius: 4px;");
    
    mainLayout->addWidget(header);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(logoutButton);
    
    connect(logoutButton, &QPushButton::clicked, this, &AdminDashboard::logout);
}

void AdminDashboard::setupFlightsTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);
        
    // Flight Table
    flightsTable = new QTableWidget;
    flightsTable->setColumnCount(8);
    flightsTable->setHorizontalHeaderLabels({"Flight #", "Origin", "Destination", "Departure", "Arrival", "Aircraft", "Status", "Total Seats", "Available Seats"});
    flightsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    flightsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    flightsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Flight Management Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *addFlightButton = new QPushButton("Add Flight");
    QPushButton *editFlightButton = new QPushButton("Edit Flight");
    QPushButton *deleteFlightButton = new QPushButton("Delete Flight");
    
    addFlightButton->setStyleSheet("background-color: #2ecc71; color: white;");
    editFlightButton->setStyleSheet("background-color: #3498db; color: white;");
    deleteFlightButton->setStyleSheet("background-color: #e74c3c; color: white;");
    
    buttonLayout->addWidget(addFlightButton);
    buttonLayout->addWidget(editFlightButton);
    buttonLayout->addWidget(deleteFlightButton);
    
    layout->addWidget(flightsTable);
    layout->addLayout(buttonLayout);
    
    connect(addFlightButton, &QPushButton::clicked, this, &AdminDashboard::showAddFlightDialog);
    connect(editFlightButton, &QPushButton::clicked, this, &AdminDashboard::showEditFlightDialog);
    connect(deleteFlightButton, &QPushButton::clicked, this, &AdminDashboard::deleteFlight);
}


void AdminDashboard::setupAircraftTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);
        
        QLabel *label = new QLabel("Aircraft Management");
        label->setAlignment(Qt::AlignCenter);
        
        // Aircraft Table
        aircraftTable = new QTableWidget;
        aircraftTable->setColumnCount(5);
        aircraftTable->setHorizontalHeaderLabels({"ID", "Model", "Manufacturer", "Capacity", "Status"});
        aircraftTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        
        // Sample data
        QStringList aircraftData = {
            "A001,Boeing 737,Boeing,200,Available",
            "A002,Airbus A320,Airbus,150,Maintenance"
        };
        
        aircraftTable->setRowCount(aircraftData.size());
        for (int i = 0; i < aircraftData.size(); ++i) {
            QStringList parts = aircraftData[i].split(',');
            for (int j = 0; j < parts.size(); ++j) {
                aircraftTable->setItem(i, j, new QTableWidgetItem(parts[j]));
            }
        }
        
        // Aircraft Management Buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *addAircraftButton = new QPushButton("Add Aircraft");
        QPushButton *editAircraftButton = new QPushButton("Edit Aircraft");
        QPushButton *deleteAircraftButton = new QPushButton("Delete Aircraft");
        
        addAircraftButton->setStyleSheet("background-color: #2ecc71; color: white;");
        editAircraftButton->setStyleSheet("background-color: #3498db; color: white;");
        deleteAircraftButton->setStyleSheet("background-color: #e74c3c; color: white;");
        
        buttonLayout->addWidget(addAircraftButton);
        buttonLayout->addWidget(editAircraftButton);
        buttonLayout->addWidget(deleteAircraftButton);
        
        layout->addWidget(label);
        layout->addWidget(aircraftTable);
        layout->addLayout(buttonLayout);


        connect(addAircraftButton, &QPushButton::clicked, this, &AdminDashboard::showAddAircraftDialog);
        connect(editAircraftButton, &QPushButton::clicked, this, &AdminDashboard::showEditAircraftDialog);
        connect(deleteAircraftButton, &QPushButton::clicked, this, &AdminDashboard::deleteAircraft);
        connect(aircraftTable, &QTableWidget::itemClicked, [this](QTableWidgetItem *item) {
            int row = item->row();
            if (row >= 0) {
                aircraftTable->selectRow(row);
            }
        });
}

void AdminDashboard::setupReportsTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    QLabel *label = new QLabel("Operational Reports");
    label->setAlignment(Qt::AlignCenter);
    
    // Report Summary
    QGroupBox *summaryGroup = new QGroupBox("Summary");
    QFormLayout *summaryLayout = new QFormLayout;
    
    totalFlightsLabel = new QLabel("0");
    totalReservationsLabel = new QLabel("0");
    totalRevenueLabel = new QLabel("$0.00");
    
    summaryLayout->addRow("Total Flights:", totalFlightsLabel);
    summaryLayout->addRow("Total Reservations:", totalReservationsLabel);
    summaryLayout->addRow("Total Revenue:", totalRevenueLabel);
    
    summaryGroup->setLayout(summaryLayout);
    
    // Detailed Report
    QGroupBox *detailsGroup = new QGroupBox("Flight Performance");
    QVBoxLayout *detailsLayout = new QVBoxLayout;
    
    reportText = new QTextEdit;
    reportText->setReadOnly(true);
    
    detailsLayout->addWidget(reportText);
    detailsGroup->setLayout(detailsLayout);
    
    // Generate Button
    QPushButton *generateButton = new QPushButton("Generate Report");
    generateButton->setStyleSheet("background-color: #3498db; color: white;");
    
    layout->addWidget(label);
    layout->addWidget(summaryGroup);
    layout->addWidget(detailsGroup);
    layout->addWidget(generateButton);
    
    connect(generateButton, &QPushButton::clicked, this, &AdminDashboard::generateReport);
}

void AdminDashboard::loadFlights()
{
    flightsTable->setRowCount(database.flights.size());
        
    for (size_t i = 0; i < database.flights.size(); ++i) {
        shared_ptr<Flight> flight = database.flights[i];
        
        flightsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(flight->flightNumber)));
        flightsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(flight->origin)));
        flightsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(flight->destination)));
        flightsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(flight->departureDateTime)));
        flightsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(flight->arrivalDateTime)));
        flightsTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(flight->aircraftType)));
        flightsTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(flight->status)));
        flightsTable->setItem(i, 7, new QTableWidgetItem(QString::number(flight->totalSeats)));
        // flightsTable->setItem(i, 8, new QTableWidgetItem(QString::number(flight->seatsAvailable)));

    }
}

void AdminDashboard::showAddFlightDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Flight");
    
    QFormLayout form(&dialog);
    
    QLineEdit *flightNumberEdit = new QLineEdit;
    QLineEdit *originEdit = new QLineEdit;
    QLineEdit *destinationEdit = new QLineEdit;
    QDateTimeEdit *departureEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    QDateTimeEdit *arrivalEdit = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(3600));
    QLineEdit *aircraftEdit = new QLineEdit;
    QSpinBox *seatsEdit = new QSpinBox;
    seatsEdit->setRange(1, 500);
    QComboBox *statusCombo = new QComboBox;
    statusCombo->addItems({"Scheduled", "Delayed", "Canceled"});
    // QSpinBox *seatAvailableEdit = new QSpinBox;
    // seatAvailableEdit->setRange(0, seatsEdit->value());
    // seatAvailableEdit->setMaximum(seatsEdit->value());
    // seatAvailableEdit->setMinimum(0);
    // seatAvailableEdit->setValue(seatsEdit->value());
    
    form.addRow("Flight Number:", flightNumberEdit);
    form.addRow("Origin:", originEdit);
    form.addRow("Destination:", destinationEdit);
    form.addRow("Departure:", departureEdit);
    form.addRow("Arrival:", arrivalEdit);
    form.addRow("Aircraft Type:", aircraftEdit);
    form.addRow("Status:", statusCombo);
    form.addRow("Total Seats:", seatsEdit);
    // form.addRow("Available Seats:", seatAvailableEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        shared_ptr<Flight> flight = make_shared<Flight>(
            flightNumberEdit->text().toStdString(),
            originEdit->text().toStdString(),
            destinationEdit->text().toStdString(),
            departureEdit->dateTime().toString("yyyy-MM-dd hh:mm").toStdString(),
            arrivalEdit->dateTime().toString("yyyy-MM-dd hh:mm").toStdString(),
            aircraftEdit->text().toStdString(),
            statusCombo->currentText().toStdString(),
            seatsEdit->value()
            // seatAvailableEdit->value()

        );
        
        database.addFlight(flight);
        loadFlights();
    }
}

void AdminDashboard::showEditFlightDialog() {
    int row = flightsTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select a flight to edit.");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Flight");
    
    QFormLayout form(&dialog);
    
    QLineEdit *flightNumberEdit = new QLineEdit(flightsTable->item(row, 0)->text());
    QLineEdit *originEdit = new QLineEdit(flightsTable->item(row, 1)->text());
    QLineEdit *destinationEdit = new QLineEdit(flightsTable->item(row, 2)->text());
    
    QDateTime departure = QDateTime::fromString(flightsTable->item(row, 3)->text(), "yyyy-MM-dd hh:mm");
    QDateTimeEdit *departureEdit = new QDateTimeEdit(departure.isValid() ? departure : QDateTime::currentDateTime());
    
    QDateTime arrival = QDateTime::fromString(flightsTable->item(row, 4)->text(), "yyyy-MM-dd hh:mm");
    QDateTimeEdit *arrivalEdit = new QDateTimeEdit(arrival.isValid() ? arrival : QDateTime::currentDateTime().addSecs(3600));
    
    QComboBox *statusCombo = new QComboBox;
    statusCombo->addItems({"Scheduled", "Delayed", "Canceled"});
    statusCombo->setCurrentText(flightsTable->item(row, 7)->text());

    QLineEdit *aircraftEdit = new QLineEdit(flightsTable->item(row, 5)->text());
    QSpinBox *seatsEdit = new QSpinBox;
    seatsEdit->setValue(flightsTable->item(row, 6)->text().toInt());
    seatsEdit->setRange(1, 500);

    // QSpinBox *seatAvailableEdit = new QSpinBox;
    // seatAvailableEdit->setValue(flightsTable->item(row, 8)->text().toInt());
    // seatAvailableEdit->setRange(0, seatsEdit->value());
    // seatAvailableEdit->setMaximum(seatsEdit->value());
    // seatAvailableEdit->setMinimum(0);
    // seatAvailableEdit->setValue(seatsEdit->value() - flightsTable->item(row, 6)->text().toInt());
    
    
    form.addRow("Flight Number:", flightNumberEdit);
    form.addRow("Origin:", originEdit);
    form.addRow("Destination:", destinationEdit);
    form.addRow("Departure:", departureEdit);
    form.addRow("Arrival:", arrivalEdit);
    form.addRow("Aircraft Type:", aircraftEdit);
    form.addRow("Status:", statusCombo);
    form.addRow("Total Seats:", seatsEdit);
    // form.addRow("Available Seats:", seatAvailableEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Update the flight in the database
        database.flights[row]->flightNumber = flightNumberEdit->text().toStdString();
        database.flights[row]->origin = originEdit->text().toStdString();
        database.flights[row]->destination = destinationEdit->text().toStdString();
        database.flights[row]->departureDateTime = departureEdit->dateTime().toString("yyyy-MM-dd hh:mm").toStdString();
        database.flights[row]->arrivalDateTime = arrivalEdit->dateTime().toString("yyyy-MM-dd hh:mm").toStdString();
        database.flights[row]->aircraftType = aircraftEdit->text().toStdString();
        database.flights[row]->status = statusCombo->currentText().toStdString();
        database.flights[row]->totalSeats = seatsEdit->value();
        // database.flights[row]->seatsAvailable = seatAvailableEdit->value();
        // database.flights[row]->seatsAvailable = seatsEdit->value() - seatAvailableEdit->value();
        
        database.saveFlights();
        loadFlights();
    }
}

void AdminDashboard::deleteFlight() {
    int row = flightsTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select a flight to delete.");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm", 
                            "Are you sure you want to delete this flight?",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        database.flights.erase(database.flights.begin() + row);
        database.saveFlights();
        loadFlights();
    }
}

void AdminDashboard::loadAircrafts() {
    aircraftTable->setRowCount(database.aircrafts.size());
    
    for (size_t i = 0; i < database.aircrafts.size(); ++i) {
        shared_ptr<Aircraft> aircraft = database.aircrafts[i];
        
        aircraftTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(aircraft->aircraftID)));
        aircraftTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(aircraft->model)));
        aircraftTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(aircraft->manufacturer)));
        aircraftTable->setItem(i, 3, new QTableWidgetItem(QString::number(aircraft->capacity)));
        aircraftTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(aircraft->status)));
    }
}


void AdminDashboard::showAddAircraftDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Aircraft");
    
    QFormLayout form(&dialog);
    
    QLineEdit *aircraftIDEdit = new QLineEdit;
    QLineEdit *modelEdit = new QLineEdit;
    QLineEdit *manufacturerEdit = new QLineEdit;
    
    QSpinBox *capacityEdit = new QSpinBox;
    capacityEdit->setRange(1, 500);
    
    QComboBox *statusCombo = new QComboBox;
    statusCombo->addItems({"Available", "Maintenance", "Retired"});
    
    form.addRow("Aircraft ID:", aircraftIDEdit);
    form.addRow("Model:", modelEdit);
    form.addRow("Manufacturer:", manufacturerEdit);
    form.addRow("Capacity:", capacityEdit);
    form.addRow("Status:", statusCombo);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        shared_ptr<Aircraft> aircraft = make_shared<Aircraft>(
            aircraftIDEdit->text().toStdString(),
            modelEdit->text().toStdString(),
            manufacturerEdit->text().toStdString(),
            capacityEdit->value(),
            statusCombo->currentText().toStdString()
        );
        
        database.addAircraft(aircraft);
        loadAircrafts();
    }
}


void AdminDashboard::showEditAircraftDialog() {
    int row = aircraftTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select an aircraft to edit.");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Aircraft");
    
    QFormLayout form(&dialog);
    
    QLineEdit *aircraftIDEdit = new QLineEdit(aircraftTable->item(row, 0)->text());
    QLineEdit *modelEdit = new QLineEdit(aircraftTable->item(row, 1)->text());
    QLineEdit *manufacturerEdit = new QLineEdit(aircraftTable->item(row, 2)->text());
    
    QSpinBox *capacityEdit = new QSpinBox;
    capacityEdit->setValue(aircraftTable->item(row, 3)->text().toInt());
    capacityEdit->setRange(1, 500);
    
    QComboBox *statusCombo = new QComboBox;
    statusCombo->addItems({"Available", "Maintenance", "Retired"});
    statusCombo->setCurrentText(aircraftTable->item(row, 4)->text());
    
    form.addRow("Aircraft ID:", aircraftIDEdit);
    form.addRow("Model:", modelEdit);
    form.addRow("Manufacturer:", manufacturerEdit);
    form.addRow("Capacity:", capacityEdit);
    form.addRow("Status:", statusCombo);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Update the aircraft in the database
        database.aircrafts[row]->aircraftID = aircraftIDEdit->text().toStdString();
        database.aircrafts[row]->model = modelEdit->text().toStdString();
        database.aircrafts[row]->manufacturer = manufacturerEdit->text().toStdString();
        database.aircrafts[row]->capacity = capacityEdit->value();
        database.aircrafts[row]->status = statusCombo->currentText().toStdString();
        database.saveAircrafts();
        loadAircrafts();
    }
}

void AdminDashboard::deleteAircraft() {
    int row = aircraftTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Error", "Please select an aircraft to delete.");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm", 
                            "Are you sure you want to delete this aircraft?",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        database.aircrafts.erase(database.aircrafts.begin() + row);
        database.saveAircrafts();
        loadAircrafts();
    }
}


void AdminDashboard::generateReport() {
    int totalFlights = database.flights.size();
    int totalReservations = database.reservations.size();
    double totalRevenue = 0.0;
    
    for (auto& r : database.reservations) {
        totalRevenue += r->totalCost;
    }
    
    totalFlightsLabel->setText(QString::number(totalFlights));
    totalReservationsLabel->setText(QString::number(totalReservations));
    totalRevenueLabel->setText(QString("$%1").arg(totalRevenue, 0, 'f', 2));
    
    QString reportTextContent;
    reportTextContent += "Detailed Flight Performance:\n\n";
    
    for (auto& f : database.flights) {
        int bookings = 0;
        double flightRevenue = 0.0;
        
        for (auto& r : database.reservations) {
            if (r->flightNumber == f->flightNumber) {
                bookings++;
                flightRevenue += r->totalCost;
            }
        }
        
        reportTextContent += QString("Flight %1 - %2 bookings, Revenue: $%3\n")
            .arg(QString::fromStdString(f->flightNumber))
            .arg(bookings)
            .arg(flightRevenue, 0, 'f', 2);
    }
    
    reportText->setPlainText(reportTextContent);
}

void AdminDashboard::logout() {
    emit logoutRequested();
}