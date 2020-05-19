#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <rr232manager.h>
#include <QTimer>

class RS232SyncSender;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_buttons2_clicked();

    void on_pushButton_clicked();



private:
    Ui::MainWindow *ui;
    QTimer * timer;

    int buttonNumber;
    int requestNumber = 0;

    RS232SyncSender * syncSender;

public slots:
    void getData(QByteArray data);
    void sendTimedCommand();



};
#endif // MAINWINDOW_H
