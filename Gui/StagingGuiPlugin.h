#pragma once

#include "Staging/StagingPlugin.h"

class StagingGuiPlugin : public StagingPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "com.your.domain.StagingGuiPlugin")

public:
  StagingGuiPlugin();
  ~StagingGuiPlugin() override;
  
  /**
   * @brief Register all the filters with the FilterWidgetFactory
   */
  void registerFilterWidgets(FilterWidgetManager* fwm) override;

public:
  StagingGuiPlugin(const StagingGuiPlugin&) = delete;            // Copy Constructor Not Implemented
  StagingGuiPlugin(StagingGuiPlugin&&) = delete;                 // Move Constructor
  StagingGuiPlugin& operator=(const StagingGuiPlugin&) = delete; // Copy Assignment Not Implemented
  StagingGuiPlugin& operator=(StagingGuiPlugin&&) = delete;      // Move Assignment Not Implemented
};
