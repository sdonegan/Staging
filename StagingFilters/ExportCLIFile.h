#ifndef _exportclifile_h_
#define _exportclifile_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The ExportCLIFile class. See [Filter documentation](@ref exportclifile) for details.
 */
class ExportCLIFile : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ExportCLIFile)
  SIMPL_FILTER_NEW_MACRO(ExportCLIFile)
  SIMPL_TYPE_MACRO_SUPER(ExportCLIFile, AbstractFilter)

  ~ExportCLIFile() override;

  SIMPL_FILTER_PARAMETER(QString, EdgeGeometry)
  Q_PROPERTY(QString EdgeGeometry READ getEdgeGeometry WRITE setEdgeGeometry)

  SIMPL_FILTER_PARAMETER(DataArrayPath, LayerIdsArrayPath)
  Q_PROPERTY(DataArrayPath LayerIdsArrayPath READ getLayerIdsArrayPath WRITE setLayerIdsArrayPath)

  SIMPL_FILTER_PARAMETER(bool, SplitByGroup)
  Q_PROPERTY(bool SplitByGroup READ getSplitByGroup WRITE setSplitByGroup)

  SIMPL_FILTER_PARAMETER(DataArrayPath, GroupIdsArrayPath)
  Q_PROPERTY(DataArrayPath GroupIdsArrayPath READ getGroupIdsArrayPath WRITE setGroupIdsArrayPath)

  SIMPL_FILTER_PARAMETER(QString, OutputDirectory)
  Q_PROPERTY(QString OutputDirectory READ getOutputDirectory WRITE setOutputDirectory)

  SIMPL_FILTER_PARAMETER(QString, OutputFilePrefix)
  Q_PROPERTY(QString OutputFilePrefix READ getOutputFilePrefix WRITE setOutputFilePrefix)

  SIMPL_FILTER_PARAMETER(double, UnitsScaleFactor)
  Q_PROPERTY(double UnitsScaleFactor READ getUnitsScaleFactor WRITE setUnitsScaleFactor)

  SIMPL_FILTER_PARAMETER(int, Precision)
  Q_PROPERTY(int Precision READ getPrecision WRITE setPrecision)

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
  ExportCLIFile();

  /**
  * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
  */
  void dataCheck();

  /**
  * @brief Initializes all the private instance variables.
  */
  void initialize();

private:
  DEFINE_DATAARRAY_VARIABLE(int32_t, LayerIds)
  DEFINE_DATAARRAY_VARIABLE(int32_t, GroupIds)

  ExportCLIFile(const ExportCLIFile&);  // Copy Constructor Not Implemented
  ExportCLIFile(ExportCLIFile&&);       // Move Constructor Not Implemented
  void operator=(const ExportCLIFile&); // Operator '=' Not Implemented
};

#endif /* _exportclifile_h_ */
