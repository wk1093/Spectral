#include <stdio.h>
#include <string>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#include <commctrl.h>
#endif
/**
 * @brief Selects the window and graphics modules from a list of available modules.
 * 
 * This function will do something different depending on the platform, but mostly it will display a dialog box with a list of available modules and allow the user to select one.
 * 
 * @param dialog The dialog text to display.
 * @param gfx_mods A list of available graphics modules.
 * @param sel_impl The selected graphics module.
 */
void sSelectModules(const char* dialog, const std::vector<sModuleDef>& gfx_mods, char** sel_impl);
#ifdef _WIN32

int __selected_graphics_module = 0;
HWND __ok_button = NULL;
HWND __cancel_button = NULL;
HWND __comboBox = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            printf("Window destroyed\n");
            PostQuitMessage(0);
            break;
        case WM_COMMAND:
            // if the user clicked the OK button
            if (HIWORD(wParam) == BN_CLICKED) {
                if ((HWND)lParam == __ok_button) {
                    __selected_graphics_module = SendMessage(__comboBox, CB_GETCURSEL, 0, 0);
                    printf("Selected graphics module: %d\n", __selected_graphics_module);
                    PostQuitMessage(0);
                } else if ((HWND)lParam == __cancel_button) {
                    __selected_graphics_module = -1;
                    printf("User cancelled\n");
                    PostQuitMessage(0);
                }
            }
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void sSelectModules(const char* dialog, const std::vector<sModuleDef>& gfx_mods, char** sel_impl) {
    printf("sSelectModules() called\n");
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    // ensure getmodulehandle didn't fail
    if (!wc.hInstance) {
        DWORD err = GetLastError();
        printf("GetModuleHandle failed: %lu\n", err);
        return;
    }
    wc.lpszClassName = "SpectralEngineDialog";
    ATOM atom = RegisterClass(&wc);
    if (!atom) {
        DWORD err = GetLastError();
        printf("RegisterClass failed: %lu\n", err);
    }
    HWND hwnd = CreateWindowEx(0, "SpectralEngineDialog", "SpectralEngine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, wc.hInstance, NULL);
    if (!hwnd) {
        printf("CreateWindowEx() failed\n");
        MessageBoxA(0, "CreateWindowEx() failed", "Fatal Error", MB_OK);
        return;
    }

    HWND comboBox = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 10, 30, 160, 200, hwnd, NULL, wc.hInstance, NULL);

    HWND button = CreateWindow("BUTTON", "OK", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 80, 80, 25, hwnd, NULL, wc.hInstance, NULL);
    HWND cancelButton = CreateWindow("BUTTON", "Cancel", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 100, 80, 80, 25, hwnd, NULL, wc.hInstance, NULL);

    __ok_button = button;
    __cancel_button = cancelButton;
    __comboBox = comboBox;
    __selected_graphics_module = 0;

    HWND label = CreateWindow("STATIC", dialog, WS_VISIBLE | WS_CHILD, 10, 10, 160, 20, hwnd, NULL, wc.hInstance, NULL);
    // Add items to the combo boxes
    

    for (const auto& mod : gfx_mods) {
        SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)mod.dispname.c_str());
        printf("Added %s to combo box\n", mod.dispname.c_str());
    }
    SendMessage(comboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    DestroyWindow(comboBox);
    DestroyWindow(button);
    DestroyWindow(cancelButton);
    DestroyWindow(label);
    DestroyWindow(hwnd);
    UnregisterClass("SpectralEngineDialog", wc.hInstance);

    __ok_button = NULL;
    __cancel_button = NULL;
    __comboBox = NULL;

    if (__selected_graphics_module == -1) {
        // exit if the user clicked cancel
        printf("User cancelled\n");
        *sel_impl = NULL;
        return;
    }
    *sel_impl = strdup(gfx_mods[__selected_graphics_module].impl.c_str());
    return;
}
#else
void sSelectModules(const char* dialog, const std::vector<sModuleDef>& gfx_mods, char** sel_impl) {
    // a basic terminal interface

    printf("%s\n", dialog);

    for (size_t i = 0; i < gfx_mods.size(); i++) {
        printf("%zu: %s\n", i, gfx_mods[i].dispname.c_str());
    }

    printf("Select a graphics module: ");
    size_t selected = 0;
    scanf("%zu", &selected);
    if (selected >= gfx_mods.size()) {
        printf("Invalid selection\n");
        *sel_impl = NULL;
        return;
    }
    *sel_impl = strdup(gfx_mods[selected].impl.c_str());
    printf("Selected graphics module: %s\n", *sel_impl);
    return;
}
#endif