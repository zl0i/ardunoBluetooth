#ifndef ARDBL_H
#define ARDBL_H

#include <QObject>
#include <QtQml>
#include <QStringListModel>


#include <QBluetoothLocalDevice>
#include <QBluetoothServer>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>


#include <QDebug>
#include <QRegExp>





class arduinoBL : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString msg READ ReadMsg WRITE WriteMsg NOTIFY msgChanged)
    Q_PROPERTY(QStringListModel *listDevice_model READ ReadListDevice_model NOTIFY listDeviceChanged)
    Q_PROPERTY(bool isHC05 READ getIsHC05 NOTIFY ishc05Changed)

    Q_PROPERTY(QString temperature READ getTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString pressure READ getPressure NOTIFY pressureChanged)


    QString msg;
    QStringList listDevice;
    QString temperature;
    QString pressure;

    QBluetoothServer *rfcommServer;
    QBluetoothSocket *soc;
    QBluetoothDeviceInfo hc05;
    QBluetoothLocalDevice localDevice;

    QStringListModel *listDevice_model;
    bool isHC05 = false;
public:
    explicit arduinoBL(/*QQmlApplicationEngine *eng, */QObject *parent = nullptr);

    void WriteMsg(QString &str);
    QString ReadMsg();

    QStringListModel *ReadListDevice_model();

    bool getIsHC05();

    QString getTemperature();
    QString getPressure();



    Q_INVOKABLE void startSearh();
    Q_INVOKABLE void connectHC05();
    Q_INVOKABLE void onHC05();
    Q_INVOKABLE void offHC05();

signals:
    void msgChanged();
    void listDeviceChanged();
    void ishc05Changed();
    void temperatureChanged();
    void pressureChanged();
public slots:
    void NewDeviceSearch(const QBluetoothDeviceInfo &info);
    void finishedSearch();
    void ReadHC05();
    void stateHC05Changed(QBluetoothSocket::SocketState state);

};

#endif // ARDBL_H
