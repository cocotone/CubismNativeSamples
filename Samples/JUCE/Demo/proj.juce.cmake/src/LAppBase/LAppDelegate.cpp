/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppDelegate.hpp"
#include <iostream>
#include "LAppView.hpp"
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppTextureManager.hpp"

using namespace Csm;
using namespace std;
using namespace LAppDefine;

#include <juce_opengl/juce_opengl.h>

namespace {
  LAppDelegate* s_instance = NULL;
}

LAppDelegate* LAppDelegate::GetInstance()
{
  if (s_instance == NULL)
  {
    s_instance = new LAppDelegate();
  }

  return s_instance;
}

void LAppDelegate::ReleaseInstance()
{
  if (s_instance != NULL)
  {
    delete s_instance;
  }

  s_instance = NULL;
}

LAppDelegate::LAppDelegate() :
  _cubismOption(),
  _captured(false),
  _mouseX(0.0f),
  _mouseY(0.0f),
  _isEnd(false),
  _component(nullptr)
{
}

LAppDelegate::~LAppDelegate()
{
}

bool LAppDelegate::Initialize()
{
  _view = std::make_unique<LAppView>();
  _textureManager = std::make_unique<LAppTextureManager>();

  if (DebugLogEnable)
  {
     LAppPal::PrintLog("START");
  }

  //AppViewの初期化
  _view->Initialize();

  // Cubism SDK の初期化
  InitializeCubism();

  return true;
}

void LAppDelegate::Release()
{
  _view.reset();
  _textureManager.reset();

  // リソースを解放
  LAppLive2DManager::ReleaseInstance();

  //Cubism SDK の解放
  CubismFramework::Dispose();
}

void LAppDelegate::InitializeCubism()
{
  //setup cubism
  _cubismOption.LogFunction = LAppPal::PrintMessage;
  _cubismOption.LoggingLevel = LAppDefine::CubismLoggingLevel;
  Csm::CubismFramework::StartUp(&_cubismAllocator, &_cubismOption);

  //Initialize cubism
  CubismFramework::Initialize();

  //load model
  LAppLive2DManager::GetInstance();

  //default proj
  CubismMatrix44 projection;

  LAppPal::UpdateTime();

  _view->InitializeSprite();
}

void LAppDelegate::SetWindow(juce::Component* component)
{
    _component = component;
}

GLuint OpenGLShaderFactory::createShader()
{
    //バーテックスシェーダのコンパイル
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShader =
        "#version 120\n"
        "attribute vec3 position;"
        "attribute vec2 uv;"
        "varying vec2 vuv;"
        "void main(void){"
        "    gl_Position = vec4(position, 1.0);"
        "    vuv = uv;"
        "}";
    glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderId);
    if (!OpenGLShaderFactory::checkShader(vertexShaderId))
    {
        return 0;
    }

    //フラグメントシェーダのコンパイル
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShader =
        "#version 120\n"
        "varying vec2 vuv;"
        "uniform sampler2D texture;"
        "uniform vec4 baseColor;"
        "void main(void){"
        "    gl_FragColor = texture2D(texture, vuv) * baseColor;"
        "}";
    glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(fragmentShaderId);
    if (!OpenGLShaderFactory::checkShader(fragmentShaderId))
    {
        return 0;
    }

    //プログラムオブジェクトの作成
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    // リンク
    glLinkProgram(programId);

    glUseProgram(programId);

    return programId;
}

bool OpenGLShaderFactory::checkShader(GLuint shaderId)
{
    GLint status;
    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* log = reinterpret_cast<GLchar*>(CSM_MALLOC(logLength));
        glGetShaderInfoLog(shaderId, logLength, &logLength, log);
        CubismLogError("Shader compile log: %s", log);
        CSM_FREE(log);
    }

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glDeleteShader(shaderId);
        return false;
    }

    return true;
}
