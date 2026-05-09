#include <windows.h>
#include <string>

#define IDR_PROXY_PAL 101
#define IDR_MPV_INPUT 102

static std::string GetExeDir()
{
    char path[MAX_PATH] = {};
    GetModuleFileNameA(nullptr, path, MAX_PATH);

    char* slash = strrchr(path, '\\');
    if (slash) *slash = '\0';

    return std::string(path);
}

static bool FileExists(const std::string& path)
{
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

static bool DirectoryExists(const std::string& path)
{
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

static bool WriteResourceToFile(WORD resId, LPCSTR resType, const std::string& outPath)
{
    HRSRC hRes = FindResourceA(nullptr, MAKEINTRESOURCEA(resId), resType);
    if (!hRes) {
        MessageBoxA(nullptr, "FindResourceA failed", "Patch", MB_OK);
        return false;
    }

    HGLOBAL hData = LoadResource(nullptr, hRes);
    if (!hData) {
        MessageBoxA(nullptr, "LoadResource failed", "Patch", MB_OK);
        return false;
    }

    DWORD size = SizeofResource(nullptr, hRes);
    void* ptr = LockResource(hData);
    if (!ptr || size == 0) {
        MessageBoxA(nullptr, "LockResource / SizeofResource failed", "Patch", MB_OK);
        return false;
    }

    HANDLE hFile = CreateFileA(
        outPath.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        char buf[256];
        wsprintfA(buf, "CreateFileA failed: %lu\nPath: %s", GetLastError(), outPath.c_str());
        MessageBoxA(nullptr, buf, "Patch", MB_OK);
        return false;
    }

    DWORD written = 0;
    BOOL ok = WriteFile(hFile, ptr, size, &written, nullptr);
    CloseHandle(hFile);

    if (!ok || written != size) {
        char buf[256];
        wsprintfA(buf, "WriteFile failed: %lu", GetLastError());
        MessageBoxA(nullptr, buf, "Patch", MB_OK);
        return false;
    }

    return true;
}

static bool IsGameRunning()
{
    HWND wnd = FindWindowA(nullptr, "Flyable Heart");
    return wnd != nullptr;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::string root = GetExeDir();
    std::string dllDir = root + "\\dll";
    std::string palPath = dllDir + "\\PAL.dll";
    std::string palOrigPath = dllDir + "\\PAL_orig.dll";
    std::string inputConfPath = root + "\\mpv_input.conf";

    if (IsGameRunning()) {
        MessageBoxA(nullptr,
            "Flyable Heart is running.\nPlease close the game first.",
            "Patch",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    if (!DirectoryExists(dllDir)) {
        MessageBoxA(nullptr,
            "dll folder not found.",
            "Patch",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    if (!FileExists(palPath) && !FileExists(palOrigPath)) {
        MessageBoxA(nullptr,
            "Original PAL.dll not found in dll folder.",
            "Patch",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    // 初回だけ退避
    if (!FileExists(palOrigPath)) {
        if (!MoveFileA(palPath.c_str(), palOrigPath.c_str())) {
            MessageBoxA(nullptr,
                "Failed to rename original PAL.dll to PAL_orig.dll",
                "Patch",
                MB_ICONERROR | MB_OK);
            return 1;
        }
    } else {
        DeleteFileA(palPath.c_str());
    }

    // 新proxy DLLを書き出し
    if (!WriteResourceToFile(IDR_PROXY_PAL, RT_RCDATA, palPath)) {
        MessageBoxA(nullptr,
            "Failed to write proxy PAL.dll",
            "Patch",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    // mpv_input.confを書き出し
    if (!WriteResourceToFile(IDR_MPV_INPUT, RT_RCDATA, inputConfPath)) {
        MessageBoxA(nullptr,
            "Failed to write mpv_input.conf",
            "Patch",
            MB_ICONERROR | MB_OK);
        return 1;
    }

    MessageBoxA(nullptr,
        "Patch installed successfully.",
        "Patch",
        MB_ICONINFORMATION | MB_OK);

    return 0;
}