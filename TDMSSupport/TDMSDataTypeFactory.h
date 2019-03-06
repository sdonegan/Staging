#ifndef _tdmsdatatypefactory_h
#define _tdmsdatatypefactory_h

#include <map>

#include "TDMSDataType.hpp"

class TDMSDataTypeFactory
{
public:
  virtual ~TDMSDataTypeFactory();
  TDMSDataTypeFactory(const TDMSDataTypeFactory&) = delete;
  TDMSDataTypeFactory& operator=(const TDMSDataTypeFactory&) = delete;

  static TDMSDataTypeFactory* Instance();

  TDMSDataType::Pointer getDataType(uint32_t index);

private:
  TDMSDataTypeFactory();

  void initializeDataTypes();

  std::string getListOfSupportedDataTypes();

  static TDMSDataTypeFactory* self;
  std::map<uint32_t, TDMSDataType::Pointer> m_DataTypes;
};

#endif
