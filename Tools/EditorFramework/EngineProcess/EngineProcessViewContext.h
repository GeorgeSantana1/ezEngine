#pragma once

#include <EditorFramework/Plugin.h>
#include <System/Window/Window.h>
#include <RendererFoundation/Device/Device.h>
#include <CoreUtils/Graphics/Camera.h>
#include <Core/ResourceManager/ResourceHandle.h>

class ezEngineProcessDocumentContext;
class ezEditorEngineDocumentMsg;
class ezViewRedrawMsgToEngine;
class ezEditorEngineViewMsg;
class ezView;
typedef ezTypedResourceHandle<class ezRenderPipelineResource> ezRenderPipelineResourceHandle;

/// \brief Represents the window inside the editor process, into which the engine process renders
class EZ_EDITORFRAMEWORK_DLL ezEditorProcessViewWindow : public ezWindowBase
{
public:
  ezEditorProcessViewWindow()
  {
    m_hWnd = 0;
    m_uiWidth = 0;
    m_uiHeight = 0;
  }


  virtual ezSizeU32 GetClientAreaSize() const override { return ezSizeU32(m_uiWidth, m_uiHeight); }
  virtual ezWindowHandle GetNativeWindowHandle() const override { return m_hWnd; }
  virtual void ProcessWindowMessages() override { }

  ezWindowHandle m_hWnd;
  ezUInt16 m_uiWidth;
  ezUInt16 m_uiHeight;
};

/// \brief Represents the view/window on the engine process side, holds all data necessary for rendering
class EZ_EDITORFRAMEWORK_DLL ezEngineProcessViewContext
{
public:
  ezEngineProcessViewContext(ezEngineProcessDocumentContext* pContext);
  virtual ~ezEngineProcessViewContext();

  ezEditorProcessViewWindow& GetEditorWindow() { return m_Window; }

  ezEngineProcessDocumentContext* GetDocumentContext() const { return m_pDocumentContext; }

  virtual void HandleViewMessage(const ezEditorEngineViewMsg* pMsg);

  void SendViewMessage(ezEditorEngineDocumentMsg* pViewMsg);

protected:
  virtual void SetupRenderTarget(ezWindowHandle hWnd, ezUInt16 uiWidth, ezUInt16 uiHeight);
  virtual void Redraw();
  virtual void SetCamera(const ezViewRedrawMsgToEngine* pMsg);

  /// \brief Returns true if passed in handle matches the one created by CreateRenderPipeline.
  virtual bool IsDefaultRenderPipeline(ezRenderPipelineResourceHandle hPipeline) = 0;
  /// \brief Returns the handle to the default render pipeline.
  virtual ezRenderPipelineResourceHandle CreateDefaultRenderPipeline() = 0;
  /// \brief Create the actual view.
  virtual ezView* CreateView() = 0;

private:
  ezEngineProcessDocumentContext* m_pDocumentContext;
  ezEditorProcessViewWindow m_Window;

protected:
  ezCamera m_Camera;
  ezView* m_pView;
};


