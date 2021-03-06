// LitesNes.Application.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <iostream>

#include <windows.h>
#include <functional>
#include <DirectXColors.h>

#include <wrl.h>
#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#include <DirectXMath.h>
#include <gsl\gsl>

#include "DirectXTK\WICTextureLoader.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <tchar.h>

#include "NesDebugger.h"
#include "PPU.h"
#include "RomReader.h"
#include "CpuInstruction.h"

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

HRESULT CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return E_FAIL;

    CreateRenderTarget();

    return S_OK;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            ImGui_ImplDX11_InvalidateDeviceObjects();
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
            ImGui_ImplDX11_CreateDeviceObjects();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int main(int, char**)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(_T("ImGui Example"), _T("ImGui DirectX11 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (CreateDeviceD3D(hwnd) < 0)
    {
        CleanupDeviceD3D();
        UnregisterClass(_T("ImGui Example"), wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplDX11_Init(hwnd, g_pd3dDevice, g_pd3dDeviceContext);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = true;
    //bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		char *texArray = new char[256 * 240 * 4];
		//ID3D11Texture2D *boxTex = 0;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> boxTex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = 256;
		desc.Height = 240;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA boxTexInitData;
		ZeroMemory(&boxTexInitData, sizeof(D3D11_SUBRESOURCE_DATA));
		boxTexInitData.pSysMem = texArray;
		boxTexInitData.SysMemPitch = 256 * 4;

		for (uint32_t i = 0; i < 256 * 240 * 4; i += 4)
		{
			texArray[i] = uint8_t(i);
			texArray[i+1] = uint8_t(i);
			texArray[i+2] = uint8_t(i/64);
			texArray[i+3] = uint8_t(255);
		}
		HRESULT res = g_pd3dDevice->CreateTexture2D(&desc, &boxTexInitData, boxTex.ReleaseAndGetAddressOf());
		if (FAILED(res)) {
			throw std::exception("NoTextureForYou");
		}
		//ID3D11ShaderResourceView* m_texture;
		res = g_pd3dDevice->CreateShaderResourceView(boxTex.Get(), NULL, &m_texture);
		if (FAILED(res))
		{
			throw std::exception("NoShaderResourceForYou");
		}

		/*
		D3D11_TEXTURE2D_DESC boxTexDesc;
		ZeroMemory(&boxTexDesc, sizeof(D3D11_TEXTURE2D_DESC));
		boxTexDesc.ArraySize = 1;
		boxTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		boxTexDesc.CPUAccessFlags = 0;
		boxTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		boxTexDesc.Height = 20;
		boxTexDesc.MipLevels = 1;
		boxTexDesc.MiscFlags = 0;
		boxTexDesc.SampleDesc.Count = 4;
		boxTexDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
		boxTexDesc.Usage = D3D11_USAGE_DEFAULT;
		boxTexDesc.Width = 20;


		if (FAILED(DirectX::CreateWICTextureFromFile(g_pd3dDevice, L"cat.png", nullptr,
			&m_texture))) {
			throw std::exception();
		}
		ImTextureID marioTex = m_texture;*/

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
	NesDebugger debug;
	RomReader donkeyKongRom("C:\\Users\\jnigh\\Source\\Repos\\LitesNes\\content\\donkeykong.nes");
	donkeyKongRom.ReadFile();

	uint32_t chrRomSize = donkeyKongRom.GetChrRomDataSize();
	char* chrRomPtr = donkeyKongRom.GetChrRomData();
	PPU NesPPU(chrRomSize, chrRomPtr);

	//Creating the CPU Instructions for ram. Later this should be encapsulated
	uint16_t indexStart = 0x8000;
	uint16_t index = indexStart;
	uint16_t instructionCount = 0;
	uint8_t* romToLoadIn = reinterpret_cast<uint8_t*>(donkeyKongRom.GetPrgRomData());
	for (uint16_t i = 0; i < donkeyKongRom.GetPrgRomDataSize(); ++i)
	{
		uint16_t curIndex = 0x8000 + i;
		NesDebugger::mRam.SetMemoryByLocation(curIndex, romToLoadIn[i]);
	}
	if (donkeyKongRom.GetPrgRomDataSize() <= 16400)
	{
		for (uint16_t i = 0; i < donkeyKongRom.GetPrgRomDataSize(); ++i)
		{
			uint16_t curIndex = 0xC000 + i;
			NesDebugger::mRam.SetMemoryByLocation(curIndex, romToLoadIn[i]);
		}
	}
	//NesDebugger::mRam. //Gotta place this data in the RAM. Technically duplicating data, but accurate
	//while (index < donkeyKongRom.GetPrgRomDataSize()) {
	while (index < 0xC000) {
		uint16_t curIndex = index - indexStart;
		uint8_t opcode = romToLoadIn[curIndex];
		auto descriptionSearchAttempt = CpuInstruction::sOperations.find(opcode);
		if (descriptionSearchAttempt == CpuInstruction::sOperations.end())
		{
			opcode = 0;
			descriptionSearchAttempt = CpuInstruction::sOperations.find(opcode);
		}
		const CpuInstruction::OperationDescription& curOperationDescription = descriptionSearchAttempt->second;
		CpuInstruction curInstruction(opcode, curOperationDescription, (romToLoadIn + curIndex), index + 0x4000);

		debug.mInstructionList.push_back(curInstruction);
		NesDebugger::sOperations.emplace(index, instructionCount);
		if (donkeyKongRom.GetPrgRomDataSize() <= 16400) //If we're duplicating the data, duplicate the entries too
		{
			uint16_t mirroredIndex = index + 0x4000;
			NesDebugger::sOperations.emplace(mirroredIndex, instructionCount);
		}

		index += curOperationDescription.mBytes;
		instructionCount++;
	}
	uint32_t count = 230;
	debug.Init();
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
		//Updating our NesDebug class
		debug.Update();

        ImGui_ImplDX11_NewFrame();

		{
			ImGui::Begin("Texture", 0, 0);
			for (uint32_t i = 0; i < 256 * 240 * 4; i += 4)
			{
				texArray[i] = uint8_t(255);
				texArray[i+1] = uint8_t(255);
				texArray[i+2] = uint8_t(255);
				texArray[i+3] = uint8_t(255);
			}
			//debug.DrawFromBuffer(reinterpret_cast<uint32_t*>(texArray));
			//NesPPU.DrawFromBuffer(reinterpret_cast<uint32_t*>(texArray));
			NesPPU.Render(reinterpret_cast<uint32_t*>(texArray));
			//NesPPU.DrawFromBuffer(reinterpret_cast<uint32_t*>(texArray));
			{
				//update sprite
				/*
				OAM::OAMEntry& oam = PPU::sOAM.GetEntry(1);
				if (oam.yPos > 100)
				{
					right = false;
					++demoSprite;
				}
				else if (oam.yPos < 25)
				{
					right = true;
					++demoSprite;
				}
				if (right)
				{
					++oam.yPos;
					++oam.xPos;
				} else {
					--oam.yPos;
					--oam.xPos;
				}
				oam.tileIndex = demoSprite % 255;
				oam.attributes = 12;*/
			}
			count++;
			res = g_pd3dDevice->CreateTexture2D(&desc, &boxTexInitData, boxTex.ReleaseAndGetAddressOf());
			if (FAILED(res)) {
				throw std::exception("NoTextureForYou");
			}
			res = g_pd3dDevice->CreateShaderResourceView(boxTex.Get(), NULL, &m_texture);
			if (FAILED(res))
			{
				throw std::exception("NoShaderResourceForYou");
			}

			ImTextureID marioTex = m_texture.Get();

			ImGui::Image(marioTex, ImVec2(256,240));
			char text[5];
			uint16_t ppuAddr = PPU::PPUADDR.GetAddress();
			NesDebugger::PopulateCharBufferWithHex(text, ppuAddr);
			ImGui::Text("PPUAddr: ");
			ImGui::SameLine();
			ImGui::Text(text);
			ImGui::End();
		}

		{
			debug.RenderStackWindow();
		}

		//Rendering our NES Debug windows
		debug.RenderDebugger();
		debug.RenderMemoryWindow();

		//Uncomment this to show demo window, useful for scouting features
        if (show_demo_window)
        {
			/*
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);*/
        }

		{
			ImGui::Begin("Framerate", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::SetWindowSize(ImVec2(200, 30), ImGuiSetCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2(2, 2), ImGuiSetCond_FirstUseEver);
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
			//ImGui::ShowTestWindow();
		}

        // Rendering
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClass(_T("ImGui Example"), wc.hInstance);

    return 0;
}
