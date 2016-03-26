#include <string>
#include <memory>
#include <sstream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <wrl.h>
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

class DxfwWindowDeleter {
 public:
  void operator()(dxfwWindow* window) const {
    dxfwDestroyWindow(window);
  }
};

struct Vertex {
  Vertex() {
  }

  Vertex(float x, float y, float z, float r, float g, float b, float a) : position(x, y, z), color(r, g, b, a) {
  }

  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

struct DirectXState {
  Microsoft::WRL::ComPtr<ID3D11Device> device;
  Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
};

struct Scene {
  Microsoft::WRL::ComPtr<ID3D11Buffer> triangle_vertex_buffer;
  Microsoft::WRL::ComPtr<ID3DBlob> vs_buffer;
  Microsoft::WRL::ComPtr<ID3DBlob> ps_buffer;
  Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> vertex_layout;
};

void ErrorCallback(dxfwError error) {
  DXFW_ERROR_TRACE(__FILE__, __LINE__, true, error);
}

void GetBasePath(TCHAR* path) {
  GetModuleFileName(nullptr, path, MAX_PATH);
  PathRemoveFileSpec(path);
}

bool InitializeDeviceAndSwapChain(dxfwWindow* window, DirectXState* state) {
  // Device settings
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

  // SwapChain settings
  DXGI_SWAP_CHAIN_DESC swap_chain_desc;
  ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

  HWND window_handle = dxfwGetHandle(window);
  uint32_t width;
  uint32_t height;
  dxfwGetWindowSize(window, &width, &height);

  swap_chain_desc.BufferCount = 1;
  swap_chain_desc.BufferDesc.Width = width;
  swap_chain_desc.BufferDesc.Height = height;
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

  // Create
  auto hr = S_OK;
  for (decltype(num_driver_types) driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++) {
    D3D_DRIVER_TYPE driver_type = driver_types[driver_type_index];
    D3D_FEATURE_LEVEL result_feature_level;
    hr = D3D11CreateDeviceAndSwapChain(NULL, driver_type, NULL, create_device_flags, feature_levels,
      num_feature_levels, D3D11_SDK_VERSION, &swap_chain_desc,
      state->swap_chain.GetAddressOf(), state->device.GetAddressOf(),
      &result_feature_level, state->device_context.GetAddressOf());

    if (SUCCEEDED(hr)) {
      break;
    }

    if (FAILED(hr)) {
      DXFW_DIRECTX_TRACE(__FILE__, __LINE__, false, hr);
    }
  }

  if (FAILED(hr)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, hr);
    return false;
  }

  return true;
}

bool InitializeDirect3d11(dxfwWindow* window, DirectXState* state) {
  // Create device
  bool device_ok = InitializeDeviceAndSwapChain(window, state);
  if (!device_ok) {
    return false;
  }

  // Create our BackBuffer
  ID3D11Texture2D* back_buffer;
  auto back_buffer_result = state->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
  if (FAILED(back_buffer_result)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, back_buffer_result);
    return false;
  }

  // Create our Render Target
  auto rtv_result = state->device->CreateRenderTargetView(back_buffer, NULL, state->render_target_view.GetAddressOf());
  if (FAILED(rtv_result)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, rtv_result);
    return false;
  }

  // Back buffer is held by RTV, so we can release it here
  back_buffer->Release();

  // Set our Render Target
  state->device_context->OMSetRenderTargets(1, state->render_target_view.GetAddressOf(), NULL);

  return true;
}

bool InitializeVertexShader(TCHAR* base_path, ID3D11Device* device, ID3DBlob** buffer, ID3D11VertexShader** vs) {
  Microsoft::WRL::ComPtr<ID3DBlob> error_blob;

  TCHAR full_path[MAX_PATH];
  PathCombine(full_path, base_path, TEXT("shaders.hlsl"));

  HRESULT shader_compilation_result = D3DCompileFromFile(full_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_4_0", 0, 0, buffer, error_blob.GetAddressOf());
  if (FAILED(shader_compilation_result))
  {
    if (error_blob) {
      OutputDebugStringA((const char*)error_blob->GetBufferPointer());
    }
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, shader_compilation_result);
    return false;
  }

  HRESULT shader_creation_result = device->CreateVertexShader((*buffer)->GetBufferPointer(), (*buffer)->GetBufferSize(), NULL, vs);
  if (FAILED(shader_creation_result)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, shader_creation_result);
    return false;
  }

  return true;
}

