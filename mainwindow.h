#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void getGeo(QNetworkReply *reply);
    void getWeather(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *API_GEO, *API_WEATHER;
};
#endif // MAINWINDOW_H
