#include "Modbus.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>

Modbus::Modbus() {
  content = "";
  serialTimer = nullptr;
}

void Modbus::startConnection() {
  client = new QModbusRtuSerialMaster();


  std::ifstream inTino;
  std::string home = std::getenv("HOME");
  inTino.open(home + "/.tino/clientPort.txt");
  std::string portnameString;
  getline(inTino, portnameString);

  QString qportnameString = QString::fromStdString(portnameString);
  client->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                 qportnameString);

//  client->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
//                                 "/dev/pts/2");

  client->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                 QSerialPort::NoParity);
  client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                 QSerialPort::Baud57600);
  client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                 QSerialPort::Data8);
  client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                 QSerialPort::OneStop);

  if (!client->connectDevice())
    qDebug() << "cannot connect ";
  else
    qDebug() << "connect ";

  qDebug() << "error: " << client->errorString();
  qDebug() << "state: " << client->state();
}

void Modbus::startReading() {
  if (serialTimer) {
    serialTimer->stop();
    delete serialTimer;
    serialTimer = nullptr;
    qDebug() << "stopped reading";
  } else {
    serialTimer = new QTimer();
    connect(serialTimer, &QTimer::timeout, this, &Modbus::query);
    serialTimer->start(70);
    qDebug() << "started reading";
  }
}

void Modbus::query() {
  QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, startAddress,
                           static_cast<unsigned short>(nBytes));

  if (auto *reply = client->sendReadRequest(readUnit, 1)) {
    if (!reply->isFinished())
      //      connect the finished signal of the request to your read slot
      connect(reply, &QModbusReply::finished, this, &Modbus::readready);
    //    else
    //      delete reply; // broadcast replies return immediately
  } else
    qDebug() << "request error";
}

void Modbus::readready() const {
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();
  qDebug() << "read ";
  qDebug() << "";

  for (int j = 0; j < nBytes; j++)
    qDebug() << QString("The value of %1 is %2").arg(j).arg(result.value(j));
}

void Modbus::endConnection() {
  if (serialTimer) {
    serialTimer->stop();
    delete serialTimer;
    serialTimer = nullptr;
  }

  client->disconnectDevice();
  delete client;
  client = nullptr;

  qDebug() << "connection closed";
}

QString Modbus::getContent() { return content; }

Modbus::~Modbus() {
  client->disconnectDevice();
  delete client;
  client = nullptr;
}

void Modbus::setNnbytes(int n) { nBytes = n; }

void Modbus::setStartAddress(int n) { startAddress = n; }
