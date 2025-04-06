#include <stdio.h>
#include <string>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#include <commctrl.h>
#endif
typedef std::vector<std::string> sModuleList;
#ifdef _WIN32

int __selected_window_module = 0;
int __selected_graphics_module = 0;
HWND __ok_button = NULL;
HWND __cancel_button = NULL;
HWND __comboBox1 = NULL;
HWND __comboBox2 = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
        case WM_COMMAND:
            // if the user clicked the OK button
            if (LOWORD(wParam) == BN_CLICKED) {
                if ((HWND)lParam == __ok_button) {
                    __selected_window_module = SendMessage(__comboBox1, CB_GETCURSEL, 0, 0);
                    __selected_graphics_module = SendMessage(__comboBox2, CB_GETCURSEL, 0, 0);
                    PostQuitMessage(0);
                } else if ((HWND)lParam == __cancel_button) {
                    __selected_window_module = -1;
                    __selected_graphics_module = -1;
                    PostQuitMessage(0);
                }
            }
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
void sSelectModules(sModuleList win_mods, char** window_module, sModuleList gfx_mods, char** graphics_module) {
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SpectralEngineDialog";

    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, "SpectralEngineDialog", "SpectralEngine", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, wc.hInstance, NULL);
    if (!hwnd) {
        printf("CreateWindowEx() failed\n");
        MessageBoxA(0, "CreateWindowEx() failed", "Fatal Error", MB_OK);
        return;
    }

    HWND comboBox1 = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 10, 30, 160, 200, hwnd, NULL, wc.hInstance, NULL);
    HWND comboBox2 = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 210, 30, 160, 200, hwnd, NULL, wc.hInstance, NULL);

    HWND button = CreateWindow("BUTTON", "OK", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 80, 80, 25, hwnd, NULL, wc.hInstance, NULL);
    HWND cancelButton = CreateWindow("BUTTON", "Cancel", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 100, 80, 80, 25, hwnd, NULL, wc.hInstance, NULL);

    __ok_button = button;
    __cancel_button = cancelButton;
    __comboBox1 = comboBox1;
    __comboBox2 = comboBox2;

    HWND label1 = CreateWindow("STATIC", "Select Window Module:", WS_VISIBLE | WS_CHILD, 10, 10, 160, 20, hwnd, NULL, wc.hInstance, NULL);
    HWND label2 = CreateWindow("STATIC", "Select Graphics Module:", WS_VISIBLE | WS_CHILD, 210, 10, 160, 20, hwnd, NULL, wc.hInstance, NULL);
    // Add items to the combo boxes
    
    for (const auto& mod : win_mods) {
        SendMessage(comboBox1, CB_ADDSTRING, 0, (LPARAM)mod.c_str());
    }
    for (const auto& mod : gfx_mods) {
        SendMessage(comboBox2, CB_ADDSTRING, 0, (LPARAM)mod.c_str());
    }
    SendMessage(comboBox1, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
    SendMessage(comboBox2, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    DestroyWindow(hwnd);
    UnregisterClass("SpectralEngineDialog", wc.hInstance);

    if (__selected_window_module == -1 || __selected_graphics_module == -1) {
        // exit if the user clicked cancel
        printf("User cancelled\n");
        *window_module = nullptr;
        *graphics_module = nullptr;
        return;
    }

    *window_module = strdup(win_mods[__selected_window_module].c_str());
    *graphics_module = strdup(gfx_mods[__selected_graphics_module].c_str());
    return;
}
#else
#error Not Implemented!
#endif