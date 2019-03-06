#ifndef _ReadBinaryCTXRadia_h_
#define _ReadBinaryCTXRadia_h_

#include <QtCore/QFile>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/SIMPLib.h"

/**
 * @brief The ReadBinaryCTXRadia class. See [Filter documentation](@ref ReadBinaryCTXRadia) for details.
 */
class ReadBinaryCTXRadia : public AbstractFilter
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ReadBinaryCTXRadia)
  SIMPL_FILTER_NEW_MACRO(ReadBinaryCTXRadia)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ReadBinaryCTXRadia, AbstractFilter)

  ~ReadBinaryCTXRadia() override;

  SIMPL_FILTER_PARAMETER(QString, InputFile)
  Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

  SIMPL_FILTER_PARAMETER(QString, InputHeaderFile)
  Q_PROPERTY(QString InputHeaderFile READ getInputHeaderFile WRITE setInputHeaderFile)

  SIMPL_FILTER_PARAMETER(QString, DataContainerName)
  Q_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, DensityArrayName)
  Q_PROPERTY(QString DensityArrayName READ getDensityArrayName WRITE setDensityArrayName)

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
  ReadBinaryCTXRadia();

  /**
   * @brief sanityCheckFileSizeVersusAllocatedSize Ensures the allocated array and the raw
   * binary file have the same number of bytes
   * @param allocatedBytes Number of bytes allocated
   * @param fileSize Size of the raw binary file
   * @return Integer error code
   */
  int32_t sanityCheckFileSizeVersusAllocatedSize(size_t allocatedBytes, size_t fileSize);

  /**
   * @brief readBinaryCTFile Reads the raw binary CT file
   * @return Integer error code
   */
  int32_t readBinaryCTFile(size_t dims[3]);

  /**
   * @brief readHeaderMetaData Reads the number of voxels and voxel extents
   * from the NSI header file
   * @return Integer error code
   */
  int32_t readHeaderMetaData(ImageGeom::Pointer image);
  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  QFile m_InHeaderStream;
  QFile m_InStream;

  DEFINE_DATAARRAY_VARIABLE(float, Density)

  ReadBinaryCTXRadia(const ReadBinaryCTXRadia&) = delete; // Copy Constructor Not Implemented
  ReadBinaryCTXRadia(ReadBinaryCTXRadia&&) = delete;      // Move Constructor Not Implemented
  void operator=(const ReadBinaryCTXRadia&) = delete;     // Operator '=' Not Implemented
};

#endif /* _ReadBinaryCTXRadia_H_ */
