#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    _live2dView = std::make_unique<Live2DViewComponent>();
    
    this->addAndMakeVisible(_live2dView.get());

    setSize (800, 600);
}

MainComponent::~MainComponent()
{
    _live2dView.reset();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    _live2dView->setBounds(getLocalBounds());
}
