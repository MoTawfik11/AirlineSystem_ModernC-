#ifndef AGENTDASHBOARD_H
#define AGENTDASHBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QTextEdit>
#include "systemdatabase.h"

class AgentDashboard : public QWidget {
    Q_OBJECT
public:
    explicit AgentDashboard(QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:

    void logout();

private:
    void setupUI();
    void setupSearchTab(QWidget *tab);
    void setupReservationsTab(QWidget *tab);
    void SearchFlights();
    void bookFlight();
    void updateReservationsTable();
    void modifyReservation();
    void cancelReservation();

    QLineEdit *originEdit;
    QLineEdit *destinationEdit;
    QDateEdit *dateEdit;
    QPushButton *searchButton;
    QPushButton *bookButton;
    QPushButton *modifyButton;
    QPushButton *cancelButton;
    QPushButton *refreshButton;
    QTableWidget *resultsTable;
    QTableWidget *reservationsTable;
    SystemDatabase database;

    QTableWidget *flightsTable;
    QTableWidget *aircraftTable;
    QLabel *totalFlightsLabel;
    QLabel *totalReservationsLabel;
    QLabel *totalRevenueLabel;
    QTextEdit *reportText;
};

#endif // AGENTDASHBOARD_H