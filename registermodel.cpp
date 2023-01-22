#include "registermodel.h"

#include <QDebug>

RegisterModel::RegisterModel(QTableWidget *tableWidget, quint16 StartAddr, quint16 RegCount, ModeWR Mode)
{
  m_StartAddr = StartAddr;
  m_RegCount = RegCount;
  m_Mode = Mode;
  m_Widget = tableWidget;

  tableWidget->setRowCount(m_RegCount);
  tableWidget->setColumnCount(4);
  tableWidget->setHorizontalHeaderLabels(QStringList() << "Type" << "Register" << "Data" <<  "Bytes");

  for (auto iter = 0; iter < m_RegCount; ++iter)
  {
    List_Types.append(new QComboBox());
    List_Types[iter]->addItems(QStringList() << str_RegisterType[static_cast<int>(RegisterType::int16)]);
//        << str_RegisterType[static_cast<int>(RegisterType::int8)]
//        << str_RegisterType[static_cast<int>(RegisterType::uint8)]
//        << str_RegisterType[static_cast<int>(RegisterType::int16)]
//        << str_RegisterType[static_cast<int>(RegisterType::uint16)]
//        << str_RegisterType[static_cast<int>(RegisterType::int32)]
//        << str_RegisterType[static_cast<int>(RegisterType::uint32)] );

    List_Register.append(new QLineEdit());
    List_Register[iter]->setReadOnly(true);
    List_Register[iter]->setText(QString::number(iter+m_StartAddr));

    List_Data.append(new QLineEdit());
    if(m_Mode == ModeWR::Read)
      List_Data[iter]->setReadOnly(true);
    else
      List_Data[iter]->setReadOnly(false);

    List_DataBytes.append(new QLineEdit());
    List_DataBytes[iter]->setReadOnly(true);


    Set_TypeCellonWidget(m_Widget, iter, List_Types[iter]);
    Set_RegisterCellonWidget(m_Widget, iter, List_Register[iter]);
    Set_DataCellonWidget(m_Widget, iter, List_Data[iter]);
    Set_DataBytesCellonWidget(m_Widget, iter, List_DataBytes[iter]);
  }
}

RegisterModel::RegisterModel(const RegisterModel &)
{
#warning "must write"
}

RegisterModel::RegisterModel(const RegisterModel &&)
{
#warning "must write"
}

RegisterModel::~RegisterModel()
{
  List_Types.clear();
  List_Register.clear();
  List_Data.clear();
  List_DataBytes.clear();
}
