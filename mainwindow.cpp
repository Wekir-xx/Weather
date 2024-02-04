#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrl>
#include <QTime>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon (QIcon(":/images/icon.jpg"));
    this->setWindowTitle("Weather");
    this->setFixedSize (375,500);
    ui->time->setText ("<html><head/><body><p align=\"center\"><span style=\" font-size:20pt; font-weight:700;\">" +
                       QTime::currentTime().toString("hh:mm") + "</span></p></body></html>");

    QBrush brush;
    brush.setTextureImage (QImage(":/images/sky.jpg").scaled (375,500));
    QPalette palete;
    palete.setBrush (QPalette::Window, brush);
    this->setPalette(palete);

    API_GEO = new QNetworkAccessManager(this);
    connect(API_GEO, SIGNAL(finished(QNetworkReply*)), this, SLOT(getGeo(QNetworkReply*)));
    API_GEO->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    temp = QString::number(obj["wind_kph"].toInt());
    ui->speed->setText ("<html><head/><body><p align=\"center\"><span style=\" font-family:\'Courier New\'; font-size:11pt; font-weight:700; color:#000000;\">"
                        + temp + " km/h</span></p></body></html>");
    val = obj.value ("condition");
    obj = val.toObject ();
    ui->condition->setText ("<html><head/><body><p><span style=\" font-family:\'Courier New\'; font-size:11pt; font-weight:700; color:#000000;\">"
                       + obj["text"].toString () + "</span></p></body></html>");

    if(obj["text"].toString() == "Partly cloudy")
        ui->picture->setPixmap (QPixmap(":/images/partly_cloudy.png"));
    ui->picture->setScaledContents (true);

    reply->deleteLater ();
    this->show ();
}
