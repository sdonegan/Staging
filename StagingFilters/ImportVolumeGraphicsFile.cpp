#include "ReadBinaryCTXRadia.h"

#include <QtCore/QFileInfo>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Common/ScopedFileMonitor.hpp"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "AFRLDistributionC/AFRLDistributionCConstants.h"
#include "AFRLDistributionC/AFRLDistributionCVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ReadBinaryCTXRadia::ReadBinaryCTXRadia()
: m_InputFile("")
, m_InputHeaderFile("")
, m_DataContainerName("ImageDataContainer")
, m_CellAttributeMatrixName("CellData")
, m_DensityArrayName("Density")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ReadBinaryCTXRadia::~ReadBinaryCTXRadia() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadBinaryCTXRadia::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Input CT File", InputFile, FilterParameter::Parameter, ReadBinaryCTXRadia, "*.vol", "Voxel data"));
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Input Header File", InputHeaderFile, FilterParameter::Parameter, ReadBinaryCTXRadia, "*.vgi", "NSI header"));
  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ReadBinaryCTXRadia));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ReadBinaryCTXRadia));
  parameters.push_back(SIMPL_NEW_STRING_FP("Density", DensityArrayName, FilterParameter::CreatedArray, ReadBinaryCTXRadia));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadBinaryCTXRadia::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setInputFile(reader->readString("InputFile", getInputFile()));
  setInputHeaderFile(reader->readString("InputHeaderFile", getInputHeaderFile()));
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName()));
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName()));
  setDensityArrayName(reader->readString("DensityArrayName", getDensityArrayName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadBinaryCTXRadia::initialize()
{
  if(m_InHeaderStream.isOpen())
    m_InHeaderStream.close();
  if(m_InStream.isOpen())
    m_InStream.close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadBinaryCTXRadia::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();
  int32_t err = 0;

  QFileInfo fi(getInputFile());

  if(getInputFile().isEmpty() == true)
  {
    QString ss = QObject::tr("The input binary CT voxel file must be set");
    setErrorCondition(-387);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
  else if(fi.exists() == false)
  {
    QString ss = QObject::tr("The input binary CT voxel file does not exist");
    setErrorCondition(-388);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  QFileInfo fiHdr(getInputHeaderFile());

  if(getInputHeaderFile().isEmpty() == true)
  {
    QString ss = QObject::tr("The input header file must be set");
    setErrorCondition(-387);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
  else if(fiHdr.exists() == false)
  {
    QString ss = QObject::tr("The input header file does not exist");
    setErrorCondition(-388);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  if(getErrorCondition() < 0)
  {
    return;
  }

  if(m_InHeaderStream.isOpen() == true)
  {
    m_InHeaderStream.close();
  }

  m_InHeaderStream.setFileName(getInputHeaderFile());

  if(!m_InHeaderStream.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString ss = QObject::tr("Error opening input header file: %1").arg(getInputHeaderFile());
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);

  err = readHeaderMetaData(image);

  if(err < 0)
  {
    QString ss = QObject::tr("Error reading input header file: %1").arg(getInputHeaderFile());
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName());

  if(getErrorCondition() < 0)
  {
    return;
  }

  m->setGeometry(image);

  size_t dims[3] = {0, 0, 0};
  std::tie(dims[0], dims[1], dims[2]) = image->getDimensions();

  QVector<size_t> tDims = {dims[0], dims[1], dims[2]};
  QVector<size_t> cDims(1, 1);

  m->createNonPrereqAttributeMatrix(this, getCellAttributeMatrixName(), tDims, AttributeMatrix::Type::Cell);

  DataArrayPath path(getDataContainerName(), getCellAttributeMatrixName(), getDensityArrayName());

  m_DensityPtr =
      getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>, AbstractFilter, float>(this, path, 0, cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_DensityPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_Density = m_DensityPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadBinaryCTXRadia::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ReadBinaryCTXRadia::sanityCheckFileSizeVersusAllocatedSize(size_t allocatedBytes, size_t fileSize)
{
  if(fileSize < allocatedBytes)
  {
    return -1;
  }
  else if(fileSize > allocatedBytes)
  {
    return 1;
  }
  // File Size and Allocated Size are equal so we  are good to go
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ReadBinaryCTXRadia::readBinaryCTFile(size_t dims[3])
{
  int32_t error = 0;

  QFileInfo fi(getInputFile());
  size_t filesize = static_cast<size_t>(fi.size());
  size_t allocatedBytes = m_DensityPtr.lock()->getSize() * sizeof(float);

  error = sanityCheckFileSizeVersusAllocatedSize(allocatedBytes, filesize);

  if(error < 0)
  {
    QString ss = QObject::tr("Binary file size is smaller than the number of allocated bytes");
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return getErrorCondition();
  }

  FILE* f = fopen(getInputFile().toLatin1().data(), "rb");
  if(nullptr == f)
  {
    QString ss = QObject::tr("Error opening binary input file: %1").arg(getInputFile());
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return getErrorCondition();
  }

  ScopedFileMonitor monitor(f);

  float* ptr = m_DensityPtr.lock()->getPointer(0);

  size_t index = 0;
  size_t totalPoints = dims[0] * dims[1] * dims[2];
  int64_t progIncrement = static_cast<int64_t>(totalPoints / 100);
  int64_t prog = 1;
  int64_t progressInt = 0;
  int64_t counter = 0;

  for(size_t z = 0; z < dims[2]; z++)
  {
    for(size_t y = 0; y < dims[1]; y++)
    {
      for(size_t x = 0; x < dims[0]; x++)
      {
        // Compute the proper index into the final storage array
        index = (dims[0] * dims[1] * z) + (dims[0] * (dims[1] - y - 1)) + (dims[0] - x - 1);
        fread(ptr + index, sizeof(float), 1, f);

        // Check for progress
        if(counter > prog)
        {
          progressInt = static_cast<int64_t>((static_cast<float>(counter) / totalPoints) * 100.0f);
          QString ss = QObject::tr("Reading Data || %1% Completed").arg(progressInt);
          notifyStatusMessage(getMessagePrefix(), getHumanLabel(), ss);
          prog = prog + progIncrement;
        }
        counter++;
      }
    }
  }

  return error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ReadBinaryCTXRadia::readHeaderMetaData(ImageGeom::Pointer image)
{
  int32_t error = 0;

  QByteArray buf;
  QList<QByteArray> tokens;
  bool ok = false;

  size_t dims[3] = {0, 0, 0};
  float res[3] = {0.0, 0.0, 0.0};
  bool done = false;

  while(m_InHeaderStream.atEnd() == false && done == false)
  {
    buf = m_InHeaderStream.readLine();
    buf = buf.trimmed();
    tokens = buf.split(' ');
    for(qint32 i = 0; i < tokens.size(); i++)
    {
      if(tokens[i] == "[representation]")
      {
        buf = m_InHeaderStream.readLine();
        buf = buf.trimmed();
        tokens = buf.split(' ');
        for(qint32 j = 0; j < tokens.size(); j++)
        {
          if(tokens[j] == "size")
          {
            dims[0] = tokens[j + 2].toULongLong(&ok);
            if(!ok)
            {
              return -1;
            }
            dims[1] = tokens[j + 3].toULongLong(&ok);
            if(!ok)
            {
              return -1;
            }
            dims[2] = tokens[j + 4].toULongLong(&ok);
            if(!ok)
            {
              return -1;
            }
            done = true;
            break;
          }
        }
      }
    }
  }

  m_InHeaderStream.reset();
  done = false;

  while(m_InHeaderStream.atEnd() == false && done == false)
  {
    buf = m_InHeaderStream.readLine();
    buf = buf.trimmed();
    tokens = buf.split(' ');
    for(qint32 i = 0; i < tokens.size(); i++)
    {
      if(tokens[i] == "[geometry]")
      {
        bool done2 = false;
        while(m_InHeaderStream.atEnd() == false && done2 == false)
        {
          buf = m_InHeaderStream.readLine();
          buf = buf.trimmed();
          tokens = buf.split(' ');
          for(qint32 j = 0; j < tokens.size(); j++)
          {
            if(tokens[j] == "resolution")
            {
              res[0] = tokens[j + 2].toFloat(&ok);
              if(!ok)
              {
                return -1;
              }
              res[1] = tokens[j + 3].toFloat(&ok);
              if(!ok)
              {
                return -1;
              }
              res[2] = tokens[j + 4].toFloat(&ok);
              if(!ok)
              {
                return -1;
              }
              done2 = true;
              break;
            }
          }
        }
        if(done2 == true)
        {
          done = true;
          break;
        }
      }
    }
  }

  image->setDimensions(dims);
  image->setResolution(res);

  return error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadBinaryCTXRadia::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  int32_t err = 0;

  ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(getDataContainerName())->getGeometryAs<ImageGeom>();

  size_t dims[3] = {0, 0, 0};
  std::tie(dims[0], dims[1], dims[2]) = image->getDimensions();

  err = readBinaryCTFile(dims);

  if(err < 0)
  {
    QString ss = QObject::tr("Error reading binary input file");
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ReadBinaryCTXRadia::newFilterInstance(bool copyFilterParameters) const
{
  ReadBinaryCTXRadia::Pointer filter = ReadBinaryCTXRadia::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ReadBinaryCTXRadia::getCompiledLibraryName() const
{
  return AFRLDistributionCConstants::AFRLDistributionCBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ReadBinaryCTXRadia::getBrandingString() const
{
  return "AFRLDistributionC";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ReadBinaryCTXRadia::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << AFRLDistributionC::Version::Major() << "." << AFRLDistributionC::Version::Minor() << "." << AFRLDistributionC::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ReadBinaryCTXRadia::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ReadBinaryCTXRadia::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ReadBinaryCTXRadia::getHumanLabel() const
{
  return "Import Binary CT File (XRadia)";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ReadBinaryCTXRadia::getUuid()
{
  return QUuid("{5fa10d81-94b4-582b-833f-8eabe659069e}");
}