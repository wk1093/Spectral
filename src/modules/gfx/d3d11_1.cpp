#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <cmath>

#include "module.h"

CEXPORT const char* getShaderType() {
    return "hlsl";
}

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
    RECT rect;
    GetClientRect(__d3d11_1_context.hwnd, &rect);
    D3D11_VIEWPORT viewport = {0.0f, 0.0f, (float)(rect.right - rect.left), (float)(rect.bottom - rect.top), 0.0f, 1.0f};
    __d3d11_1_context.deviceContext->RSSetViewports(1, &viewport);
    __d3d11_1_context.deviceContext->OMSetRenderTargets(1, &__d3d11_1_context.frameBufferView, 0);
}

CEXPORT void present() {
    __d3d11_1_context.swapChain->Present(1, 0);
}

struct sInternalMesh {
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    UINT stride;
    UINT offset;
};

struct sInternalShader {
    enum sShaderType type;
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11GeometryShader* geometryShader;
    ID3DBlob* shaderBlob;
    sVertexDefinition* vertDef;
};

struct sInternalShaderProgram {
    sInternalShader vertexShader;
    sInternalShader fragmentShader;
    // sInternalShader geometryShader;
    ID3D11InputLayout* inputLayout;
};

CEXPORT sMesh createMesh(sShader vertexShader, void* vertices, size_t vertexSize, sIndex* indices, size_t indexSize) {
    ID3D11Buffer* vertexBuffer;
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = vertexSize;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = vertices;

        HRESULT hResult = __d3d11_1_context.device->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateBuffer() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }
    }

    ID3D11Buffer* indexBuffer;
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = indexSize;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = indices;

        HRESULT hResult = __d3d11_1_context.device->CreateBuffer(&bufferDesc, &subResourceData, &indexBuffer);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateBuffer() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }
    }
    sVertexDefinition* vd = ((sInternalShader*)vertexShader.internal)->vertDef;
    unsigned int sizeofVertex = 0;
    for (int i = 0; i < vd->count; i++) {
        sizeofVertex += vd->elements[i];
    }
    return {new sInternalMesh{vertexBuffer, indexBuffer, sizeofVertex * (unsigned int)(sizeof(float)), 0}};
}

