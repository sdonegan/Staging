

#include "StagingGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
StagingGuiPlugin::StagingGuiPlugin()
: StagingPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
StagingGuiPlugin::~StagingGuiPlugin() = default;

#include "Staging/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