bool InitializePixelShader(TCHAR* base_path, ID3D11Device* device, ID3DBlob** buffer, ID3D11PixelShader** ps) {
  Microsoft::WRL::ComPtr<ID3DBlob> error_blob;

  TCHAR full_path[MAX_PATH];
  PathCombine(full_path, base_path, TEXT("shaders.hlsl"));

  HRESULT shader_compilation_result = D3DCompileFromFile(full_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_4_0", 0, 0, buffer, error_blob.GetAddressOf());
  if (FAILED(shader_compilation_result))
  {
    if (error_blob) {
      DXFW_TRACE(__FILE__, __LINE__, false, (const char*)error_blob->GetBufferPointer());
    }
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, shader_compilation_result);
    return false;
  }

  HRESULT shader_creation_result = device->CreatePixelShader((*buffer)->GetBufferPointer(), (*buffer)->GetBufferSize(), NULL, ps);
  if (FAILED(shader_creation_result)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, shader_creation_result);
    return false;
  }

  return true;
}

bool CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer** triangle_vertex_buffer) {
  Vertex v[] = {
    Vertex(0.0f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
    Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
    Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
  };

  D3D11_BUFFER_DESC vertexBufferDesc;
  ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA vertexBufferData;
  ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
  vertexBufferData.pSysMem = v;

  HRESULT create_vertex_buffer_result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, triangle_vertex_buffer);
  
  if (FAILED(create_vertex_buffer_result)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, create_vertex_buffer_result);
    return false;
  }

  return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vs_buffer, ID3D11InputLayout** vertex_layout) {
  D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT layout_elements_count = 2;

  HRESULT create_input_layout_result = device->CreateInputLayout(layout, layout_elements_count, vs_buffer->GetBufferPointer(), vs_buffer->GetBufferSize(), vertex_layout);
  if (FAILED(create_input_layout_result)) {
    DXFW_DIRECTX_TRACE(__FILE__, __LINE__, true, create_input_layout_result);
    return false;
  }

  return true;
}

void CreateViewport(dxfwWindow* window, D3D11_VIEWPORT* viewport) {
  ZeroMemory(viewport, sizeof(D3D11_VIEWPORT));

  uint32_t width;
  uint32_t height;
  dxfwGetWindowSize(window, &width, &height);

  viewport->TopLeftX = 0;
  viewport->TopLeftY = 0;
  viewport->Width = static_cast<float>(width);
  viewport->Height = static_cast<float>(height);
}

bool InitializeScene(TCHAR* base_path, dxfwWindow* window, DirectXState* state, Scene* scene) {
  bool vs_ok = InitializeVertexShader(base_path, state->device.Get(), scene->vs_buffer.GetAddressOf(), scene->vs.GetAddressOf());
  if (!vs_ok) {
    return false;
  }

  bool ps_ok = InitializePixelShader(base_path, state->device.Get(), scene->ps_buffer.GetAddressOf(), scene->ps.GetAddressOf());
  if (!ps_ok) {
    return false;
  }

  state->device_context->VSSetShader(scene->vs.Get(), 0, 0);
  state->device_context->PSSetShader(scene->ps.Get(), 0, 0);

  bool vb_ok = CreateVertexBuffer(state->device.Get(), scene->triangle_vertex_buffer.GetAddressOf());
  if (!vb_ok) {
    return false;
  }

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  state->device_context->IASetVertexBuffers(0, 1, scene->triangle_vertex_buffer.GetAddressOf(), &stride, &offset);

  bool il_ok = CreateInputLayout(state->device.Get(), scene->vs_buffer.Get(), scene->vertex_layout.GetAddressOf());
  if (!il_ok) {
    return false;
  }

  state->device_context->IASetInputLayout(scene->vertex_layout.Get());
  state->device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D11_VIEWPORT viewport;
  CreateViewport(window, &viewport);
  state->device_context->RSSetViewports(1, &viewport);

  return true;
}

int main(int /* argc */, char** /* argv */) {
  DxfwGuard dxfw_guard;
  if (!dxfw_guard.IsInitialized()) {
    return -1;
  }

  TCHAR base_path[MAX_PATH];
  GetBasePath(base_path);

  dxfwSetErrorCallback(ErrorCallback);

  std::unique_ptr<dxfwWindow, DxfwWindowDeleter> window(dxfwCreateWindow(800, 600, "Hello DirectX"));
  if (!window) {
    return -1;
  }

  DirectXState state;
  bool direct3d11_ok = InitializeDirect3d11(window.get(), &state);
  if (!direct3d11_ok) {
    return -1;
  }

  Scene scene;
  bool scene_ok = InitializeScene(base_path, window.get(), &state, &scene);
  if (!scene_ok) {
    return -1;
  }

  while (!dxfwShouldWindowClose(window.get())) {
    // Clear
    float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
    state.device_context->ClearRenderTargetView(state.render_target_view.Get(), bgColor);

    // Render
    state.device_context->Draw(3, 0);

    // Swap buffers
    state.swap_chain->Present(0, 0);

    dxfwPollOsEvents();
  }

  state.device_context->ClearState();

  return 0;
}
