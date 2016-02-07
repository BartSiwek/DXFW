#include <string>
#include <memory>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>
#endif

#include <dxfw/dxfw.h>

class DxfwGuard {
public:
  DxfwGuard() : m_is_initialized_(false) {
    m_is_initialized_ = dxfwInitialize();
  }

  DxfwGuard(const DxfwGuard&) = delete;
  DxfwGuard& operator=(const DxfwGuard&) = delete;
  DxfwGuard(DxfwGuard&&) = delete;
  DxfwGuard& operator=(DxfwGuard&&) = delete;

  ~DxfwGuard() {
    if (m_is_initialized_) {
      dxfwTerminate();
    }
  }

  bool IsInitialized() {
    return m_is_initialized_;
  }

private:
  bool m_is_initialized_;
};

class DxfwWindowGuard {
public:
  DxfwWindowGuard(uint32_t width, uint32_t height, const char* title) : m_window_(nullptr) {
    m_window_ = dxfwCreateWindow(width, height, title);
  }

  DxfwWindowGuard(const DxfwWindowGuard&) = delete;
  DxfwWindowGuard& operator=(const DxfwWindowGuard&) = delete;
  DxfwWindowGuard(DxfwWindowGuard&&) = delete;
  DxfwWindowGuard& operator=(DxfwWindowGuard&&) = delete;

  ~DxfwWindowGuard() {
    if (m_window_ != nullptr) {
      dxfwDestroyWindow(m_window_);
    }
  }

  operator bool() const {
    return (m_window_ != nullptr);
  }

  const dxfwWindow* Get() const {
    return m_window_;
  }

  dxfwWindow* Get() {
    return m_window_;
  }

private:
  dxfwWindow* m_window_;
};

void ErrorCallback(dxfwError error) {
  std::wstring error_message = std::to_wstring(error);
  MessageBox(NULL, error_message.c_str(), L"DXFW Error", MB_OK);
}

bool InitializeDirect3d11() {
/*
  HRESULT hr = S_OK;

  UINT create_device_flags = 0;
#ifdef _DEBUG
  create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driver_types[] = {
    D3D_DRIVER_TYPE_HARDWARE,
  };
  UINT num_driver_types = 1;

  D3D_FEATURE_LEVEL feature_levels[] = {
    D3D_FEATURE_LEVEL_11_0
  };
  UINT num_feature_levels = 1;

  // Describe our SwapChain
  DXGI_SWAP_CHAIN_DESC swap_chain_desc;

  ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

  HWND window_handle = m_window_.GetHandle();

  swap_chain_desc.BufferCount = 1;
  swap_chain_desc.BufferDesc.Width = 800;  // Get from window
  swap_chain_desc.BufferDesc.Height = 600;  // Get from window
  swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
  swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
  swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.OutputWindow = window_handle;
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = 0;
  swap_chain_desc.Windowed = TRUE;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  IDXGISwapChain* swap_chain = NULL;
  for (UINT driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++) {
    D3D_DRIVER_TYPE driver_type = driver_types[driver_type_index];
    D3D_FEATURE_LEVEL result_feature_level;
    hr = D3D11CreateDeviceAndSwapChain(NULL, driver_type, NULL, create_device_flags, feature_levels,
      num_feature_levels, D3D11_SDK_VERSION, &swap_chain_desc,
      &swap_chain, &m_d3d11_device_, &result_feature_level,
      &m_d3d11_device_context_);

    if (SUCCEEDED(hr)) {
      break;
    }

    if (FAILED(hr)) {
      DXTrace(__FILE__, __LINE__, hr, DXGetErrorDescription(hr), TRUE);
    }
  }

  // Create our BackBuffer
  ID3D11Texture2D* back_buffer;
  hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
  if (FAILED(hr)) {
    DXTrace(__FILE__, __LINE__, hr, DXGetErrorDescription(hr), TRUE);
    return false;
  }

  // Create our Render Target
  hr = m_d3d11_device_->CreateRenderTargetView(back_buffer, NULL, &m_render_target_view_);
  if (FAILED(hr)) {
    DXTrace(__FILE__, __LINE__, hr, DXGetErrorDescription(hr), TRUE);
    return false;
  }

  back_buffer->Release();

  // Set our Render Target
  m_d3d11_device_context_->OMSetRenderTargets(1, &m_render_target_view_, NULL);

  // Set the swaphain for window
  m_window_.SetSwapChain(swap_chain);

  // Return success
  return true;
  */
  return true;
}

int main(int /* argc */, char** /* argv */) {
  DxfwGuard dxfw_guard;
  if (!dxfw_guard.IsInitialized()) {
    return -1;
  }

  dxfwSetErrorCallback(ErrorCallback);

  DxfwWindowGuard window(800, 600, "Hello DirectX");

  if (!window) {
    dxfwTerminate();
    return -1;
  }

  bool direct3d11_ok = InitializeDirect3d11();
  if (!direct3d11_ok) {
    return -1;
  }

  // Initialize Scene

  while (!dxfwShouldWindowClose(window.Get())) {
    // Render
    // Swap buffers
    dxfwPollOsEvents();
  }

  return 0;
}
