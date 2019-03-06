#include "AlignGeometries.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerSelectionFilterParameter.h"
#include "SIMPLib/Geometry/EdgeGeom.h"
#include "SIMPLib/Geometry/IGeometry2D.h"
#include "SIMPLib/Geometry/IGeometry3D.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Geometry/RectGridGeom.h"
#include "SIMPLib/Geometry/VertexGeom.h"

#include "AFRLDistributionC/AFRLDistributionCConstants.h"
#include "AFRLDistributionC/AFRLDistributionCVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AlignGeometries::AlignGeometries()
: m_MovingGeometry("")
, m_TargetGeometry("")
, m_AlignmentType(0)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AlignGeometries::~AlignGeometries()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignGeometries::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignGeometries::setupFilterParameters()
{
  FilterParameterVector parameters;
  DataContainerSelectionFilterParameter::RequirementType dcsReq;
  parameters.push_back(SIMPL_NEW_DC_SELECTION_FP("Moving Geometry", MovingGeometry, FilterParameter::RequiredArray, AlignGeometries, dcsReq));
  parameters.push_back(SIMPL_NEW_DC_SELECTION_FP("Target Geometry", TargetGeometry, FilterParameter::RequiredArray, AlignGeometries, dcsReq));
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Alignment Type");
    parameter->setPropertyName("AlignmentType");
    QVector<QString> choices;
    choices.push_back("Origin");
    choices.push_back("Centroid");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameter->setSetterCallback(SIMPL_BIND_SETTER(AlignGeometries, this, AlignmentType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(AlignGeometries, this, AlignmentType));
    parameters.push_back(parameter);
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignGeometries::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  getDataContainerArray()->getPrereqGeometryFromDataContainer<IGeometry, AbstractFilter>(this, getMovingGeometry());
  getDataContainerArray()->getPrereqGeometryFromDataContainer<IGeometry, AbstractFilter>(this, getTargetGeometry());

  if(getAlignmentType() != 0 && getAlignmentType() != 1)
  {
    QString ss = QObject::tr("Invalid selection for alignment type");
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignGeometries::preflight()
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
void extractOrigin(IGeometry::Pointer geometry, float origin[3])
{
  if(ImageGeom::Pointer image = std::dynamic_pointer_cast<ImageGeom>(geometry))
  {
    image->getOrigin(origin);
    return;
  }
  else if(RectGridGeom::Pointer rectGrid = std::dynamic_pointer_cast<RectGridGeom>(geometry))
  {
    FloatArrayType::Pointer xBounds = rectGrid->getXBounds();
    FloatArrayType::Pointer yBounds = rectGrid->getYBounds();
    FloatArrayType::Pointer zBounds = rectGrid->getZBounds();
    origin[0] = std::numeric_limits<float>::max();
    origin[1] = std::numeric_limits<float>::max();
    origin[2] = std::numeric_limits<float>::max();
    for(size_t i = 0; i < xBounds->getNumberOfTuples(); i++)
    {
      if(xBounds->getValue(i) < origin[0])
      {
        origin[0] = xBounds->getValue(i);
      }
    }
    for(size_t i = 0; i < yBounds->getNumberOfTuples(); i++)
    {
      if(yBounds->getValue(i) < origin[1])
      {
        origin[1] = yBounds->getValue(i);
      }
    }
    for(size_t i = 0; i < zBounds->getNumberOfTuples(); i++)
    {
      if(zBounds->getValue(i) < origin[2])
      {
        origin[2] = zBounds->getValue(i);
      }
    }
    return;
  }
  else if(VertexGeom::Pointer vertex = std::dynamic_pointer_cast<VertexGeom>(geometry))
  {
    float* vertices = vertex->getVertexPointer(0);
    origin[0] = std::numeric_limits<float>::max();
    origin[1] = std::numeric_limits<float>::max();
    origin[2] = std::numeric_limits<float>::max();
    for(int64_t i = 0; i < vertex->getNumberOfVertices(); i++)
    {
      for(size_t j = 0; j < 3; j++)
      {
        if(vertices[3 * i + j] < origin[j])
        {
          origin[j] = vertices[3 * i + j];
        }
      }
    }
    return;
  }
  else if(EdgeGeom::Pointer edge = std::dynamic_pointer_cast<EdgeGeom>(geometry))
  {
    float* vertices = edge->getVertexPointer(0);
    origin[0] = std::numeric_limits<float>::max();
    origin[1] = std::numeric_limits<float>::max();
    origin[2] = std::numeric_limits<float>::max();
    for(int64_t i = 0; i < vertex->getNumberOfVertices(); i++)
    {
      for(size_t j = 0; j < 3; j++)
      {
        if(vertices[3 * i + j] < origin[j])
        {
          origin[j] = vertices[3 * i + j];
        }
      }
    }
    return;
  }
  else if(IGeometry2D::Pointer geometry2d = std::dynamic_pointer_cast<IGeometry2D>(geometry))
  {
    float* vertices = geometry2d->getVertexPointer(0);
    origin[0] = std::numeric_limits<float>::max();
    origin[1] = std::numeric_limits<float>::max();
    origin[2] = std::numeric_limits<float>::max();
    for(int64_t i = 0; i < vertex->getNumberOfVertices(); i++)
    {
      for(size_t j = 0; j < 3; j++)
      {
        if(vertices[3 * i + j] < origin[j])
        {
          origin[j] = vertices[3 * i + j];
        }
      }
    }
    return;
  }
  else if(IGeometry3D::Pointer geometry3d = std::dynamic_pointer_cast<IGeometry3D>(geometry))
  {
    float* vertices = geometry3d->getVertexPointer(0);
    origin[0] = std::numeric_limits<float>::max();
    origin[1] = std::numeric_limits<float>::max();
    origin[2] = std::numeric_limits<float>::max();
    for(int64_t i = 0; i < vertex->getNumberOfVertices(); i++)
    {
      for(size_t j = 0; j < 3; j++)
      {
        if(vertices[3 * i + j] < origin[j])
        {
          origin[j] = vertices[3 * i + j];
        }
      }
    }
    return;
  }
  else
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void extractCentroid(IGeometry::Pointer geometry, float centroid[3])
{
  if(ImageGeom::Pointer image = std::dynamic_pointer_cast<ImageGeom>(geometry))
  {
    size_t dims[3] = {0, 0, 0};
    float origin[3] = {0.0f, 0.0f, 0.0f};
    float res[3] = {0.0f, 0.0f, 0.0f};
    std::tie(dims[0], dims[1], dims[2]) = image->getDimensions();
    image->getOrigin(origin);
    image->getResolution(res);
    centroid[0] = (static_cast<float>(dims[0]) * res[0] / 2.0f) + origin[0];
    centroid[1] = (static_cast<float>(dims[1]) * res[1] / 2.0f) + origin[1];
    centroid[2] = (static_cast<float>(dims[2]) * res[2] / 2.0f) + origin[2];
    return;
  }
  else if(RectGridGeom::Pointer rectGrid = std::dynamic_pointer_cast<RectGridGeom>(geometry))
  {
    FloatArrayType::Pointer xBounds = rectGrid->getXBounds();
    FloatArrayType::Pointer yBounds = rectGrid->getYBounds();
    FloatArrayType::Pointer zBounds = rectGrid->getZBounds();
    float min[3] = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    float max[3] = {std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};
    for(size_t i = 0; i < xBounds->getNumberOfTuples(); i++)
    {
      if(xBounds->getValue(i) < min[0])
      {
        min[0] = xBounds->getValue(i);
      }
      if(xBounds->getValue(i) > max[0])
      {
        max[0] = xBounds->getValue(i);
      }
    }
    for(size_t i = 0; i < yBounds->getNumberOfTuples(); i++)
    {
      if(yBounds->getValue(i) < min[1])
      {
        min[1] = yBounds->getValue(i);
      }
      if(yBounds->getValue(i) > max[1])
      {
        max[1] = yBounds->getValue(i);
      }
    }
    for(size_t i = 0; i < zBounds->getNumberOfTuples(); i++)
    {
      if(zBounds->getValue(i) < min[2])
      {
        min[2] = zBounds->getValue(i);
      }
      if(zBounds->getValue(i) > max[2])
      {
        max[2] = zBounds->getValue(i);
      }
    }
    centroid[0] = (max[0] - min[0]) / 2.0f;
    centroid[1] = (max[1] - min[1]) / 2.0f;
    centroid[2] = (max[2] - min[2]) / 2.0f;
    return;
  }
  else if(VertexGeom::Pointer vertex = std::dynamic_pointer_cast<VertexGeom>(geometry))
  {
    float* vertices = vertex->getVertexPointer(0);
    centroid[0] = 0.0f;
    centroid[1] = 0.0f;
    centroid[2] = 0.0f;
    for(int64_t i = 0; i < vertex->getNumberOfVertices(); i++)
    {
      centroid[0] += vertices[3 * i + 0];
      centroid[1] += vertices[3 * i + 1];
      centroid[2] += vertices[3 * i + 2];
    }
    centroid[0] /= static_cast<float>(vertex->getNumberOfVertices());
    centroid[1] /= static_cast<float>(vertex->getNumberOfVertices());
    centroid[2] /= static_cast<float>(vertex->getNumberOfVertices());
    return;
  }
  else if(EdgeGeom::Pointer edge = std::dynamic_pointer_cast<EdgeGeom>(geometry))
  {
    float* vertices = edge->getVertexPointer(0);
    centroid[0] = 0.0f;
    centroid[1] = 0.0f;
    centroid[2] = 0.0f;
    for(int64_t i = 0; i < edge->getNumberOfVertices(); i++)
    {
      centroid[0] += vertices[3 * i + 0];
      centroid[1] += vertices[3 * i + 1];
      centroid[2] += vertices[3 * i + 2];
    }
    centroid[0] /= static_cast<float>(edge->getNumberOfVertices());
    centroid[1] /= static_cast<float>(edge->getNumberOfVertices());
    centroid[2] /= static_cast<float>(edge->getNumberOfVertices());
    return;
  }
  else if(IGeometry2D::Pointer geometry2d = std::dynamic_pointer_cast<IGeometry2D>(geometry))
  {
    float* vertices = geometry2d->getVertexPointer(0);
    centroid[0] = 0.0f;
    centroid[1] = 0.0f;
    centroid[2] = 0.0f;
    for(int64_t i = 0; i < geometry2d->getNumberOfVertices(); i++)
    {
      centroid[0] += vertices[3 * i + 0];
      centroid[1] += vertices[3 * i + 1];
      centroid[2] += vertices[3 * i + 2];
    }
    centroid[0] /= static_cast<float>(geometry2d->getNumberOfVertices());
    centroid[1] /= static_cast<float>(geometry2d->getNumberOfVertices());
    centroid[2] /= static_cast<float>(geometry2d->getNumberOfVertices());
    return;
  }
  else if(IGeometry3D::Pointer geometry3d = std::dynamic_pointer_cast<IGeometry3D>(geometry))
  {
    float* vertices = geometry3d->getVertexPointer(0);
    centroid[0] = 0.0f;
    centroid[1] = 0.0f;
    centroid[2] = 0.0f;
    for(int64_t i = 0; i < geometry3d->getNumberOfVertices(); i++)
    {
      centroid[0] += vertices[3 * i + 0];
      centroid[1] += vertices[3 * i + 1];
      centroid[2] += vertices[3 * i + 2];
    }
    centroid[0] /= static_cast<float>(geometry3d->getNumberOfVertices());
    centroid[1] /= static_cast<float>(geometry3d->getNumberOfVertices());
    centroid[2] /= static_cast<float>(geometry3d->getNumberOfVertices());
    return;
  }
  else
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void translateGeometry(IGeometry::Pointer geometry, float translation[3])
{
  if(ImageGeom::Pointer image = std::dynamic_pointer_cast<ImageGeom>(geometry))
  {
    float origin[3] = {0.0f, 0.0f, 0.0f};
    image->getOrigin(origin);
    origin[0] += translation[0];
    origin[1] += translation[1];
    origin[2] += translation[2];
    image->setOrigin(origin);
    return;
  }
  else if(RectGridGeom::Pointer rectGrid = std::dynamic_pointer_cast<RectGridGeom>(geometry))
  {
    float* xBounds = rectGrid->getXBounds()->getPointer(0);
    float* yBounds = rectGrid->getYBounds()->getPointer(0);
    float* zBounds = rectGrid->getZBounds()->getPointer(0);
    for(size_t i = 0; i < rectGrid->getXPoints(); i++)
    {
      xBounds[i] += translation[0];
    }
    for(size_t i = 0; i < rectGrid->getYPoints(); i++)
    {
      yBounds[i] += translation[1];
    }
    for(size_t i = 0; i < rectGrid->getZPoints(); i++)
    {
      zBounds[i] += translation[2];
    }
    return;
  }
  else if(VertexGeom::Pointer vertex = std::dynamic_pointer_cast<VertexGeom>(geometry))
  {
    float* vertices = vertex->getVertexPointer(0);
    for(int64_t i = 0; i < vertex->getNumberOfVertices(); i++)
    {
      vertices[3 * i + 0] += translation[0];
      vertices[3 * i + 1] += translation[1];
      vertices[3 * i + 2] += translation[2];
    }
    return;
  }
  else if(EdgeGeom::Pointer edge = std::dynamic_pointer_cast<EdgeGeom>(geometry))
  {
    float* vertices = edge->getVertexPointer(0);
    for(int64_t i = 0; i < edge->getNumberOfVertices(); i++)
    {
      vertices[3 * i + 0] += translation[0];
      vertices[3 * i + 1] += translation[1];
      vertices[3 * i + 2] += translation[2];
    }
    return;
  }
  else if(IGeometry2D::Pointer geometry2d = std::dynamic_pointer_cast<IGeometry2D>(geometry))
  {
    float* vertices = geometry2d->getVertexPointer(0);
    for(int64_t i = 0; i < geometry2d->getNumberOfVertices(); i++)
    {
      vertices[3 * i + 0] += translation[0];
      vertices[3 * i + 1] += translation[1];
      vertices[3 * i + 2] += translation[2];
    }
    return;
  }
  else if(IGeometry3D::Pointer geometry3d = std::dynamic_pointer_cast<IGeometry3D>(geometry))
  {
    float* vertices = geometry3d->getVertexPointer(0);
    for(int64_t i = 0; i < geometry3d->getNumberOfVertices(); i++)
    {
      vertices[3 * i + 0] += translation[0];
      vertices[3 * i + 1] += translation[1];
      vertices[3 * i + 2] += translation[2];
    }
    return;
  }
  else
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignGeometries::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  IGeometry::Pointer moving = getDataContainerArray()->getDataContainer(m_MovingGeometry)->getGeometry();
  IGeometry::Pointer target = getDataContainerArray()->getDataContainer(m_TargetGeometry)->getGeometry();

  if(m_AlignmentType == 0)
  {
    float movingOrigin[3] = {0.0f, 0.0f, 0.0f};
    float targetOrigin[3] = {0.0f, 0.0f, 0.0f};
    extractOrigin(moving, movingOrigin);
    extractOrigin(target, targetOrigin);

    float translation[3] = {targetOrigin[0] - movingOrigin[0], targetOrigin[1] - movingOrigin[1], targetOrigin[2] - movingOrigin[2]};
    translateGeometry(moving, translation);
  }
  else if(m_AlignmentType == 1)
  {
    float movingCentroid[3] = {0.0f, 0.0f, 0.0f};
    float targetCentroid[3] = {0.0f, 0.0f, 0.0f};
    extractCentroid(moving, movingCentroid);
    extractCentroid(target, targetCentroid);

    float translation[3] = {targetCentroid[0] - movingCentroid[0], targetCentroid[0] - movingCentroid[0], targetCentroid[0] - movingCentroid[0]};
    translateGeometry(moving, translation);
  }
  else
  {
    QString ss = QObject::tr("Invalid selection for alignment type");
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer AlignGeometries::newFilterInstance(bool copyFilterParameters) const
{
  AlignGeometries::Pointer filter = AlignGeometries::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignGeometries::getCompiledLibraryName() const
{
  return AFRLDistributionCConstants::AFRLDistributionCBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignGeometries::getBrandingString() const
{
  return "AFRLDistributionC";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignGeometries::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << AFRLDistributionC::Version::Major() << "." << AFRLDistributionC::Version::Minor() << "." << AFRLDistributionC::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignGeometries::getGroupName() const
{
  return SIMPL::FilterGroups::ReconstructionFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignGeometries::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::AlignmentFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignGeometries::getHumanLabel() const
{
  return "Align Geometries";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid AlignGeometries::getUuid()
{
  return QUuid("{ce1ee404-0336-536c-8aad-f9641c9458be}");
}