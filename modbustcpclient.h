#ifndef MODBUSTCPCLIENT_H
#define MODBUSTCPCLIENT_H

#include <QMainWindow>
#include <QObject>
#include "comsettings.h"
#include "registermodel.h"
#include <QtSerialBus/qmodbustcpclient.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ModbusTcpClient; }
QT_END_NAMESPACE

class ModbusTcpClient : public QMainWindow
{
  Q_OBJECT

public:
  ModbusTcpClient(QWidget *parent = nullptr);
  ~ModbusTcpClient();


private slots:
  void triggered_CommSetting(bool checked = false)
  {
    pCommSetttings->show();
  }
  void onModbusStateChanged(int state);
  void onReadReady();

  void on_pushButton_Send_clicked();

  void onShowTypeChanged(int index);
  void on_spinBox_RegCount_editingFinished();

private:
  Ui::ModbusTcpClient *ui;

  RegisterModel *regModel;
  QModbusTcpClient *pModbusDevice;
  ComSettings *pCommSetttings{nullptr};
  QModbusDataUnit Prepare_Request() const;

};
#endif // MODBUSTCPCLIENT_H
