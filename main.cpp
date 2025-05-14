#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QFont font("Segoe UI", 10);
    app.setFont(font);
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
