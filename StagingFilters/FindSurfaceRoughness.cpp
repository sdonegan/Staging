#include "FindSurfaceRoughness.h"

#include <cmath>
#include <cstring>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "AFRLDistributionC/AFRLDistributionCConstants.h"
#include "AFRLDistributionC/AFRLDistributionCVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindSurfaceRoughness::FindSurfaceRoughness()
: m_BoundaryCellsArrayPath(SIMPL::Defaults::ImageDataContainerName, SIMPL::Defaults::CellAttributeMatrixName, SIMPL::CellData::BoundaryCells)
, m_AttributeMatrixName("RoughnessData")
, m_RoughnessParamsArrayName("RougnessParameters")
, m_RoughnessParams(nullptr)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindSurfaceRoughness::~FindSurfaceRoughness() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindSurfaceRoughness::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindSurfaceRoughness::setupFilterParameters()
{
  FilterParameterVector parameters;
  DataArraySelectionFilterParameter::RequirementType dasReq = DataArraySelectionFilterParameter::CreateRequirement(SIMPL::TypeNames::Int8, 1, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Boundary Cells", BoundaryCellsArrayPath, FilterParameter::RequiredArray, FindSurfaceRoughness, dasReq));
  parameters.push_back(SIMPL_NEW_STRING_FP("Roughness Attribute Matrix", AttributeMatrixName, FilterParameter::CreatedArray, FindSurfaceRoughness));
  parameters.push_back(SIMPL_NEW_STRING_FP("Roughness Parameters", RoughnessParamsArrayName, FilterParameter::CreatedArray, FindSurfaceRoughness));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindSurfaceRoughness::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  ImageGeom::Pointer image = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(this, getBoundaryCellsArrayPath().getDataContainerName());

  if(getErrorCondition() < 0)
  {
    return;
  }

  QVector<size_t> cDims(1, 1);

  m_BoundaryCellsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int8_t>, AbstractFilter>(this, getBoundaryCellsArrayPath(), cDims);
  if(nullptr != m_BoundaryCellsPtr.lock().get())
  {
    m_BoundaryCells = m_BoundaryCellsPtr.lock()->getPointer(0);
  }

  DataContainer::Pointer dc = getDataContainerArray()->getPrereqDataContainer<AbstractFilter>(this, getBoundaryCellsArrayPath().getDataContainerName());

  if(getErrorCondition() < 0)
  {
    return;
  }

  dc->createNonPrereqAttributeMatrix(this, getAttributeMatrixName(), QVector<size_t>(1, 1), AttributeMatrix::Type::CellFeature);

  DataArrayPath path(getBoundaryCellsArrayPath().getDataContainerName(), getAttributeMatrixName(), getRoughnessParamsArrayName());

  cDims[0] = 3;

  m_RoughnessParamsPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<double>, AbstractFilter, double>(this, path, 0, cDims);
  if(nullptr != m_RoughnessParamsPtr.lock().get())
  {
    m_RoughnessParams = m_RoughnessParamsPtr.lock()->getPointer(0);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindSurfaceRoughness::preflight()
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
void FindSurfaceRoughness::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(m_BoundaryCellsArrayPath.getDataContainerName())->getGeometryAs<ImageGeom>();
  size_t dims[3] = {0, 0, 0};
  float res[3] = {0.0f, 0.0f, 0.0f};
  image->getResolution(res);
  std::tie(dims[0], dims[1], dims[2]) = image->getDimensions();
  std::vector<double> xs;
  std::vector<double> ys;
  double coords[3] = {0.0, 0.0, 0.0};

  for(size_t z = 0; z < dims[2]; z++)
  {
    size_t zStride = (z * dims[1] * dims[0]);
    for(size_t y = 0; y < dims[1]; y++)
    {
      size_t yStride = (y * dims[0]);
      for(size_t x = 0; x < dims[0]; x++)
      {
        size_t index = zStride + yStride + x;
        if(m_BoundaryCells[index] > 0)
        {
          image->getCoords(index, coords);
          xs.push_back(coords[0]);
          ys.push_back(coords[1]);
        }
      }
    }
  }

  using LineParams = std::pair<double, double>;

  auto linear_regression_slope = [&]() -> LineParams {
    double xmean = std::accumulate(std::begin(xs), std::end(xs), 0.0);
    double ymean = std::accumulate(std::begin(ys), std::end(ys), 0.0);
    xmean /= xs.size();
    ymean /= ys.size();

    std::vector<double> xx;
    std::transform(std::begin(xs), std::end(xs), std::begin(xs), std::back_inserter(xx), std::multiplies<double>());
    double sumxx = std::accumulate(std::begin(xx), std::end(xx), 0.0);
    double ssxx = sumxx - (xx.size() * xmean * xmean);

    std::vector<double> xy;
    std::transform(std::begin(xs), std::end(xs), std::begin(ys), std::back_inserter(xy), std::multiplies<double>());
    double sumxy = std::accumulate(std::begin(xy), std::end(xy), 0.0);
    double ssxy = sumxy - (xy.size() * xmean * ymean);

    double b = ssxy / ssxx;
    double a = ymean - (b * xmean);

    return std::make_pair(a, b);
  };

  LineParams line = linear_regression_slope();
  m_RoughnessParams[1] = line.first;
  m_RoughnessParams[2] = line.second;

  double distance = 0.0;
  for(auto i = 0; i < xs.size(); i++)
  {
    double numerator = std::abs(ys[i] - (line.first + (line.second * xs[i])));
    double denominator = std::sqrt(1 + (line.second * line.second));
    distance += (numerator / denominator);
  }

  distance /= static_cast<double>(xs.size());

  m_RoughnessParams[0] = distance;

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FindSurfaceRoughness::newFilterInstance(bool copyFilterParameters) const
{
  FindSurfaceRoughness::Pointer filter = FindSurfaceRoughness::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindSurfaceRoughness::getCompiledLibraryName() const
{
  return AFRLDistributionCConstants::AFRLDistributionCBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindSurfaceRoughness::getBrandingString() const
{
  return "AFRLDistributionC";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindSurfaceRoughness::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << AFRLDistributionC::Version::Major() << "." << AFRLDistributionC::Version::Minor() << "." << AFRLDistributionC::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindSurfaceRoughness::getGroupName() const
{
  return SIMPL::FilterGroups::StatisticsFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindSurfaceRoughness::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::GeometryFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindSurfaceRoughness::getHumanLabel() const
{
  return "Find Surface Roughness";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid FindSurfaceRoughness::getUuid()
{
  return QUuid("{4178c7f9-5f90-5e95-8cf1-a67ca2a98a60}");
}