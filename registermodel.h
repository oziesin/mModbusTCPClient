#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>

class RegisterModel
{
public:
  enum class ModeWR
  {
    Read,
    Write,
    Max
  };

  RegisterModel(QTableWidget *tableWidget, quint16 StartAddr, quint16 RegCount, ModeWR Mode = ModeWR::Read);
  RegisterModel(const RegisterModel&);
  RegisterModel(const RegisterModel&&);

  ~RegisterModel();


  void Set_Mode( const ModeWR& rMode )
  {
    m_Mode = rMode;
  }
  ModeWR Get_Mode() const
  {
    return m_Mode;
  }

  int Get_Length()const
  {
    Q_ASSERT((List_Types.length() == List_Data.length()) && (List_DataBytes.length() == List_Register.length()) && (List_DataBytes.length() == List_Data.length()));
    return List_Types.length();
  }

  QList<QComboBox*> get_RegisterTypeList() const
  {
    return List_Types;
  }
  QList<QLineEdit*> get_RegisterAddrList() const
  {
    return List_Register;
  }
  QList<QLineEdit*> get_RegisterData()
  {
    return List_Data;
  }
  QList<QLineEdit*> get_RegisterDataBytes() const
  {
    return List_DataBytes;
  }


  void deleteInstance(int count)
  {
    for (int var = 0; var < count; ++var)
      deleteInstance();
    m_RegCount -= count;
    m_Widget->setRowCount(m_RegCount);
  }
  void AddInstance(int count)
  {
    m_RegCount += count;
    m_Widget->setRowCount(m_RegCount);
    for (int var = 0; var < count; ++var)
      AddInstance();
  }


private:

  void AddInstance()
  {
    List_Types.append(new QComboBox());
    List_Types.last()->addItems(QStringList() << str_RegisterType[static_cast<int>(RegisterType::int16)]);
//          << str_RegisterType[static_cast<int>(RegisterType::int8)]
//          << str_RegisterType[static_cast<int>(RegisterType::uint8)]
//          << str_RegisterType[static_cast<int>(RegisterType::int16)]
//          << str_RegisterType[static_cast<int>(RegisterType::uint16)]
//          << str_RegisterType[static_cast<int>(RegisterType::int32)]
//          << str_RegisterType[static_cast<int>(RegisterType::uint32)] );

    List_Register.append(new QLineEdit());
    List_Register.last()->setReadOnly(true);
    List_Register.last()->setText(QString::number( m_StartAddr + List_Register.length() - 1));

    List_Data.append(new QLineEdit());
    if(m_Mode == ModeWR::Read)
      List_Data.last()->setReadOnly(true);
    else
      List_Data.last()->setReadOnly(false);

    List_DataBytes.append(new QLineEdit());
    List_DataBytes.last()->setReadOnly(true);

    Set_TypeCellonWidget(m_Widget, List_Types.length()-1, List_Types.last());
    Set_RegisterCellonWidget(m_Widget, List_Register.length()-1, List_Register.last());
    Set_DataCellonWidget(m_Widget, List_Data.length()-1, List_Data.last());
    Set_DataBytesCellonWidget(m_Widget, List_DataBytes.length()-1, List_DataBytes.last());
  }
  void deleteInstance()
  {
    delete List_Types.last();
    delete List_Register.last();
    delete List_Data.last();
    delete List_DataBytes.last();

    List_Types.removeLast();
    List_Register.removeLast();
    List_Data.removeLast();
    List_DataBytes.removeLast();
  }
  void Set_TypeCellonWidget(QTableWidget* tableWidget,int row, QWidget *widget)
  {
    tableWidget->setCellWidget(row, 0, widget);
  }
  void Set_RegisterCellonWidget(QTableWidget* tableWidget,int row, QWidget *widget)
  {
    tableWidget->setCellWidget(row, 1, widget);
  }
  void Set_DataCellonWidget(QTableWidget* tableWidget,int row, QWidget *widget)
  {
    tableWidget->setCellWidget(row, 2, widget);
  }
  void Set_DataBytesCellonWidget(QTableWidget* tableWidget,int row, QWidget *widget)
  {
    tableWidget->setCellWidget(row, 3, widget);
  }
  enum class RegisterType
  {
    int8,
    uint8,
    int16,
    uint16,
    int32,
    uint32,
    Max
  };

  inline static const QString str_RegisterType[] = {"int8","uint8","int16","uint16","int32","uint32","Max"};

  QList<QComboBox*> List_Types;

  QList<QLineEdit*> List_Register;

  QList<QLineEdit*> List_Data;

  QList<QLineEdit*> List_DataBytes;

  QTableWidget *m_Widget;
  ModeWR m_Mode;
  quint16 m_StartAddr;
  quint16 m_RegCount;
};

#endif // REGISTERMODEL_H
