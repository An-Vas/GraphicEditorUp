#include <QApplication>
#include "Design/MainWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWindow widget;
    widget.setBaseSize(2000, 1800);
    widget.show();

    return app.exec();
}
