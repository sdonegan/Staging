/*
 * Your License or Copyright can go here
 */

#include "StagingFilter.h"

#include "SIMPLib/Common/Constants.h"



#include "Staging/StagingConstants.h"
#include "Staging/StagingVersion.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
StagingFilter::StagingFilter() 
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
StagingFilter::~StagingFilter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void StagingFilter::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void StagingFilter::setupFilterParameters()
{
  FilterParameterVector parameters;

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void StagingFilter::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void StagingFilter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void StagingFilter::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  if (getCancel()) { return; }

  if (getWarningCondition() < 0)
  {
    QString ss = QObject::tr("Some warning message");
    setWarningCondition(-88888888);
    notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());
  }

  if (getErrorCondition() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer StagingFilter::newFilterInstance(bool copyFilterParameters) const
{
  StagingFilter::Pointer filter = StagingFilter::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString StagingFilter::getCompiledLibraryName() const
{ 
  return StagingConstants::StagingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString StagingFilter::getBrandingString() const
{
  return "Staging";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString StagingFilter::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  Staging::Version::Major() << "." << Staging::Version::Minor() << "." << Staging::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString StagingFilter::getGroupName() const
{ 
  return SIMPL::FilterGroups::Unsupported; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString StagingFilter::getSubGroupName() const
{ 
  return "Staging"; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString StagingFilter::getHumanLabel() const
{ 
  return "StagingFilter"; 
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid StagingFilter::getUuid()
{
  return QUuid("{fb593a96-02f2-555e-ab75-47bba08f2c20}");
}

