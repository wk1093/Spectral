#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <cmath>
#include <vector>

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
    ID3D11DepthStencilView* depthStencilView;
    ID3D11RasterizerState* rasterizerState;
    ID3D11DepthStencilState* depthStencilState;
    sWindow* win;
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
        d3d11SwapChainDesc.SampleDesc.Count = 4;
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
    ID3D11DepthStencilView* d3d11DepthStencilView;
    {
        ID3D11Texture2D* d3d11FrameBuffer;
        HRESULT hResult = d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
        if (FAILED(hResult)) {
            MessageBoxA(0, "GetBuffer() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

        hResult = d3d11Device->CreateRenderTargetView(d3d11FrameBuffer, 0, &d3d11FrameBufferView);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateRenderTargetView() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        D3D11_TEXTURE2D_DESC depthStencilDesc = {};
        d3d11FrameBuffer->GetDesc(&depthStencilDesc);

        d3d11FrameBuffer->Release();

        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.SampleDesc.Count = 4;

        ID3D11Texture2D* depthBuffer;

        hResult = d3d11Device->CreateTexture2D(&depthStencilDesc, 0, &depthBuffer);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateTexture2D() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
        depthStencilViewDesc.Format = depthStencilDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

        hResult = d3d11Device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &d3d11DepthStencilView);
        if (FAILED(hResult)) {
            MessageBoxA(0, "CreateDepthStencilView() failed", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
        }

        depthBuffer->Release();

    }


    ID3D11RasterizerState* rasterizerState;
    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.FrontCounterClockwise = TRUE;
        rasterizerDesc.MultisampleEnable = TRUE;

        d3d11Device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    }

    ID3D11DepthStencilState* depthStencilState;
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable    = TRUE;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;

        d3d11Device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
    }

    __d3d11_1_context.hwnd = hwnd;
    __d3d11_1_context.device = d3d11Device;
    __d3d11_1_context.deviceContext = d3d11DeviceContext;
    __d3d11_1_context.swapChain = d3d11SwapChain;
    __d3d11_1_context.frameBufferView = d3d11FrameBufferView;
    __d3d11_1_context.depthStencilView = d3d11DepthStencilView;
    __d3d11_1_context.rasterizerState = rasterizerState;
    __d3d11_1_context.depthStencilState = depthStencilState;
    __d3d11_1_context.win = win;
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
    __d3d11_1_context.deviceContext->ClearDepthStencilView(__d3d11_1_context.depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    RECT rect;
    GetClientRect(__d3d11_1_context.hwnd, &rect);
    D3D11_VIEWPORT viewport = {0.0f, 0.0f, (float)(rect.right - rect.left), (float)(rect.bottom - rect.top), 0.0f, 1.0f};
    __d3d11_1_context.deviceContext->RSSetViewports(1, &viewport);
    __d3d11_1_context.deviceContext->RSSetState(__d3d11_1_context.rasterizerState);
    __d3d11_1_context.deviceContext->OMSetDepthStencilState(__d3d11_1_context.depthStencilState, 0);
    __d3d11_1_context.deviceContext->OMSetRenderTargets(1, &__d3d11_1_context.frameBufferView, __d3d11_1_context.depthStencilView);
}

CEXPORT void present() {
    bool vsync = __d3d11_1_context.win->vsync;
    __d3d11_1_context.swapChain->Present(vsync, 0);
}

struct sInternalMesh {
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    UINT stride;
    UINT offset;
    size_t numIndices;
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
    size_t textureCount;
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
    return {new sInternalMesh{vertexBuffer, indexBuffer, sizeofVertex * (unsigned int)(sizeof(float)), 0, indexSize / sizeof(sIndex)}};
}

