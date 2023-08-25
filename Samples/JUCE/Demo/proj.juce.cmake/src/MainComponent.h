#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "Live2DViewComponent.h"

//==============================================================================
class MainComponent
  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    virtual~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    std::unique_ptr<Live2DViewComponent> _live2dView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
