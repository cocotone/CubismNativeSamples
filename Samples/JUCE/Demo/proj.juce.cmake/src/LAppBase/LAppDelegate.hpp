/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "LAppAllocator.hpp"

class LAppView;
class LAppTextureManager;

/**
* @brief   アプリケーションクラス。
*   Cubism SDK の管理を行う。
*/
class LAppDelegate
{
public:
  /**
  * @brief   クラスのインスタンス（シングルトン）を返す。<br>
  *           インスタンスが生成されていない場合は内部でインスタンを生成する。
  *
  * @return  クラスのインスタンス
  */
  static LAppDelegate* GetInstance();

  /**
  * @brief   クラスのインスタンス（シングルトン）を解放する。
  *
  */
  static void ReleaseInstance();

  /**
  * @brief   APPに必要なものを初期化する。
  */
  bool Initialize();

  /**
  * @brief   解放する。
  */
  void Release();

  /**
  * @brief   Window情報を取得する。
  */
  juce::Component* GetWindow() { return _component; }
  void SetWindow(juce::Component* component);

  /**
  * @brief   View情報を取得する。
  */
  LAppView* GetView() { return _view.get(); }

  /**
  * @brief   アプリケーションを終了するかどうか。
  */
  bool GetIsEnd() { return _isEnd; }

  /**
  * @brief   アプリケーションを終了する。
  */
  void AppEnd() { _isEnd = true; }

  LAppTextureManager* GetTextureManager() { return _textureManager.get(); }

private:
  /**
  * @brief   コンストラクタ
  */
  LAppDelegate();

  /**
  * @brief   デストラクタ
  */
  ~LAppDelegate();

  /**
  * @brief   Cubism SDK の初期化
  */
  void InitializeCubism();

  LAppAllocator _cubismAllocator;              ///< Cubism SDK Allocator
  Csm::CubismFramework::Option _cubismOption;  ///< Cubism SDK Option
  juce::Component* _component {nullptr};       ///< JUCE Component
  //std::unique_ptr<juce::Component> _dummyComponent;
  std::unique_ptr<LAppView> _view;                             ///< View情報
  bool _captured;                              ///< クリックしているか
  float _mouseX;                               ///< マウスX座標
  float _mouseY;                               ///< マウスY座標
  bool _isEnd;                                 ///< APP終了しているか
  std::unique_ptr<LAppTextureManager> _textureManager;         ///< テクスチャマネージャー
};

class OpenGLShaderFactory
{
public:
    /**
    * @brief シェーダーを登録する。
    */
    static GLuint createShader();

private:
    /**
     * @brief   createShader内部関数 エラーチェック
     */
    static bool checkShader(GLuint shaderId);
};
