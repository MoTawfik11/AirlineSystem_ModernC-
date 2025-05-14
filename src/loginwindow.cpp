#include "loginwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void LoginWindow::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Header
    QLabel *header = new QLabel("Airline Management System");
    header->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50;");
    header->setAlignment(Qt::AlignCenter);
    
    // Login Form
    QGroupBox *loginGroup = new QGroupBox("Login");
    loginGroup->setStyleSheet("QGroupBox { font-weight: bold; }");
    
    QFormLayout *formLayout = new QFormLayout;
    
    userTypeCombo = new QComboBox;
    userTypeCombo->addItems({"Administrator", "Booking Agent", "Passenger"});
    
    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Enter username");
    
    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Enter password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    
    passengerIdEdit = new QLineEdit;
    passengerIdEdit->setPlaceholderText("Enter Passenger ID");
    passengerIdEdit->setVisible(false);
    
    formLayout->addRow(new QLabel("User Type:"), userTypeCombo);
    formLayout->addRow(new QLabel("Username:"), usernameEdit);
    formLayout->addRow(new QLabel("Password:"), passwordEdit);
    formLayout->addRow(new QLabel("Passenger ID:"), passengerIdEdit);
    
    loginGroup->setLayout(formLayout);
    
    // Login Button
    QPushButton *loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("background-color: #3498db; color: white; padding: 8px; border-radius: 4px;");
    
    // Footer
    QLabel *footer = new QLabel("© 2025 Airline Management System");
    footer->setStyleSheet("color: #7f8c8d;");
    footer->setAlignment(Qt::AlignCenter);
    
    // Add widgets to layout
    layout->addWidget(header);
    layout->addSpacing(20);
    layout->addWidget(loginGroup);
    layout->addWidget(loginButton);
    layout->addStretch();
    layout->addWidget(footer);
    
    // Connections
    connect(userTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        passengerIdEdit->setVisible(index == 2); // Show only for passenger
    });
    
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
}

void LoginWindow::attemptLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    QString passengerId = passengerIdEdit->text();
    int userType = userTypeCombo->currentIndex();
    
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password are required!");
        return;
    }
    
    if (userType == 2 && passengerId.isEmpty()) { // Passenger
        QMessageBox::warning(this, "Error", "Passenger ID is required!");
        return;
    }
    
    // Simplified authentication for GUI demo
    if (userType == 0) { // Admin
        if (username == "admin" && password == "admin123") {
            emit adminLoginRequested();
        } else {
            QMessageBox::warning(this, "Error", "Invalid admin credentials!");
        }
    } else if (userType == 1) { // Agent
        if (username == "agent" && password == "agent123") {
            emit agentLoginRequested();
        } else {
            QMessageBox::warning(this, "Error", "Invalid agent credentials!");
        }
    } else if (userType == 2) { // Passenger
    if (username != "passenger" || password != "passenger123") {
        QMessageBox::warning(this, "Error", "Invalid passenger credentials!");
        return;
    }

    QFile file("reservations.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to open reservation.json");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        QMessageBox::critical(this, "Error", "Invalid reservation.json format");
        return;
    }

    QJsonArray reservations = doc.array();
    bool idFound = false;

    for (const QJsonValue &value : reservations) {
        QJsonObject obj = value.toObject();
        if (obj["passengerID"].toString() == passengerId) {
            idFound = true;
            break;
        }
    }

    if (idFound) {
        emit passengerLoginRequested();
    } else {
        QMessageBox::warning(this, "Error", "Passenger ID not found in reservations!");
    }
}

}