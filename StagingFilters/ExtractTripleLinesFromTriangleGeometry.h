#ifndef _extracttriplelinesfromtrianglegeometry_h_
#define _extracttriplelinesfromtrianglegeometry_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The ExtractTripleLinesFromTriangleGeometry class. See [Filter documentation](@ref extracttriplelinesfromtrianglegeometry) for details.
 */
class ExtractTripleLinesFromTriangleGeometry : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ExtractTripleLinesFromTriangleGeometry)
  SIMPL_FILTER_NEW_MACRO(ExtractTripleLinesFromTriangleGeometry)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ExtractTripleLinesFromTriangleGeometry, AbstractFilter)

  ~ExtractTripleLinesFromTriangleGeometry() override;

  SIMPL_FILTER_PARAMETER(DataArrayPath, NodeTypesArrayPath)
  Q_PROPERTY(DataArrayPath NodeTypesArrayPath READ getNodeTypesArrayPath WRITE setNodeTypesArrayPath)

  SIMPL_FILTER_PARAMETER(QString, EdgeGeometry)
  Q_PROPERTY(QString EdgeGeometry READ getEdgeGeometry WRITE setEdgeGeometry)

  SIMPL_FILTER_PARAMETER(QString, VertexAttributeMatrixName)
  Q_PROPERTY(QString VertexAttributeMatrixName READ getVertexAttributeMatrixName WRITE setVertexAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, EdgeAttributeMatrixName)
  Q_PROPERTY(QString EdgeAttributeMatrixName READ getEdgeAttributeMatrixName WRITE setEdgeAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, NodeTypesArrayName)
  Q_PROPERTY(QString NodeTypesArrayName READ getNodeTypesArrayName WRITE setNodeTypesArrayName)

  SIMPL_FILTER_PARAMETER(bool, SmoothTripleLines)
  Q_PROPERTY(bool SmoothTripleLines READ getSmoothTripleLines WRITE setSmoothTripleLines)

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
  ExtractTripleLinesFromTriangleGeometry();

  void extractTripleLines();

  void smoothTripleLines();

  std::vector<float> computeBsplineInterpolation(std::vector<std::vector<float>> points, float minVal, float maxVal);

  /**
  * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
  */
  void dataCheck();

  /**
  * @brief Initializes all the private instance variables.
  */
  void initialize();

private:
  DEFINE_DATAARRAY_VARIABLE(int8_t, NodeTypes)
  DEFINE_DATAARRAY_VARIABLE(int8_t, TripleLineNodeTypes)

  ExtractTripleLinesFromTriangleGeometry(const ExtractTripleLinesFromTriangleGeometry&) = delete; // Copy Constructor Not Implemented
  ExtractTripleLinesFromTriangleGeometry(ExtractTripleLinesFromTriangleGeometry&&) = delete;      // Move Constructor Not Implemented
  void operator=(const ExtractTripleLinesFromTriangleGeometry&) = delete;                         // Operator '=' Not Implemented
};

#endif /* _ExtractTripleLinesFromTriangleGeometry_H_ */
