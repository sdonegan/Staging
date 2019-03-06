/*
 * Your License should go here
 */
#ifndef _stagingconstants_h_
#define _stagingconstants_h_

#include <QtCore/QString>

/**
* @brief This namespace is used to define some Constants for the plugin itself.
*/
namespace StagingConstants
{
  const QString StagingPluginFile("StagingPlugin");
  const QString StagingPluginDisplayName("Staging");
  const QString StagingBaseName("Staging");

  namespace FilterGroups
  {
  	const QString StagingFilters("Staging");
  }
}

/**
* @brief Use this namespace to define any custom GUI widgets that collect FilterParameters
* for a filter. Do NOT define general reusable widgets here.
*/
namespace FilterParameterWidgetType
{
/* const QString SomeCustomWidget("SomeCustomWidget"); */
}
#endif
