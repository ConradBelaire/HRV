#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // For testing purposes!
    bool redLED;
    bool greenLED;
    bool blueLED;


private slots:
    void toggleRedLED();
    void toggleGreenLED();
    void toggleBlueLED();
};
#endif // MAINWINDOW_H
