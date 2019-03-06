#ifndef _findminkowskibouliganddimension_h_
#define _findminkowskibouliganddimension_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The FindMinkowskiBouligandDimension class. See [Filter documentation](@ref findminkowskibouliganddimension) for details.
 */
class FindMinkowskiBouligandDimension : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(FindMinkowskiBouligandDimension)
  SIMPL_FILTER_NEW_MACRO(FindMinkowskiBouligandDimension)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(FindMinkowskiBouligandDimension, AbstractFilter)

  ~FindMinkowskiBouligandDimension() override;

  SIMPL_FILTER_PARAMETER(DataArrayPath, MaskArrayPath)
  Q_PROPERTY(DataArrayPath MaskArrayPath READ getMaskArrayPath WRITE setMaskArrayPath)

  SIMPL_FILTER_PARAMETER(QString, AttributeMatrixName)
  Q_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, MinkowskiBouligandDimensionArrayName)
  Q_PROPERTY(QString MinkowskiBouligandDimensionArrayName READ getMinkowskiBouligandDimensionArrayName WRITE setMinkowskiBouligandDimensionArrayName)

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
  FindMinkowskiBouligandDimension();

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
  DEFINE_DATAARRAY_VARIABLE(double, MinkowskiBouligandDimension)

  void findSourceIndices(std::vector<size_t>& indices, size_t sourceDim, size_t index, size_t box[3], size_t imageDim);

  FindMinkowskiBouligandDimension(const FindMinkowskiBouligandDimension&) = delete; // Copy Constructor Not Implemented
  FindMinkowskiBouligandDimension(FindMinkowskiBouligandDimension&&) = delete;      // Move Constructor Not Implemented
  void operator=(const FindMinkowskiBouligandDimension&) = delete;                  // Operator '=' Not Implemented
};

#endif /* _FindMinkowskiBouligandDimension_H_ */
