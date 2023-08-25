#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_opengl/juce_opengl.h>

//==============================================================================
class Live2DViewComponent
    : public juce::OpenGLAppComponent
{
public:
    //==============================================================================
    Live2DViewComponent();
    virtual ~Live2DViewComponent() override;

private:
    //==============================================================================
    virtual void initialise() override;
    virtual void shutdown() override;
    virtual void render() override;

    //==============================================================================
    virtual void paint(juce::Graphics& g) override;

    virtual void mouseDown(const juce::MouseEvent& event) override;
    virtual void mouseMove(const juce::MouseEvent& event) override;
    virtual void mouseDrag(const juce::MouseEvent& event) override;

    //==============================================================================
    int _lastWidth;
    int _lastHeight;

    mutable std::mutex _mtx;
    std::atomic<bool> _initialized;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Live2DViewComponent)
};
