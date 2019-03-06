#ifndef _laplaciasmoothpointcloud_h_
#define _laplaciasmoothpointcloud_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The LaplacianSmoothPointCloud class. See [Filter documentation](@ref laplaciansmoothpointcloud) for details.
 */
class LaplacianSmoothPointCloud : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(LaplacianSmoothPointCloud)
  SIMPL_FILTER_NEW_MACRO(LaplacianSmoothPointCloud)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(LaplacianSmoothPointCloud, AbstractFilter)

  ~LaplacianSmoothPointCloud() override;

  SIMPL_FILTER_PARAMETER(QString, DataContainerName)
  Q_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)

  SIMPL_FILTER_PARAMETER(float, Lambda)
  Q_PROPERTY(float Lambda READ getLambda WRITE setLambda)

  SIMPL_FILTER_PARAMETER(int, NumIterations)
  Q_PROPERTY(int NumIterations READ getNumIterations WRITE setNumIterations)

  SIMPL_FILTER_PARAMETER(bool, UseMask)
  Q_PROPERTY(bool UseMask READ getUseMask WRITE setUseMask)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MaskArrayPath)
  Q_PROPERTY(DataArrayPath MaskArrayPath READ getMaskArrayPath WRITE setMaskArrayPath)

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
  LaplacianSmoothPointCloud();

  /**
   * @brief smoothPointCoordinates Smooths a point coordinate based on a window
   * @param vertex Vertex pointer
   * @param startCoords Start window coordinates
   * @param endCoords End window coordinates
   * @param vertIdx Vertex index
   */
  void determineNewCoords(float* vertex, int64_t vertId, float* newCoords);
  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  DEFINE_DATAARRAY_VARIABLE(bool, Mask)

  LaplacianSmoothPointCloud(const LaplacianSmoothPointCloud&) = delete; // Copy Constructor Not Implemented
  LaplacianSmoothPointCloud(LaplacianSmoothPointCloud&&) = delete;       // Move Constructor Not Implemented
  void operator=(const LaplacianSmoothPointCloud&) = delete;            // Operator '=' Not Implemented
};

#endif /* _laplaciansmoothpointcloud_H_ */
