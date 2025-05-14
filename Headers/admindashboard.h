#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include "systemdatabase.h"

class QTableWidget;
class QLabel;
class QTextEdit;
class QTabWidget;
class QPushButton;
class QDialog;
class QDateTimeEdit;

class AdminDashboard : public QWidget {
    Q_OBJECT
public:
    explicit AdminDashboard(QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void logout();


private:
    void setupUI();
    void setupFlightsTab(QWidget *tab);
    void setupAircraftTab(QWidget *tab);
    void setupReportsTab(QWidget *tab);
    void loadFlights();
    void showAddFlightDialog();
    void showEditFlightDialog();
    void deleteFlight();
    void loadAircrafts();
    void showAddAircraftDialog();
    void showEditAircraftDialog();
    void deleteAircraft();
    void generateReport();


    QTableWidget *flightsTable;
    QTableWidget *aircraftTable;
    QTabWidget *tabWidget;
    QPushButton *addFlightButton;
    QPushButton *editFlightButton;
    QPushButton *deleteFlightButton;
    QPushButton *addAircraftButton;
    QPushButton *editAircraftButton;
    QPushButton *deleteAircraftButton;
    QLabel *totalFlightsLabel;
    QLabel *totalReservationsLabel;
    QLabel *totalRevenueLabel;

    QTextEdit *reportText;
    SystemDatabase database;
};

#endif // ADMINDASHBOARD_H