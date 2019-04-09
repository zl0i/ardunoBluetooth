#include "ardbl.h"

arduinoBL::arduinoBL(/*QQmlApplicationEngine *eng, */QObject *parent) : QObject(parent)
{

    //pcon = eng->rootContext();
    listDevice_model = new QStringListModel(this);
    msg = "Error";
    emit msgChanged();
    isHC05 = false;
    emit ishc05Changed();
    temperature = "nan";
    pressure = "nan";
    emit temperatureChanged();
    emit pressureChanged();



    soc = new QBluetoothSocket();


    QString localDeviceName;

    // Check if Bluetooth is available on this device
    if (localDevice.isValid()) {
        //mylbl->setProperty("text", "Ok");
        // Turn Bluetooth on
        msg = "Ok";
        emit msgChanged();
        localDevice.powerOn();

        // Read local device name
        localDeviceName = localDevice.name();

        // Make it visible to others
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        // Get connected devices
        QList<QBluetoothAddress> remotes;
        remotes = localDevice.connectedDevices();

        QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
        connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(NewDeviceSearch(QBluetoothDeviceInfo)));
        connect(discoveryAgent, SIGNAL(finished()), this, SLOT(finishedSearch()));

        // Start a discovery
        discoveryAgent->start();
        msg = "Search";
        emit msgChanged();
    }


}

void arduinoBL::startSearh() {
    isHC05 = false;
    emit ishc05Changed();
    if(localDevice.isValid()) {
        if(soc->state() == QBluetoothSocket::ConnectedState) {
            soc->close();
            msg = "Close";
            emit msgChanged();
        }
        listDevice.clear();
        listDevice_model->setStringList(listDevice);
        emit listDeviceChanged();

        QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
        discoveryAgent->stop();
        connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(NewDeviceSearch(QBluetoothDeviceInfo)));
        connect(discoveryAgent, SIGNAL(finished()), this, SLOT(finishedSearch()));

        // Start a discovery
        discoveryAgent->start();
        msg = "Search";
        emit msgChanged();
    }
    else {
        msg = "Error";
        emit msgChanged();
    }
}

void arduinoBL::NewDeviceSearch(const QBluetoothDeviceInfo &info) {
    listDevice << info.name()+ " " + info.address().toString();
    listDevice_model->setStringList(listDevice);
    emit listDeviceChanged();
    if(info.name() == "HC-05") {
        hc05 = info;
        isHC05 = true;
        emit ishc05Changed();
    }
}

void arduinoBL::finishedSearch() {
    msg = "all = " + QString::number(listDevice.size());
    emit msgChanged();
    if(listDevice.size() == 0) listDevice << "Not Found";
    listDevice_model->setStringList(listDevice);
    emit listDeviceChanged();
}

void arduinoBL::connectHC05() {
    if(!isHC05) return;
    static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
    soc->connectToService(hc05.address(), QBluetoothUuid(serviceUuid), QIODevice::ReadWrite);

    connect(soc, SIGNAL(stateChanged(QBluetoothSocket::SocketState)), this, SLOT(stateHC05Changed(QBluetoothSocket::SocketState)));
    msg = "Connecting...";
    emit msgChanged();
}

void arduinoBL::onHC05() {
    soc->write("n");
}

void arduinoBL::offHC05() {
    soc->write("f");
}

void arduinoBL::ReadHC05() {
    QByteArray arr = soc->readAll();
    QString buf = QString::fromLocal8Bit(arr);
    if(buf.size() > 5) {
        temperature = buf;
        emit temperatureChanged();
    }
    /*QStringList list;
    list = buf.split('\n');
    if(list.size() >= 1) {
        temperature = list.at(0);
        pressure = list.at(1);
        emit pressureChanged();
        emit temperatureChanged();
    }
    if(list.size() >= 2) {

    }*/
    /*msg = buf;
    emit msgChanged();*/

}

void arduinoBL::stateHC05Changed(QBluetoothSocket::SocketState state) {
    switch (state) {
    case QBluetoothSocket::ConnectedState:
        msg = "Connected";
        connect(soc, SIGNAL(readyRead()), this, SLOT(ReadHC05()));
        break;
    default:
        msg = "Error";
        break;
    }
    emit msgChanged();
}

void arduinoBL::WriteMsg(QString &str) {
    msg = str;
}

QString arduinoBL::ReadMsg() {
    return msg;
}

QStringListModel *arduinoBL::ReadListDevice_model()  {
    return listDevice_model;
}

bool arduinoBL::getIsHC05() {
    return isHC05;
}

QString arduinoBL::getPressure() {
    return pressure;
}

QString arduinoBL::getTemperature() {
    return temperature;
}
