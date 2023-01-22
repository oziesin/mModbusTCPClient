#ifndef COMSETTINGS_H
#define COMSETTINGS_H

#include <QWidget>
#include <QDebug>
#include <QModbusTcpClient>
#include <QUrl>

namespace Ui {
  class ComSettings;
}

class ComSettings : public QWidget
{
  Q_OBJECT

public:
  explicit ComSettings(QModbusTcpClient &rTcpClient, QWidget *parent = nullptr);

  ~ComSettings();

private slots:
  void onModbusStateChanged(int state);

  void on_Pb_Connect_clicked();

  void on_Le_Addr_editingFinished();

private:
  Ui::ComSettings *ui;

  QModbusTcpClient *pModbusDevice;

  bool TcpStatus{false};
  QString Load(const QString &Key);
  void Save( const QString &Key, const QString &Data);
  bool FormatCheck( const QString &Ip );
  bool FormatCheck( const int &Port)
  {
    return ( (Port > 0) && (Port < 65535) ) ? true : false;
  }
};

#endif // COMSETTINGS_H
