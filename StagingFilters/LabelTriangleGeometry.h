#ifndef _LabelCADFile_h_
#define _LabelCADFile_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The LabelCADFile class. See [Filter documentation](@ref labelcadfile) for details.
 */
class LabelCADFile : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(LabelCADFile)
  SIMPL_FILTER_NEW_MACRO(LabelCADFile)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(LabelCADFile, AbstractFilter)

  ~LabelCADFile() override;

  SIMPL_FILTER_PARAMETER(QString, CADDataContainerName)
  Q_PROPERTY(QString CADDataContainerName READ getCADDataContainerName WRITE setCADDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, TriangleAttributeMatrixName)
  Q_PROPERTY(QString TriangleAttributeMatrixName READ getTriangleAttributeMatrixName WRITE setTriangleAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, RegionIdArrayName)
  Q_PROPERTY(QString RegionIdArrayName READ getRegionIdArrayName WRITE setRegionIdArrayName)

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
  LabelCADFile();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
  * @brief updateEdgeInstancePointers
  */
  void updateTriangleInstancePointers();

private:
  DEFINE_DATAARRAY_VARIABLE(int32_t, RegionId)

  LabelCADFile(const LabelCADFile&) = delete;   // Copy Constructor Not Implemented
  LabelCADFile(LabelCADFile&&) = delete;        // Move Constructor Not Implemented
  void operator=(const LabelCADFile&) = delete; // Operator '=' Not Implemented
};

#endif /* _LabelCADFile_H_ */
