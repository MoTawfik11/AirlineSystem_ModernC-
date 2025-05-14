#include "mainwindow.h"
#include "loginwindow.h"
#include "admindashboard.h"
#include "agentdashboard.h"
#include "passengerdashboard.h"
#include "systemdatabase.h"
#include "flight.h"
#include "aircraft.h"
#include <QStackedWidget>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Airline Management System");
    setMinimumSize(800, 600);
    
    // Apply a modern style
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, QColor(255, 255, 255));
    palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    QApplication::setPalette(palette);
    
    // Stacked widget for different views
    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);
    
    // Create and add widgets
    loginWindow = new LoginWindow;
    adminDashboard = new AdminDashboard;
    agentDashboard = new AgentDashboard;
    passengerDashboard = new PassengerDashboard;
    
    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(adminDashboard);
    stackedWidget->addWidget(agentDashboard);
    stackedWidget->addWidget(passengerDashboard);
    
    // Connect signals
    connect(loginWindow, &LoginWindow::adminLoginRequested, this, [this]() {
        stackedWidget->setCurrentWidget(adminDashboard);
    });
    
    connect(loginWindow, &LoginWindow::agentLoginRequested, this, [this]() {
        stackedWidget->setCurrentWidget(agentDashboard);
    });
    
    connect(loginWindow, &LoginWindow::passengerLoginRequested, this, [this]() {
        stackedWidget->setCurrentWidget(passengerDashboard);
    });
    
    connect(adminDashboard, &AdminDashboard::logoutRequested, this, [this]() {
        stackedWidget->setCurrentWidget(loginWindow);
    });
    
    connect(agentDashboard, &AgentDashboard::logoutRequested, this, [this]() {
        stackedWidget->setCurrentWidget(loginWindow);
    });
    
    connect(passengerDashboard, &PassengerDashboard::logoutRequested, this, [this]() {
        stackedWidget->setCurrentWidget(loginWindow);
    });
}
