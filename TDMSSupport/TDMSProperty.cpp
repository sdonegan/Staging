#include "TDMSProperty.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TDMSProperty::TDMSProperty(TDMSDataType::Pointer type, IDataArray::Pointer value)
: m_Type(type)
, m_Value(value)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TDMSProperty::~TDMSProperty()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TDMSProperty::Pointer TDMSProperty::New(TDMSDataType::Pointer type, IDataArray::Pointer value)
{
  Pointer shared(new TDMSProperty(type, value));
  return shared;
}