#ifndef _combinestlfiles_h_
#define _combinestlfiles_h_

#include <QDir>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The CombineStlFiles class. See [Filter documentation](@ref combinestlfiles) for details.
 */
class CombineStlFiles : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(CombineStlFiles)
  SIMPL_FILTER_NEW_MACRO(CombineStlFiles)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(CombineStlFiles, AbstractFilter)

  ~CombineStlFiles() override;

  SIMPL_FILTER_PARAMETER(QString, StlFilesPath)
  Q_PROPERTY(QString StlFilesPath READ getStlFilesPath WRITE setStlFilesPath)

  SIMPL_FILTER_PARAMETER(QString, TriangleDataContainerName)
  Q_PROPERTY(QString TriangleDataContainerName READ getTriangleDataContainerName WRITE setTriangleDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, FaceAttributeMatrixName)
  Q_PROPERTY(QString FaceAttributeMatrixName READ getFaceAttributeMatrixName WRITE setFaceAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, FaceNormalsArrayName)
  Q_PROPERTY(QString FaceNormalsArrayName READ getFaceNormalsArrayName WRITE setFaceNormalsArrayName)

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
  CombineStlFiles();

  /**
  * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
  */
  void dataCheck();

  /**
  * @brief Initializes all the private instance variables.
  */
  void initialize();

private:
  DEFINE_DATAARRAY_VARIABLE(double, FaceNormals);

  QFileInfoList m_FileList;

  CombineStlFiles(const CombineStlFiles&) = delete; // Copy Constructor Not Implemented
  CombineStlFiles(CombineStlFiles&&) = delete;      // Move Constructor Not Implemented
  void operator=(const CombineStlFiles&) = delete;  // Operator '=' Not Implemented
};

#endif /* _CombineStlFiles_H_ */
