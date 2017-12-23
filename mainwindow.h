#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nican_period.h"
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    NICAN_Period *nican1=new NICAN_Period;
    NCTYPE_CAN_STRUCT sendData;

    void showResponse(NCTYPE_CAN_STRUCT_P *p);
    void processError(const QString &s);
    void sendFrames();

signals:
    void triggedSend(NCTYPE_CAN_STRUCT sendData);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
