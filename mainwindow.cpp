#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrl>
#include <QTime>
#include <QPixmap>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon (QIcon(":/images/icon.jpg"));
    this->setWindowTitle("Weather");
    this->setFixedSize (375,500);

    QBrush brush;
    brush.setTextureImage (QImage(":/images/sky.jpg").scaled (375,500));
    QPalette palete;
    palete.setBrush (QPalette::Window, brush);
    this->setPalette(palete);

    QTimer* timer = new QTimer(this);
    timer->setInterval(10000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateWeather()));
    updateWeather();
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWeather()
{
    ui->time->setText ("<html><head/><body><p align=\"center\"><span style=\" font-size:20pt; font-weight:700;\">" +
                      QTime::currentTime().toString("hh:mm") + "</span></p></body></html>");

    API_GEO = new QNetworkAccessManager(this);
    connect(API_GEO, SIGNAL(finished(QNetworkReply*)), this, SLOT(getGeo(QNetworkReply*)));
    API_GEO->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
}

void MainWindow::getGeo(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object ();

    API_WEATHER = new QNetworkAccessManager(this);
    connect(API_WEATHER, SIGNAL(finished(QNetworkReply*)), this, SLOT(getWeather(QNetworkReply*)));
    API_WEATHER->get(QNetworkRequest(QUrl(QString("https://api.weatherapi.com/v1/current.json?key=da0fd75db051489d85d121457240302&q=%1")
                                              .arg (obj["city"].toString()))));

    ui->city->setText ("<html><head/><body><p align=\"center\"><span style=\" font-size:23pt; font-weight:700;\">" +
                       obj["city"].toString() + "</span></p></body></html>");

    reply->deleteLater ();
}

void MainWindow::getWeather(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object ();

    QJsonValue val = obj.value("current");
    obj = val.toObject ();
    QString temp = QString::number(obj["temp_c"].toInt());

    ui->temperature->setText ("<html><head/><body><p align=\"center\"><span style=\" font-size:34pt;\">" + temp
                              + " C</span><span style=\" font-size:34pt; vertical-align:super;\">0</span></p></body></html>");

    temp = QString::number(int(obj["wind_kph"].toDouble()));

    ui->speed->setText ("<html><head/><body><p align=\"center\"><span style=\" font-family:\'Courier New\'; font-size:11pt; font-weight:700; color:#000000;\">"
                        + temp + " km/h</span></p></body></html>");

    val = obj.value ("condition");
    obj = val.toObject ();

    ui->condition->setText ("<html><head/><body><p><span style=\" font-family:\'Courier New\'; font-size:11pt; font-weight:700; color:#000000;\">"
                       + obj["text"].toString () + "</span></p></body></html>");

    QString time_day;

    if(QTime::currentTime().toString("h").toInt() > 8 && QTime::currentTime().toString("h").toInt() < 23)
        time_day = "day/";
    else
        time_day = "night/";

    if(obj["text"].toString() == "Partly cloudy")
        time_day += "116.png";
    else if(obj["text"].toString() == "Light rain")
        time_day += "296.png";

    ui->picture->setPixmap (QPixmap(QString(":/images/weather/%1").arg(time_day)));
    ui->picture->setScaledContents (true);

    reply->deleteLater ();
}
