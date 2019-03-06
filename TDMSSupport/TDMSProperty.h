#ifndef _tdmsproperty_h
#define _tdmsproperty_h

#include "TDMSDataType.hpp"

#include "SIMPLib/DataArrays/DataArray.hpp"

class TDMSMetaData;

class TDMSProperty
{
public:
  virtual ~TDMSProperty();
  TDMSProperty(const TDMSProperty&) = delete;
  TDMSProperty& operator=(const TDMSProperty&) = delete;

  typedef std::shared_ptr<TDMSProperty> Pointer;

  TDMSDataType::Pointer dataType()
  {
    return m_Type;
  }

  IDataArray::Pointer value()
  {
    return m_Value;
  }

private:
  friend class TDMSMetaData;

  TDMSProperty(TDMSDataType::Pointer type, IDataArray::Pointer value);
  static Pointer New(TDMSDataType::Pointer type, IDataArray::Pointer value);

  TDMSDataType::Pointer m_Type;
  IDataArray::Pointer m_Value;
};

#endif
