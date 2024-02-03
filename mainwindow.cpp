#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    API_GEO = new QNetworkAccessManager(this);
    connect(API_GEO, SIGNAL(finished(QNetworkReply*)), this, SLOT(getGeo(QNetworkReply*)));
    API_GEO->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete API_GEO;
    delete API_WEATHER;
}

void MainWindow::getGeo(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object ();

    API_WEATHER = new QNetworkAccessManager(this);
    connect(API_WEATHER, SIGNAL(finished(QNetworkReply*)), this, SLOT(getWeather(QNetworkReply*)));
    API_WEATHER->get(QNetworkRequest(QUrl(QString("https://api.weatherapi.com/v1/current.json?key=da0fd75db051489d85d121457240302&q=%1")
                                              .arg (doc["city"].toString())))); //lang=ru
    reply->deleteLater ();
}

void MainWindow::getWeather(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object ();
    reply->deleteLater ();
}
