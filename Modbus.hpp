#ifndef MODBUS_HPP
#define MODBUS_HPP

#include <QDebug>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QModbusRtuSerialMaster>
#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

class rpl : public QObject {};

class Modbus : public QObject {
  Q_OBJECT
public:
  Modbus();
  ~Modbus();

public slots:
  void startConnection();
  void endConnection();
  void startReading();
  void setNnbytes(int n);
  void setStartAddress(int n);
  QString getContent();

private:
  QModbusRtuSerialMaster *client;
  QString content;
  int nBytes;
  int startAddress;
  QTimer *serialTimer;

  void query();
  void readready() const;
};

#endif // MODBUS_HPP
