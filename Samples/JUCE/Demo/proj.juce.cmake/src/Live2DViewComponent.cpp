#include "Live2DViewComponent.h"
#include "LAppBase/LAppDelegate.hpp"
#include "LAppBase/LAppView.hpp"
#include "LAppBase/LAppPal.hpp"

Live2DViewComponent::Live2DViewComponent()
    : _lastWidth(0)
    , _lastHeight(0)
    , _initialized(false)
{
}

Live2DViewComponent::~Live2DViewComponent()
{
    std::lock_guard lock(_mtx);

    LAppDelegate::GetInstance()->Release();
    shutdownOpenGL();
}

void Live2DViewComponent::initialise()
{
    //テクスチャサンプリング設定
    juce::gl::glTexParameteri(juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MAG_FILTER, juce::gl::GL_LINEAR);
    juce::gl::glTexParameteri(juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MIN_FILTER, juce::gl::GL_LINEAR);

    //透過設定
    juce::gl::glEnable(juce::gl::GL_BLEND);
    juce::gl::glBlendFunc(juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA);

    LAppDelegate::GetInstance()->Initialize();

    LAppDelegate::GetInstance()->SetWindow(this);

    _initialized = true;
}

void Live2DViewComponent::shutdown()
{
}

void Live2DViewComponent::render()
{
    std::unique_lock<std::mutex> lock(_mtx, std::try_to_lock);
    if (!lock)
    {
        DBG("OpenGL shutdowned.");
        return;
    }

    int width = this->getWidth();
    int height = this->getHeight();

    if ((_lastWidth != width || _lastHeight != height) && width > 0 && height > 0)
    {
        //AppViewの初期化
        LAppDelegate::GetInstance()->GetView()->Initialize();
        // スプライトサイズを再設定
        LAppDelegate::GetInstance()->GetView()->ResizeSprite();
        // サイズを保存しておく
        _lastWidth = width;
        _lastHeight = height;

        // ビューポート変更
        juce::gl::glViewport(0, 0, width, height);
    }

    // 時間更新
    LAppPal::UpdateTime();

    // 画面の初期化
    juce::gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    juce::gl::glClear(juce::gl::GL_COLOR_BUFFER_BIT | juce::gl::GL_DEPTH_BUFFER_BIT);
    juce::gl::glClearDepth(1.0);

    //描画更新
    LAppDelegate::GetInstance()->GetView()->Render();
}

void Live2DViewComponent::paint(juce::Graphics& g)
{
}

void Live2DViewComponent::mouseDown(const juce::MouseEvent& event)
{
    if (_initialized)
    {
        LAppDelegate::GetInstance()->GetView()->OnTouchesBegan(event.x, event.y);
    }
}

void Live2DViewComponent::mouseMove(const juce::MouseEvent& event)
{
    if (_initialized)
    {
        LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(event.x, event.y);
    }
}

void Live2DViewComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (_initialized)
    {
        LAppDelegate::GetInstance()->GetView()->OnTouchesEnded(event.x, event.y);
    }
}
