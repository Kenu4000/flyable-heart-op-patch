#include <windows.h>
#include <string.h>
#include "exports.inc"

#pragma comment(lib, "user32.lib")

static const DWORD MOVIE_OFFSET = 0x426;
static const DWORD MPV_START_DELAY_MS = 3000;

static void PumpMessages(DWORD ms)
{
    DWORD start = GetTickCount();
    while (GetTickCount() - start < ms) {
        MSG msg;
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        Sleep(10);
    }
}

static bool FileExistsA2(const char* path)
{
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

static bool GetModuleDir(HMODULE mod, char* outDir, DWORD outSize)
{
    if (!mod || !outDir || outSize == 0) return false;

    DWORD len = GetModuleFileNameA(mod, outDir, outSize);
    if (len == 0 || len >= outSize) return false;

    char* lastSlash = strrchr(outDir, '\\');
    if (!lastSlash) return false;
    *lastSlash = '\0';
    return true;
}

static bool GetSelfModule(HMODULE* outMod)
{
    if (!outMod) return false;

    HMODULE mod = nullptr;
    if (!GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&GetSelfModule,
            &mod)) {
        return false;
    }

    *outMod = mod;
    return true;
}

static bool ExtractMoviePac(const char* pacPath, const char* mpgPath)
{
    HANDLE hIn = CreateFileA(
        pacPath,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hIn == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD fileSize = GetFileSize(hIn, nullptr);
    if (fileSize == INVALID_FILE_SIZE || fileSize <= MOVIE_OFFSET) {
        CloseHandle(hIn);
        return false;
    }

    HANDLE hOut = CreateFileA(
        mpgPath,
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hOut == INVALID_HANDLE_VALUE) {
        CloseHandle(hIn);
        return false;
    }

    if (SetFilePointer(hIn, MOVIE_OFFSET, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER &&
        GetLastError() != NO_ERROR) {
        CloseHandle(hOut);
        CloseHandle(hIn);
        return false;
    }

    BYTE buffer[64 * 1024];
    DWORD remaining = fileSize - MOVIE_OFFSET;

    while (remaining > 0) {
        DWORD toRead = (remaining > sizeof(buffer)) ? (DWORD)sizeof(buffer) : remaining;
        DWORD readBytes = 0;
        DWORD writtenBytes = 0;

        if (!ReadFile(hIn, buffer, toRead, &readBytes, nullptr) || readBytes == 0) {
            CloseHandle(hOut);
            CloseHandle(hIn);
            return false;
        }

        if (!WriteFile(hOut, buffer, readBytes, &writtenBytes, nullptr) || writtenBytes != readBytes) {
            CloseHandle(hOut);
            CloseHandle(hIn);
            return false;
        }

        remaining -= readBytes;
    }

    FlushFileBuffers(hOut);
    CloseHandle(hOut);
    CloseHandle(hIn);
    return true;
}

static void ForceFocusCycle(HWND gameWnd)
{
    WNDCLASSA wc{};
    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = "PalProxyDummyWindow";

    RegisterClassA(&wc);

    HWND dummy = CreateWindowExA(
        WS_EX_TOPMOST,
        "PalProxyDummyWindow",
        "dummy",
        WS_POPUP,
        0, 0, 1, 1,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    if (!dummy) return;

    ShowWindow(dummy, SW_SHOW);
    SetForegroundWindow(dummy);
    UpdateWindow(dummy);

    PumpMessages(200);

    DestroyWindow(dummy);

    if (gameWnd) {
        SetForegroundWindow(gameWnd);
        ShowWindow(gameWnd, SW_SHOW);
        UpdateWindow(gameWnd);
    }

    PumpMessages(200);
}

static void RestoreGameWindow(HWND gameWnd)
{
    if (!gameWnd) return;

    ShowWindowAsync(gameWnd, SW_RESTORE);
    ShowWindowAsync(gameWnd, SW_SHOW);

    SetWindowPos(
        gameWnd, HWND_TOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW
    );
    SetWindowPos(
        gameWnd, HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW
    );

    DWORD curThread = GetCurrentThreadId();
    DWORD gameThread = GetWindowThreadProcessId(gameWnd, nullptr);

    AttachThreadInput(curThread, gameThread, TRUE);
    BringWindowToTop(gameWnd);
    SetForegroundWindow(gameWnd);
    SetActiveWindow(gameWnd);
    SetFocus(gameWnd);
    AttachThreadInput(curThread, gameThread, FALSE);

    UpdateWindow(gameWnd);
    PumpMessages(300);
}

extern "C" __declspec(dllexport)
int __cdecl PalVideoPlay(char* path)
{
    (void)path;

    HMODULE selfMod = nullptr;
    if (!GetSelfModule(&selfMod)) {
        return 0;
    }

    char dllDir[MAX_PATH] = {};
    if (!GetModuleDir(selfMod, dllDir, MAX_PATH)) {
        return 0;
    }

    // dllDir = ...\FLYABLE_HEART\dll
    char rootDir[MAX_PATH] = {};
    lstrcpynA(rootDir, dllDir, MAX_PATH);

    char* slash = strrchr(rootDir, '\\');
    if (!slash) {
        return 0;
    }
    *slash = '\0';
    // rootDir = ...\FLYABLE_HEART

    char mpvPath[MAX_PATH] = {};
    char pacPath[MAX_PATH] = {};
    char mpgPath[MAX_PATH] = {};
    char cmd[2048] = {};

    wsprintfA(mpvPath, "%s\\mpv.exe", rootDir);
    wsprintfA(pacPath, "%s\\movie.pac", rootDir);
    wsprintfA(mpgPath, "%s\\OPENING.mpg", rootDir);

    if (!FileExistsA2(mpvPath)) {
        return 0;
    }

    if (!FileExistsA2(mpgPath)) {
        if (!FileExistsA2(pacPath)) {
            return 0;
        }
        if (!ExtractMoviePac(pacPath, mpgPath)) {
            return 0;
        }
    }

    HWND gameWnd = GetForegroundWindow();
    ForceFocusCycle(gameWnd);

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    wsprintfA(
        cmd,
        "\"%s\" "
        "--force-window=yes "
        "--fullscreen=yes "
        "--no-terminal "
        "--osc=no "
        "--osd-level=0 "
        "--cursor-autohide=0 "
        "--input-conf=\"%s\\mpv_input.conf\" "
        "--pause=yes "
        "\"%s\"",
        mpvPath,
        rootDir,
        mpgPath
    );

    BOOL ok = CreateProcessA(
        mpvPath,
        cmd,
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        rootDir,
        &si,
        &pi
    );

    if (!ok) {
        return 0;
    }

    Sleep(MPV_START_DELAY_MS);

    HWND mpvWnd = nullptr;
    for (int i = 0; i < 1000; i++) {
        mpvWnd = FindWindowA("mpv", nullptr);
        if (mpvWnd && IsWindowVisible(mpvWnd)) {
            break;
        }
        Sleep(10);
    }

    if (mpvWnd) {
        PostMessageA(mpvWnd, WM_KEYDOWN, VK_SPACE, 0);
        PostMessageA(mpvWnd, WM_KEYUP, VK_SPACE, 0);
    }

    for (;;) {
        DWORD wait = MsgWaitForMultipleObjects(
            1,
            &pi.hProcess,
            FALSE,
            INFINITE,
            QS_ALLINPUT
        );

        if (wait == WAIT_OBJECT_0) {
            break;
        }

        if (wait == WAIT_OBJECT_0 + 1) {
            MSG msg;
            while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    RestoreGameWindow(gameWnd);
    return 1;
}