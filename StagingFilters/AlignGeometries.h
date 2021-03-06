#ifndef _aligngeometries_h_
#define _aligngeometries_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The AlignGeometries class. See [Filter documentation](@ref aligngeometries) for details.
 */
class AlignGeometries : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(AlignGeometries)
  SIMPL_FILTER_NEW_MACRO(AlignGeometries)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(AlignGeometries, AbstractFilter)

  ~AlignGeometries() override;

  SIMPL_FILTER_PARAMETER(QString, MovingGeometry)
  Q_PROPERTY(QString MovingGeometry READ getMovingGeometry WRITE setMovingGeometry)

  SIMPL_FILTER_PARAMETER(QString, TargetGeometry)
  Q_PROPERTY(QString TargetGeometry READ getTargetGeometry WRITE setTargetGeometry)

  SIMPL_FILTER_PARAMETER(int, AlignmentType)
  Q_PROPERTY(int AlignmentType READ getAlignmentType WRITE setAlignmentType)

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
  AlignGeometries();

  /**
  * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
  */
  void dataCheck();

  /**
  * @brief Initializes all the private instance variables.
  */
  void initialize();

private:
  AlignGeometries(const AlignGeometries&); // Copy Constructor Not Implemented
  AlignGeometries(AlignGeometries&&);      // Move Constructor Not Implemented
  void operator=(const AlignGeometries&);  // Operator '=' Not Implemented
};

#endif /* _aligngeometries_h_ */