CEXPORT void drawMesh(sMesh mesh) {
    auto* internal = (sInternalMesh*)mesh.internal;
    __d3d11_1_context.deviceContext->IASetVertexBuffers(0, 1, &internal->vertexBuffer, &internal->stride, &internal->offset);
    __d3d11_1_context.deviceContext->IASetIndexBuffer(internal->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    __d3d11_1_context.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    __d3d11_1_context.deviceContext->DrawIndexed(internal->numIndices, 0, 0);
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

    printf("Invalid shader type\n");
    return {nullptr};
}

CEXPORT void useShaderProgram(sShaderProgram shader) {
    auto* internal = (sInternalShaderProgram*)shader.internal;
    __d3d11_1_context.deviceContext->IASetInputLayout(internal->inputLayout);
    __d3d11_1_context.deviceContext->VSSetShader(internal->vertexShader.vertexShader, 0, 0);
    __d3d11_1_context.deviceContext->PSSetShader(internal->fragmentShader.pixelShader, 0, 0);
    internal->textureCount = 0;
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
            default:
                printf("Invalid shader type\n");
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
        if (vertexShader.vertDef == nullptr) {
            printf("Invald Vertex Data!");
        }
        if (vertexShader.vertDef->count > 15) {
            printf("Too many vertex elements!");
        }
        unsigned int offset2 = 0;
        std::vector<std::string*> strs;
        for (int i = 0; i < vertexShader.vertDef->count; i++) {
            std::string* str = new std::string("C_"+std::to_string(i)+"e");
            strs.push_back(str);
            inputElementDesc[i].SemanticName = str->c_str();
            inputElementDesc[i].SemanticIndex = 0;
            switch (vertexShader.vertDef->elements[i]) {
                case 1:
                    inputElementDesc[i].Format = DXGI_FORMAT_R32_FLOAT;
                    break;
                case 2:
                    inputElementDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT;
                    break;
                case 3:
                    inputElementDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
                    break;
                case 4:
                    inputElementDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    break;
            }
            inputElementDesc[i].InputSlot = 0;
            inputElementDesc[i].AlignedByteOffset = offset2;
            inputElementDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            inputElementDesc[i].InstanceDataStepRate = 0;
            offset2 += vertexShader.vertDef->elements[i] * sizeof(float);
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
    sInternalUniforms* internal = (sInternalUniforms*)malloc(sizeof(sInternalUniforms));
    if (!internal) {
        MessageBoxA(0, "malloc() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }
    internal->program = program;
    internal->fragmentPart = getPartialf(def, sShaderType::FRAGMENT);
    internal->vertexPart = getPartialf(def, sShaderType::VERTEX);

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = internal->fragmentPart.size();
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subResourceData{};
    uint8_t* fakeBuffer = new uint8_t[internal->fragmentPart.size()];
    subResourceData.pSysMem = fakeBuffer;
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;
    HRESULT hResult = __d3d11_1_context.device->CreateBuffer(&bufferDesc, &subResourceData, &internal->fragmentBuffer);
    if (FAILED(hResult)) {
        MessageBoxA(0, "CreateBuffer() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }

    bufferDesc.ByteWidth = internal->vertexPart.size();
    uint8_t* fakeBuffer2 = new uint8_t[internal->vertexPart.size()];
    subResourceData.pSysMem = fakeBuffer2;
    hResult = __d3d11_1_context.device->CreateBuffer(&bufferDesc, &subResourceData, &internal->vertexBuffer);
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

struct sInternalTexture {
    ID3D11ShaderResourceView* texture;
    ID3D11SamplerState* sampler;
};

CEXPORT sTexture createTexture(sTextureDefinition def) {
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11SamplerState* sampler;
    HRESULT hResult = __d3d11_1_context.device->CreateSamplerState(&samplerDesc, &sampler);
    if (FAILED(hResult)) {
        MessageBoxA(0, "CreateSamplerState() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = def.width;
    textureDesc.Height = def.height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    switch (def.channels) {
        case 1:
            textureDesc.Format = DXGI_FORMAT_R8_UNORM;
            break;
        case 3:
            textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case 4:
            textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        default:
            MessageBoxA(0, "Invalid texture format", "Fatal Error", MB_OK);
            printf("ERROR CODE: %lu\n", GetLastError());
            return {nullptr};
    }
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pSysMem = def.data;
    subResourceData.SysMemPitch = def.width * def.channels;
    
    ID3D11Texture2D* texture;
    hResult = __d3d11_1_context.device->CreateTexture2D(&textureDesc, &subResourceData, &texture);
    if (FAILED(hResult)) {
        MessageBoxA(0, "CreateTexture2D() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }

    ID3D11ShaderResourceView* textureView;
    hResult = __d3d11_1_context.device->CreateShaderResourceView(texture, 0, &textureView);
    if (FAILED(hResult)) {
        MessageBoxA(0, "CreateShaderResourceView() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }

    sInternalTexture* internal = (sInternalTexture*)malloc(sizeof(sInternalTexture));
    if (!internal) {
        MessageBoxA(0, "malloc() failed", "Fatal Error", MB_OK);
        printf("ERROR CODE: %lu\n", GetLastError());
    }
    internal->texture = textureView;
    internal->sampler = sampler;
    
    return {internal};
}

CEXPORT void useTexture(sShaderProgram program, sTexture texture, const char* name) {
    auto* internal = (sInternalTexture*)texture.internal;
    auto* internalProgram = (sInternalShaderProgram*)program.internal;
    __d3d11_1_context.deviceContext->PSSetShaderResources(internalProgram->textureCount, 1, &internal->texture);
    __d3d11_1_context.deviceContext->PSSetSamplers(internalProgram->textureCount, 1, &internal->sampler);
    internalProgram->textureCount++;
}

CEXPORT void freeTexture(sTexture texture) {
    auto* internal = (sInternalTexture*)texture.internal;
    internal->texture->Release();
    internal->sampler->Release();
    free(internal);
}

CEXPORT void freeShader(sShader shader) {
    auto* internal = (sInternalShader*)shader.internal;
    if (internal->vertexShader) {
        internal->vertexShader->Release();
    }
    if (internal->pixelShader) {
        internal->pixelShader->Release();
    }
    if (internal->geometryShader) {
        internal->geometryShader->Release();
    }
    free(internal);
}

CEXPORT void freeShaderProgram(sShaderProgram program) {
    auto* internal = (sInternalShaderProgram*)program.internal;
    internal->inputLayout->Release();
    free(internal);
}

CEXPORT void freeMesh(sMesh mesh) {
    auto* internal = (sInternalMesh*)mesh.internal;
    internal->vertexBuffer->Release();
    internal->indexBuffer->Release();
    free(internal);
}

CEXPORT void freeUniforms(sUniforms uniforms) {
    auto* internal = (sInternalUniforms*)uniforms.internal;
    internal->fragmentBuffer->Release();
    internal->vertexBuffer->Release();
    free(internal);
}

CEXPORT void destroy() {
    __d3d11_1_context.frameBufferView->Release();
    __d3d11_1_context.depthStencilView->Release();
    __d3d11_1_context.rasterizerState->Release();
    __d3d11_1_context.depthStencilState->Release();
    __d3d11_1_context.swapChain->Release();
    __d3d11_1_context.deviceContext->Release();
    __d3d11_1_context.device->Release();
}