#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <cmath>

#include "module.h"

struct sD3D11_1Context {
    HWND hwnd;
    ID3D11Device1* device;
    ID3D11DeviceContext1* deviceContext;
    IDXGISwapChain1* swapChain;
    ID3D11RenderTargetView* frameBufferView;
};

sD3D11_1Context __d3d11_1_context;

CEXPORT void init(sWindow* win) {
    HWND hwnd = (HWND)win->creator->getHandle(*win);

    ID3D11Device1* d3d11Device;
    ID3D11DeviceContext1* d3d11DeviceContext;
    {
        ID3D11Device* baseDevice;
        ID3D11DeviceContext* baseDeviceContext;
        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG_BUILD)
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
                                            0, creationFlags,
                                            featureLevels, ARRAYSIZE(featureLevels),
                                            D3D11_SDK_VERSION, &baseDevice,
                                            0, &baseDeviceContext);
        if(FAILED(hResult)){
            MessageBoxA(0, "D3D11CreateDevice() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        // Get 1.1 interface of D3D11 Device and Context
        hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&d3d11Device);
        if (FAILED(hResult)) {
            MessageBoxA(0, "QueryInterface() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }
        baseDevice->Release();

        hResult = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&d3d11DeviceContext);
        if (FAILED(hResult)) {
            MessageBoxA(0, "QueryInterface() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }
        baseDeviceContext->Release();
    }

#ifdef DEBUG_BUILD
    // Set up debug layer to break on D3D11 errors
    ID3D11Debug *d3dDebug = nullptr;
    d3d11Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug);
    if (d3dDebug)
    {
        ID3D11InfoQueue *d3dInfoQueue = nullptr;
        if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
        {
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            d3dInfoQueue->Release();
        }
        d3dDebug->Release();
    }
#endif

    // Create Swap Chain
    IDXGISwapChain1* d3d11SwapChain;
    {
        // Get DXGI Factory (needed to create Swap Chain)
        IDXGIFactory2* dxgiFactory;
        {
            IDXGIDevice1* dxgiDevice;
            HRESULT hResult = d3d11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
            if (FAILED(hResult)) {
                MessageBoxA(0, "QueryInterface() failed", "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
            }

            IDXGIAdapter* dxgiAdapter;
            hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
            if (FAILED(hResult)) {
                MessageBoxA(0, "GetAdapter() failed", "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
            }
            dxgiDevice->Release();

            DXGI_ADAPTER_DESC adapterDesc;
            dxgiAdapter->GetDesc(&adapterDesc);

            OutputDebugStringA("Graphics Device: ");
            OutputDebugStringW(adapterDesc.Description);

            hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
            if (FAILED(hResult)) {
                MessageBoxA(0, "GetParent() failed", "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
            }
            dxgiAdapter->Release();
        }

        DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
        d3d11SwapChainDesc.Width = 0; // use window width
        d3d11SwapChainDesc.Height = 0; // use window height
        d3d11SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        d3d11SwapChainDesc.SampleDesc.Count = 1;
        d3d11SwapChainDesc.SampleDesc.Quality = 0;
        d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        d3d11SwapChainDesc.BufferCount = 2;
        d3d11SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        d3d11SwapChainDesc.Flags = 0;

        HRESULT hResult = dxgiFactory->CreateSwapChainForHwnd(d3d11Device, hwnd, &d3d11SwapChainDesc, 0, 0, &d3d11SwapChain);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateSwapChainForHwnd() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        dxgiFactory->Release();
    }

    // Create Framebuffer Render Target
    ID3D11RenderTargetView* d3d11FrameBufferView;
    {
        ID3D11Texture2D* d3d11FrameBuffer;
        HRESULT hResult = d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
        if (FAILED(hResult)) {
            MessageBoxA(0, "GetBuffer() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        hResult = d3d11Device->CreateRenderTargetView(d3d11FrameBuffer, 0, &d3d11FrameBufferView);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateRenderTargetView() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }


        d3d11FrameBuffer->Release();
    }
    __d3d11_1_context.hwnd = hwnd;
    __d3d11_1_context.device = d3d11Device;
    __d3d11_1_context.deviceContext = d3d11DeviceContext;
    __d3d11_1_context.swapChain = d3d11SwapChain;
    __d3d11_1_context.frameBufferView = d3d11FrameBufferView;
}

float __clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

CEXPORT void setClearColor(float r, float g, float b, float a) {
    __clearColor[0] = r;
    __clearColor[1] = g;
    __clearColor[2] = b;
    __clearColor[3] = a;
}

CEXPORT void clear() {
    __d3d11_1_context.deviceContext->ClearRenderTargetView(__d3d11_1_context.frameBufferView, __clearColor);
}

CEXPORT void present() {
    __d3d11_1_context.swapChain->Present(1, 0);
}

struct sInternalMesh {
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    UINT numVerts;
    UINT stride;
    UINT offset;
};

struct sInternalShader {
    enum sShaderType type;
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11GeometryShader* geometryShader;
    ID3DBlob* shaderBlob;
};

struct sInternalShaderProgram {
    // TODO: Implement
};

CEXPORT sMesh createMesh(sShader vertexShader, sVertex* vertices, size_t vertexSize, sIndex* indices, size_t indexSize) {

}

CEXPORT void drawMesh(sMesh mesh) {

}

CEXPORT sShader createShader(const char* source, sShaderType type) {
    ID3DBlob* shaderBlob;
    ID3DBlob* errorBlob;
    switch(type) {
        case sShaderType::VERTEX: {
            ID3D11VertexShader *vertexShader;
            HRESULT hResult = D3DCompile(source, strlen(source), "unknown", 0, 0, "main", "vs_5_0", 0, 0, &shaderBlob,
                                         &errorBlob);
            if (FAILED(hResult)) {
                MessageBoxA(0, (char *) errorBlob->GetBufferPointer(), "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
                errorBlob->Release();
            }
            return {new sInternalShader{sShaderType::VERTEX, vertexShader, nullptr, nullptr, shaderBlob}};
        } break;
        case sShaderType::FRAGMENT: {
            ID3D11PixelShader *pixelShader;
            HRESULT hResult = D3DCompile(source, strlen(source), "unknown", 0, 0, "main", "ps_5_0", 0, 0, &shaderBlob,
                                         &errorBlob);
            if (FAILED(hResult)) {
                MessageBoxA(0, (char *) errorBlob->GetBufferPointer(), "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
                errorBlob->Release();
            }
            return {new sInternalShader{sShaderType::FRAGMENT, nullptr, pixelShader, nullptr, shaderBlob}};
        } break;
        case sShaderType::GEOMETRY: {
            printf("GEOMETRY SHADER UNIMPLEMENTED\n");
            return {nullptr};
        } break;
    }
}

CEXPORT sShader loadShader(const char* path, sShaderType type) {
    std::string source;
    if (!readFile(path, source)) {
        printf("Error reading file\n");
        return {nullptr};
    }
    return createShader(source.c_str(), type);
}

CEXPORT void useShaderProgram(sShaderProgram shader) {

}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {

}