#ifndef _interpolatemeshtoregulargrid_h_
#define _interpolatemeshtoregulargrid_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The InterpolateMeshToRegularGrid class. See [Filter documentation](@ref interpolatemeshtoregulargrid) for details.
 */
class InterpolateMeshToRegularGrid : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(InterpolateMeshToRegularGrid)
  SIMPL_FILTER_NEW_MACRO(InterpolateMeshToRegularGrid)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(InterpolateMeshToRegularGrid, AbstractFilter)

  ~InterpolateMeshToRegularGrid() override;

  SIMPL_FILTER_PARAMETER(QString, SelectedDataContainerName)
  Q_PROPERTY(QString SelectedDataContainerName READ getSelectedDataContainerName WRITE setSelectedDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, InterpolatedDataContainerName)
  Q_PROPERTY(QString InterpolatedDataContainerName READ getInterpolatedDataContainerName WRITE setInterpolatedDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, InterpolatedAttributeMatrixName)
  Q_PROPERTY(QString InterpolatedAttributeMatrixName READ getInterpolatedAttributeMatrixName WRITE setInterpolatedAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(int, ScaleOrSpecifyNumCells)
  Q_PROPERTY(int ScaleOrSpecifyNumCells READ getScaleOrSpecifyNumCells WRITE setScaleOrSpecifyNumCells)

  SIMPL_FILTER_PARAMETER(int, SetXDimension)
  Q_PROPERTY(int SetXDimension READ getSetXDimension WRITE setSetXDimension)

  SIMPL_FILTER_PARAMETER(int, SetYDimension)
  Q_PROPERTY(int SetYDimension READ getSetYDimension WRITE setSetYDimension)

  SIMPL_FILTER_PARAMETER(int, ScaleFactorNumCells)
  Q_PROPERTY(int ScaleFactorNumCells READ getScaleFactorNumCells WRITE setScaleFactorNumCells)

  SIMPL_FILTER_PARAMETER(int, OutsideMeshIdentifier)
  Q_PROPERTY(int OutsideMeshIdentifier READ getOutsideMeshIdentifier WRITE setOutsideMeshIdentifier)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
  */
  const QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  const QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
  * @brief preflight Reimplemented from @see AbstractFilter class
  */
  void preflight() override;

  /**
  * @brief getUuid Return the unique identifier for this filter.
  * @return A QUuid object.
  */
  const QUuid getUuid() override;

signals:
  /**
    * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  InterpolateMeshToRegularGrid();

  /**
   * @brief createRegularGrid Creates the structured rectilinear grid to interpolate the mesh
   * data onto
   */
  void createRegularGrid();

  /**
   * @brief interpolateMeshPointsToGrid Performs the interpolation between the selected mesh and
   * the created grid
   * @param inputMesh Source mesh
   * @param interpolatedGrid Destination grid
   */
  void interpolateMeshPointsToGrid(DataContainer::Pointer inputMesh, DataContainer::Pointer interpolatedGrid);
  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  QList<QString> m_AttrMatList;
  QMap<QString, QList<QString>> m_AttrArrayMap;
  std::vector<float> m_MeshMinExtents;
  std::vector<float> m_MeshMaxExtents;
  std::vector<bool> m_InsideMesh;
  std::vector<size_t> m_InterpolatedIndex;

  InterpolateMeshToRegularGrid(const InterpolateMeshToRegularGrid&) = delete; // Copy Constructor Not Implemented
  InterpolateMeshToRegularGrid(InterpolateMeshToRegularGrid&&) = delete;      // Move Constructor Not Implemented
  void operator=(const InterpolateMeshToRegularGrid&) = delete;               // Operator '=' Not Implemented
};

#endif /* _InterpolateMeshToRegularGrid_H_ */
