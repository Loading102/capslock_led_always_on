#include <windows.h>
#include <ntddkbd.h>
#include <iostream>

int main() {
    HANDLE keyboard = CreateFileW(
        L"\\\\.\\KbdClass0",
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);

    if (keyboard == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open keyboard device. Error: " << GetLastError() << std::endl;
        return 1;
    }

    KEYBOARD_INDICATOR_PARAMETERS params{};
    params.UnitId = 0;
    params.LedFlags = KEYBOARD_CAPS_LOCK_ON;

    DWORD bytesReturned = 0;
    BOOL result = DeviceIoControl(
        keyboard,
        IOCTL_KEYBOARD_SET_INDICATORS,
        &params,
        sizeof(params),
        nullptr,
        0,
        &bytesReturned,
        nullptr);

    if (!result) {
        std::cerr << "DeviceIoControl failed. Error: " << GetLastError() << std::endl;
        CloseHandle(keyboard);
        return 1;
    }

    std::cout << "Caps Lock LED enabled." << std::endl;
    CloseHandle(keyboard);
    return 0;
}