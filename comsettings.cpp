#include "comsettings.h"
#include "ui_comsettings.h"
#include <QDebug>
#include <QSettings>

ComSettings::ComSettings(QModbusTcpClient &rTcpClient, QWidget *parent) : QWidget(parent), ui(new Ui::ComSettings), pModbusDevice(&rTcpClient)
{
  ui->setupUi(this);

  auto text = Load("Settings");
  ui->Le_Addr->setText( text );

  Q_ASSERT(pModbusDevice != nullptr);
  connect(pModbusDevice, &QModbusClient::stateChanged,this, &ComSettings::onModbusStateChanged);
}


ComSettings::~ComSettings()
{
  delete ui;
}

void ComSettings::onModbusStateChanged(int state)
{
//  qDebug() << "State:" << state << "\n";

  switch (state)
  {
    case QModbusClient::UnconnectedState:
    {
      ui->Pb_Connect->setText("Connect");
      ui->Le_Addr->setDisabled(false);
      ui->label->setStyleSheet("background-color:red\n");
    }
    break;

    case QModbusClient::ConnectedState:
    {
      ui->Pb_Connect->setText("Disconnect");
      ui->Le_Addr->setDisabled(true);
      ui->label->setStyleSheet("background-color:green\n");
    }
    break;
  }
}
void ComSettings::on_Pb_Connect_clicked()
{
  if (pModbusDevice->state() != QModbusDevice::ConnectedState)
  {
    const QUrl url = QUrl::fromUserInput(ui->Le_Addr->text());
    qDebug() << "Ip:" << url.host() << "\tPort:" << url.port() << "\n";
    pModbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
    pModbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

#warning "This 2 parameters can be changeable by user"
    pModbusDevice->setTimeout(1000);
    pModbusDevice->setNumberOfRetries(0);

    if (!pModbusDevice->connectDevice())
      qDebug() << "Connect failed: " + pModbusDevice->errorString() << "\n";
    else
      qDebug() << "Connectted Succesfully\n";
  }
  else
  {
     pModbusDevice->disconnectDevice();
     qDebug() << "Disconnected\n";
  }
}


void ComSettings::on_Le_Addr_editingFinished()
{
  auto text = ui->Le_Addr->text();
  auto Ip_and_Port = text.split(":");

  if( ( Ip_and_Port.size() != 2 ) || ( FormatCheck(Ip_and_Port[0]) == false ) || ( FormatCheck(Ip_and_Port[1].toInt()) == false ) )
  {
    ui->Le_Addr->setText(Load("Settings"));
  }
  else
  {
    Save( "Settings",text );
    }
}

QString ComSettings::Load( const QString &Key )
{
  QSettings settings{"Self","Modbus"};
  settings.beginGroup("Commsettings");
  auto get_val = settings.value(Key).toString();
  settings.endGroup();

  return get_val;
}

void ComSettings::Save( const QString &Key, const QString &Data )
{
  QSettings settings{"Self","Modbus"};
  settings.beginGroup("Commsettings");
  settings.setValue(Key,Data);
  settings.endGroup();
}

bool ComSettings::FormatCheck(const QString &Ip)
{
  auto ipList = Ip.split('.');
  if ( ipList.size() < 4 )
  {
    return false;
  }
//  qDebug() << ipList << "Size: " << ipList.size() << "\n";

  for ( auto iter : ipList )
  {
    if( ( iter != "0" ) && ( iter.toInt() == 0 ) )
    {
      return false;
    }
  }
  return true;
}
