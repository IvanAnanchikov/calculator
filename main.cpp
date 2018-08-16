#include "calculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator calculator;
    calculator.setWindowTitle("Simple calculator");
    calculator.setFixedSize(250,350);
    calculator.show();
    return a.exec();
}
