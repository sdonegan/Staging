#include "LabelCADFile.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/TriangleGeom.h"
#include "SIMPLib/Math/GeometryMath.h"

#include "AFRLDistributionC/AFRLDistributionCConstants.h"
#include "AFRLDistributionC/AFRLDistributionCVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
LabelCADFile::LabelCADFile()
: m_CADDataContainerName("")
, m_TriangleAttributeMatrixName("")
, m_RegionIdArrayName("")
, m_RegionId(nullptr)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
LabelCADFile::~LabelCADFile() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void LabelCADFile::setupFilterParameters()
{
  FilterParameterVector parameters;
  DataContainerSelectionFilterParameter::RequirementType dcsReq;
  IGeometry::Types geomTypes = {IGeometry::Type::Triangle};
  dcsReq.dcGeometryTypes = geomTypes;
  parameters.push_back(SIMPL_NEW_DC_SELECTION_FP("CAD Geometry", CADDataContainerName, FilterParameter::RequiredArray, LabelCADFile, dcsReq));
  parameters.push_back(SIMPL_NEW_STRING_FP("Triangle Attribute Matrix", TriangleAttributeMatrixName, FilterParameter::CreatedArray, LabelCADFile));
  parameters.push_back(SIMPL_NEW_STRING_FP("Region Ids", RegionIdArrayName, FilterParameter::CreatedArray, LabelCADFile));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void LabelCADFile::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setCADDataContainerName(reader->readString("CADDataContainerName", getCADDataContainerName()));
  setTriangleAttributeMatrixName(reader->readString("TriangleAttributeMatrixName", getTriangleAttributeMatrixName()));
  setRegionIdArrayName(reader->readString("RegionIdArrayName", getRegionIdArrayName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void LabelCADFile::updateTriangleInstancePointers()
{
  setErrorCondition(0);
  setWarningCondition(0);

  if(nullptr != m_RegionIdPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_RegionId = m_RegionIdPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void LabelCADFile::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  DataArrayPath tempPath;

  TriangleGeom::Pointer triangle = getDataContainerArray()->getPrereqGeometryFromDataContainer<TriangleGeom, AbstractFilter>(this, getCADDataContainerName());

  QVector<size_t> tDims(1, 0);
  getDataContainerArray()->getDataContainer(getCADDataContainerName())->createNonPrereqAttributeMatrix(this, getTriangleAttributeMatrixName(), tDims, AttributeMatrix::Type::Face);

  if(getErrorCondition() < 0)
  {
    return;
  }

  QVector<size_t> cDims(1, 1);
  tempPath.update(getCADDataContainerName(), getTriangleAttributeMatrixName(), getRegionIdArrayName());
  m_RegionIdPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter, int32_t>(this, tempPath, 0,
                                                                                                                     cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_RegionIdPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_RegionId = m_RegionIdPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() < 0)
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void LabelCADFile::preflight()
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
void LabelCADFile::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  TriangleGeom::Pointer triangle = getDataContainerArray()->getDataContainer(getCADDataContainerName())->getGeometryAs<TriangleGeom>();

  int64_t* tris = triangle->getTriPointer(0);
  float* triVerts = triangle->getVertexPointer(0);
  int64_t numTris = triangle->getNumberOfTris();
  // int64_t numTriVerts = triangle->getNumberOfVertices();

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getCADDataContainerName());

  QVector<size_t> tDims(1, numTris);
  m->getAttributeMatrix(getTriangleAttributeMatrixName())->resizeAttributeArrays(tDims);
  updateTriangleInstancePointers();

  int check = triangle->findElementNeighbors();
  if(check < 0)
  {
    // FIXME: Should check this error code.....
  }
  ElementDynamicList::Pointer m_TriangleNeighbors = triangle->getElementNeighbors();

  size_t chunkSize = 1000;
  std::vector<int64_t> triList(chunkSize, -1);

  // first identify connected triangle sets as features
  size_t size = 0;
  int32_t regionCount = 1;
  for(size_t i = 0; i < numTris; i++)
  {
    if(m_RegionId[i] == 0)
    {
      m_RegionId[i] = regionCount;
      size = 0;
      triList[size] = i;
      size++;
      while(size > 0)
      {
        int64_t tri = triList[size - 1];
        size -= 1;
        uint16_t tCount = m_TriangleNeighbors->getNumberOfElements(tri);
        int64_t* data = m_TriangleNeighbors->getElementListPointer(tri);
        for(int j = 0; j < tCount; j++)
        {
          int64_t neighTri = data[j];
          if(m_RegionId[neighTri] == 0)
          {
            m_RegionId[neighTri] = regionCount;
            triList[size] = neighTri;
            size++;
            if(size >= triList.size())
            {
              size = triList.size();
              triList.resize(size + chunkSize);
              for(std::vector<int64_t>::size_type k = size; k < triList.size(); ++k)
              {
                triList[k] = -1;
              }
            }
          }
        }
      }
      regionCount++;
    }
  }

  // next determine bounding boxes so we can see if any regions are within other regions
  std::vector<float> xMinList(regionCount, std::numeric_limits<float>::max());
  std::vector<float> yMinList(regionCount, std::numeric_limits<float>::max());
  std::vector<float> zMinList(regionCount, std::numeric_limits<float>::max());
  std::vector<float> xMaxList(regionCount, -std::numeric_limits<float>::max());
  std::vector<float> yMaxList(regionCount, -std::numeric_limits<float>::max());
  std::vector<float> zMaxList(regionCount, -std::numeric_limits<float>::max());
  for(size_t i = 0; i < numTris; i++)
  {
    int32_t regionId = m_RegionId[i];
    for(int j = 0; j < 3; j++)
    {
      int64_t vert = tris[3 * i + j];
      if(xMinList[regionId] > triVerts[3 * vert + 0])
      {
        xMinList[regionId] = triVerts[3 * vert + 0];
      }
      if(yMinList[regionId] > triVerts[3 * vert + 1])
      {
        yMinList[regionId] = triVerts[3 * vert + 1];
      }
      if(zMinList[regionId] > triVerts[3 * vert + 2])
      {
        zMinList[regionId] = triVerts[3 * vert + 2];
      }
      if(xMaxList[regionId] < triVerts[3 * vert + 0])
      {
        xMaxList[regionId] = triVerts[3 * vert + 0];
      }
      if(yMaxList[regionId] < triVerts[3 * vert + 1])
      {
        yMaxList[regionId] = triVerts[3 * vert + 1];
      }
      if(zMaxList[regionId] < triVerts[3 * vert + 2])
      {
        zMaxList[regionId] = triVerts[3 * vert + 2];
      }
    }
  }

  std::vector<int32_t> newRegionIds(regionCount);
  std::vector<int32_t> contiguousRegionIds(regionCount);
  for(size_t i = 1; i < regionCount; i++)
  {
    newRegionIds[i] = i;
    for(size_t j = 1; j < regionCount; j++)
    {
      if(xMinList[i] > xMinList[j] && xMinList[i] < xMaxList[j] && xMaxList[i] > xMinList[j] && xMaxList[i] < xMaxList[j] && yMinList[i] > yMinList[j] && yMinList[i] < yMaxList[j] &&
         yMaxList[i] > yMinList[j] && yMaxList[i] < yMaxList[j] && zMinList[i] > zMinList[j] && zMinList[i] < zMaxList[j] && zMaxList[i] > zMinList[j] && zMaxList[i] < zMaxList[j])
      {
        newRegionIds[i] = j;
      }
    }
  }

  int32_t newRegionCount = 1;
  for(size_t i = 1; i < regionCount; i++)
  {
    if(newRegionIds[i] == i)
    {
      contiguousRegionIds[i] = newRegionCount;
      newRegionCount++;
    }
  }

  int32_t newRegionId = 0;
  for(size_t i = 0; i < numTris; i++)
  {
    int32_t regionId = m_RegionId[i];
    bool keepGoing = true;
    while(keepGoing)
    {
      if(newRegionIds[regionId] != regionId)
      {
        regionId = newRegionIds[regionId];
      }
      else
      {
        newRegionId = regionId;
        keepGoing = false;
      }
    }
    m_RegionId[i] = contiguousRegionIds[newRegionId];
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer LabelCADFile::newFilterInstance(bool copyFilterParameters) const
{
  LabelCADFile::Pointer filter = LabelCADFile::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString LabelCADFile::getCompiledLibraryName() const
{
  return AFRLDistributionCConstants::AFRLDistributionCBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString LabelCADFile::getBrandingString() const
{
  return "AFRLDistributionC";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString LabelCADFile::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << AFRLDistributionC::Version::Major() << "." << AFRLDistributionC::Version::Minor() << "." << AFRLDistributionC::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString LabelCADFile::getGroupName() const
{
  return SIMPL::FilterGroups::ReconstructionFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString LabelCADFile::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::GroupingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString LabelCADFile::getHumanLabel() const
{
  return "Label CAD Geometry";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid LabelCADFile::getUuid()
{
  return QUuid("{a250a228-3b6b-5b37-a6e4-8687484f04c4}");
}