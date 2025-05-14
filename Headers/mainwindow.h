#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class LoginWindow;
class AdminDashboard;
class AgentDashboard;
class PassengerDashboard;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    LoginWindow *loginWindow;
    AdminDashboard *adminDashboard;
    AgentDashboard *agentDashboard;
    PassengerDashboard *passengerDashboard;
};

#endif // MAINWINDOW_H
