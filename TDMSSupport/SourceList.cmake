set(TDMSSupport_SRCS
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSFileProxy.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSLeadIn.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSObject.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSSegment.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSDataTypeFactory.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSMetaData.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSProperty.cpp
)

set(TDMSSupport_HDRS
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSFileProxy.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSLeadIn.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSLeadInStruct.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSObject.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSSegment.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSDataTypeFactory.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSDataType.hpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSMetaData.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSExceptionHandler.h
  ${${PLUGIN_NAME}_SOURCE_DIR}/TDMSSupport/TDMSProperty.h
)

cmp_IDE_SOURCE_PROPERTIES( "TDMSSupport" "${TDMSSupport_HDRS}" "${TDMSSupport_SRCS}" "0")

