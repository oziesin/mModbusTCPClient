#include "modbustcpclient.h"
#include "ui_modbustcpclient.h"

ModbusTcpClient::ModbusTcpClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::ModbusTcpClient)
{
  ui->setupUi(this);

  pModbusDevice = new QModbusTcpClient(this);

  pCommSetttings = new ComSettings(*pModbusDevice);

  connect(ui->actionCommunication_Settings, &QAction::triggered, this, &ModbusTcpClient::triggered_CommSetting);

  connect(pModbusDevice, &QModbusClient::stateChanged,this, &ModbusTcpClient::onModbusStateChanged);
  connect(pModbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error)
  { statusBar()->showMessage(pModbusDevice->errorString(), 5000);});

  regModel = new RegisterModel(ui->tableWidget, ui->spinBox_StartAddr->value(), ui->spinBox_RegCount->value(), RegisterModel::ModeWR::Read);

  for (auto iter : regModel->get_RegisterTypeList())
    QObject::connect( iter, SIGNAL(currentIndexChanged(int)), this, SLOT(onShowTypeChanged(int)));
}

ModbusTcpClient::~ModbusTcpClient()
{
  delete ui;
  delete pCommSetttings;
}


void ModbusTcpClient::onModbusStateChanged(int state)
{
  switch (state)
  {
    case QModbusClient::UnconnectedState:
    {
      ui->pushButton_Send->setDisabled(true);
    }
    break;

    case QModbusClient::ConnectedState:
    {
     ui->pushButton_Send->setDisabled(false);
    }
    break;
  }
}

void ModbusTcpClient::onReadReady()
{
  auto reply = qobject_cast<QModbusReply *>(sender());
  if (!reply)
    return;

  switch (reply->error())
  {
    case QModbusDevice::NoError:
    {
      const QModbusDataUnit unit = reply->result();
      qDebug() << "Vec:" << unit.values() << "\n";
      for (uint i = 0; i < unit.valueCount(); ++i)
      {
//        qDebug() << "Addr:" << unit.startAddress()+i << "Data:" << unit.value(i) << "Hex:" << QByteArray::fromHex(QString::number(unit.value(i)).toUtf8()) << "\n";
        ui->plainTextEdit_Response->appendPlainText(QString::number(unit.value(i), 16).toUpper());
        regModel->get_RegisterData().at(i)->setText(QString::number(unit.value(i)));
        regModel->get_RegisterDataBytes().at(i)->setText(QString::number(unit.value(i),16).toUpper());
      }
    }
    break;

    case QModbusDevice::ProtocolError:
    {
      statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                  arg(reply->errorString()).
                                  arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    }
    break;
    default:
    {
      statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                      arg(reply->errorString()).
                                      arg(reply->error(), -1, 16), 5000);
    }
    break;
    }
  reply->deleteLater();
}

void ModbusTcpClient::on_pushButton_Send_clicked()
{
   if(pModbusDevice->state() == QModbusTcpClient::ConnectedState)
   {
       Q_ASSERT(ui->comboBox_FunctionCode->currentIndex() == 2);
     if (auto *reply = pModbusDevice->sendReadRequest(Prepare_Request(), ui->spinBox_SlaveId->value()); !reply->isFinished())
       connect(reply, &QModbusReply::finished, this, &ModbusTcpClient::onReadReady);
     else
       delete reply; // broadcast replies return immediately
   }
   else
     statusBar()->showMessage(tr("Read error: ") + pModbusDevice->errorString(), 5000);
}

#warning "This function will be  generic to include every modbus request"
QModbusDataUnit ModbusTcpClient::Prepare_Request() const
{
  const auto FunctionType = static_cast<QModbusDataUnit::RegisterType>(QModbusDataUnit::HoldingRegisters);
  return QModbusDataUnit(FunctionType, ui->spinBox_StartAddr->value(), ui->spinBox_RegCount->value());
}

#warning "Firstly only supports int16 this functions will used after added different show type"
void ModbusTcpClient::onShowTypeChanged(int index)
{
  if(auto Combobox_Idx = regModel->get_RegisterTypeList().indexOf(qobject_cast<QComboBox*>(sender())); Combobox_Idx != -1)
    qDebug() << "Do it something this index !" << Combobox_Idx << "\n";
  else
    qDebug() << "Cannot find Object\n";
}


void ModbusTcpClient::on_spinBox_RegCount_editingFinished()
{
  auto Model_Length = regModel->Get_Length();
  auto newLength = ui->spinBox_RegCount->value();
  auto diff = abs(Model_Length - newLength);

//  qDebug() << "Diff: " << diff << "New Count:" << newLength <<" \n";

  if(Model_Length > newLength)  // Mevcut liste gerekenden fazla register içeriyor silinmeli
    regModel->deleteInstance(diff);

  else if(Model_Length < newLength)
    regModel->AddInstance(diff);
}
