#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void adminLoginRequested();
    void agentLoginRequested();
    void passengerLoginRequested();

private slots:
    void attemptLogin();

private:
    void setupUI();
    
    QComboBox *userTypeCombo;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *passengerIdEdit;
};

#endif // LOGINWINDOW_H