CEXPORT void drawMesh(sMesh mesh) {
    auto* internal = (sInternalMesh*)mesh.internal;
    __d3d11_1_context.deviceContext->IASetVertexBuffers(0, 1, &internal->vertexBuffer, &internal->stride, &internal->offset);
    __d3d11_1_context.deviceContext->IASetIndexBuffer(internal->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    __d3d11_1_context.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    __d3d11_1_context.deviceContext->DrawIndexed(3, 0, 0);
}

CEXPORT sShader createShader(const char* source, sShaderType type, sVertexDefinition* vertDef) {
    ID3DBlob* shaderBlob;
    ID3DBlob* errorBlob;
    switch(type) {
        case sShaderType::VERTEX: {
            if (!vertDef) {
                MessageBoxA(0, "Vertex shader requires vertex definition", "Fatal Error", MB_OK);
                return {nullptr};
            }
            ID3D11VertexShader *vertexShader;
            HRESULT hResult = D3DCompile(source, strlen(source), "unknown", 0, 0, "main", "vs_5_0", 0, 0, &shaderBlob,
                                         &errorBlob);
            if (FAILED(hResult)) {
                MessageBoxA(0, (char *) errorBlob->GetBufferPointer(), "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
                errorBlob->Release();
            }
            hResult = __d3d11_1_context.device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
            if (FAILED(hResult)) {
                MessageBoxA(0, "CreateVertexShader() failed", "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
            }

            return {new sInternalShader{sShaderType::VERTEX, vertexShader, nullptr, nullptr, shaderBlob, vertDef}};
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
            hResult = __d3d11_1_context.device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
            if (FAILED(hResult)) {
                MessageBoxA(0, "CreatePixelShader() failed", "Fatal Error", MB_OK);
                printf("ERROR CODE: %lu\n", GetLastError());
            }
            return {new sInternalShader{sShaderType::FRAGMENT, nullptr, pixelShader, nullptr, shaderBlob, nullptr}};
        } break;
        case sShaderType::GEOMETRY: {
            printf("GEOMETRY SHADER UNIMPLEMENTED\n");
            return {nullptr};
        } break;
    }

    return {nullptr};
}

CEXPORT void useShaderProgram(sShaderProgram shader) {
    auto* internal = (sInternalShaderProgram*)shader.internal;
    __d3d11_1_context.deviceContext->IASetInputLayout(internal->inputLayout);
    __d3d11_1_context.deviceContext->VSSetShader(internal->vertexShader.vertexShader, 0, 0);
    __d3d11_1_context.deviceContext->PSSetShader(internal->fragmentShader.pixelShader, 0, 0);
}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
    sInternalShader vertexShader{};
    sInternalShader fragmentShader{};

    for (size_t i = 0; i < count; i++) {
        auto* shader = (sInternalShader*)shaders[i].internal;
        switch(shader->type) {
            case sShaderType::VERTEX:
                vertexShader = *shader;
                break;
            case sShaderType::FRAGMENT:
                fragmentShader = *shader;
                break;
            case sShaderType::GEOMETRY:
                printf("GEOMETRY SHADER UNIMPLEMENTED\n");
                return {nullptr};
        }
    }
    if (!vertexShader.shaderBlob) {
        MessageBoxA(0, "Vertex shader not found", "Fatal Error", MB_OK);
        return {nullptr};
    }
    if (!fragmentShader.shaderBlob) {
        MessageBoxA(0, "Fragment shader not found", "Fatal Error", MB_OK);
        return {nullptr};
    }
    ID3D11InputLayout* inputLayout;
    {
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[15] = {{0}};
        int offset = 0;
        if (vertexShader.vertDef == nullptr) {
            printf("Invald Vertex Data!");
        }
        if (vertexShader.vertDef->count > 15) {
            printf("Too many vertex elements!");
        }
        for (int i = 0; i < vertexShader.vertDef->count; i++) {
            if (i == 0)
                inputElementDesc[i] = {("C_"+std::to_string(i)+"e").c_str(), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
            else
                inputElementDesc[i] = {("C_"+std::to_string(i)+"e").c_str(), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
        }


        HRESULT hResult = __d3d11_1_context.device->CreateInputLayout(inputElementDesc, vertexShader.vertDef->count,
                                                                     vertexShader.shaderBlob->GetBufferPointer(),
                                                                     vertexShader.shaderBlob->GetBufferSize(), &inputLayout);

        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateInputLayout() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }
        vertexShader.shaderBlob->Release();
    }
    fragmentShader.shaderBlob->Release();

    return {new sInternalShaderProgram{vertexShader, fragmentShader, inputLayout}};
}

struct sInternalUniforms {
    sUniformDefinition fragmentPart;
    sUniformDefinition vertexPart;
    sShaderProgram program;
    ID3D11Buffer* fragmentBuffer;
    ID3D11Buffer* vertexBuffer;
};

CEXPORT sUniforms createUniforms(sShaderProgram program, sUniformDefinition def) {
    printf("Creating uniforms\n");
    sInternalUniforms* internal = (sInternalUniforms*)malloc(sizeof(sInternalUniforms));
    if (!internal) {
        MessageBoxA(0, "malloc() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }
    internal->program = program;
    printf("asdsad\n");
    internal->fragmentPart = getPartialf(def, sShaderType::FRAGMENT);
    internal->vertexPart = getPartialf(def, sShaderType::VERTEX);
    printf("bufsedsdec\n");

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = internal->fragmentPart.size();
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subResourceData{};
    uint8_t fakeBuffer[internal->fragmentPart.size()] = {0};
    subResourceData.pSysMem = fakeBuffer;
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;
    printf("Creating fragment buffer\n");
    HRESULT hResult = __d3d11_1_context.device->CreateBuffer(&bufferDesc, &subResourceData, &internal->fragmentBuffer);
    printf("Created fragment buffer\n");
    if (FAILED(hResult)) {
        MessageBoxA(0, "CreateBuffer() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }

    bufferDesc.ByteWidth = internal->vertexPart.size();
    uint8_t fakeBuffer2[internal->vertexPart.size()] = {0};
    subResourceData.pSysMem = fakeBuffer2;
    printf("Creating vertex buffer\n");
    hResult = __d3d11_1_context.device->CreateBuffer(&bufferDesc, &subResourceData, &internal->vertexBuffer);
    printf("Created vertex buffer\n");
    if (FAILED(hResult)) {
        MessageBoxA(0, "CreateBuffer() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }

    return {internal};
}

CEXPORT void setUniforms(sUniforms uniforms, void* data) {
    auto* internal = (sInternalUniforms*)uniforms.internal;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    __d3d11_1_context.deviceContext->Map(internal->fragmentBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, data, internal->fragmentPart.size());
    __d3d11_1_context.deviceContext->Unmap(internal->fragmentBuffer, 0);

    __d3d11_1_context.deviceContext->PSSetConstantBuffers(0, 1, &internal->fragmentBuffer);

    __d3d11_1_context.deviceContext->Map(internal->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, (char*)data + internal->fragmentPart.size(), internal->vertexPart.size());
    __d3d11_1_context.deviceContext->Unmap(internal->vertexBuffer, 0);

    __d3d11_1_context.deviceContext->VSSetConstantBuffers(0, 1, &internal->vertexBuffer);
}