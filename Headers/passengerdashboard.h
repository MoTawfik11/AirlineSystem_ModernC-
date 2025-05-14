#ifndef PASSENGERDASHBOARD_H
#define PASSENGERDASHBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTableWidget>
#include <QPushButton>
#include "systemdatabase.h"

class PassengerDashboard : public QWidget {
    Q_OBJECT
public:
    explicit PassengerDashboard(QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void searchFlights();
    void checkIn();
    void logout();

private:
    void setupUI();
    void setupSearchTab(QWidget *tab);
    void setupReservationsTab(QWidget *tab);
    void updateReservationsTable();

    QLineEdit *originEdit;
    QLineEdit *destinationEdit;
    QDateEdit *dateEdit;
    QTableWidget *resultsTable;
    QTableWidget *reservationsTable;
    SystemDatabase database;
};

#endif // PASSENGERDASHBOARD_H