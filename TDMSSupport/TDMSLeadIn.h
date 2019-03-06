#ifndef _tdmsleadin_h
#define _tdmsleadin_h

#include <fstream>
#include <memory>
#include <set>

#include "TDMSLeadInStruct.h"

class TDMSSegment;

class TDMSLeadIn
{
public:
  virtual ~TDMSLeadIn();
  TDMSLeadIn(const TDMSLeadIn&) = delete;
  TDMSLeadIn& operator=(const TDMSLeadIn&) = delete;

  typedef std::shared_ptr<TDMSLeadIn> Pointer;

  struct ToCBitMasks
  {
    const uint32_t MetaData = static_cast<uint32_t>(1) << 1;
    const uint32_t RawData = static_cast<uint32_t>(1) << 3;
    const uint32_t DAQmxRawData = static_cast<uint32_t>(1) << 7;
    const uint32_t InterleavedData = static_cast<uint32_t>(1) << 5;
    const uint32_t BigEndian = static_cast<uint32_t>(1) << 6;
    const uint32_t NewObjList = static_cast<uint32_t>(1) << 2;
  };

  struct ToCFlags
  {
    bool HasMetaData = false;
    bool HasRawData = false;
    bool HasDAQmxRawData = false;
    bool IsInterleavedData = false;
    bool IsBigEndian = false;
    bool HasNewObjList = false;
  };

  static const std::string TDMSTAG;
  static const uint32_t TDMSLEADINLENGTH = 28;
  static const std::set<uint32_t> TDMSVERSIONS;

private:
  friend class TDMSSegment;

  TDMSLeadIn(std::ifstream& filestream);
  static Pointer New(std::ifstream& filestream);

  void constructLeadIn();

  std::ifstream& m_FileStream;
  TDMSLeadInStruct m_LeadInStruct;
  ToCFlags m_ToCFlags;
};

#endif
