#include "kconfig.h"
#include <iostream>
#include <sstream>
#include <QDebug> // Include QDebug

KConfig::KConfig() : hKey(nullptr),
                     windowX(0), windowY(0), windowWidth(0), windowHeight(0),
                     canvasWidth(800), canvasHeight(600), // Default canvas size
                     backgroundColor("#FFFFFF") // Default background color
{
    // Open or create the registry key
    LONG result = RegCreateKeyExA(HKEY_CURRENT_USER, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS) {
        qDebug() << "Registry key opened/created successfully.";
    } else {
        qDebug() << "Failed to open/create registry key. Error:" << result;
    }
}

KConfig::~KConfig() {
    if (hKey != nullptr) {
        RegCloseKey(hKey);
        qDebug() << "Registry key closed.";
    }
}

bool KConfig::loadConfig() {
    if (hKey == nullptr) {
        qDebug() << "Cannot load config: Registry key is not open.";
        return false;
    }

    DWORD dwType;
    DWORD dwSize;
    LONG result;

    qDebug() << "Loading configuration from registry...";

    // Load window position and size
    dwSize = sizeof(int);
    result = RegGetValueA(hKey, NULL, "windowX", RRF_RT_REG_DWORD, &dwType, &windowX, &dwSize);
    qDebug() << "Loading windowX:" << windowX << "Result:" << result;
    dwSize = sizeof(int);
    result = RegGetValueA(hKey, NULL, "windowY", RRF_RT_REG_DWORD, &dwType, &windowY, &dwSize);
    qDebug() << "Loading windowY:" << windowY << "Result:" << result;
    dwSize = sizeof(int);
    result = RegGetValueA(hKey, NULL, "windowWidth", RRF_RT_REG_DWORD, &dwType, &windowWidth, &dwSize);
    qDebug() << "Loading windowWidth:" << windowWidth << "Result:" << result;
    dwSize = sizeof(int);
    result = RegGetValueA(hKey, NULL, "windowHeight", RRF_RT_REG_DWORD, &dwType, &windowHeight, &dwSize);
    qDebug() << "Loading windowHeight:" << windowHeight << "Result:" << result;

    // Load canvas size
    dwSize = sizeof(int);
    result = RegGetValueA(hKey, NULL, "canvasWidth", RRF_RT_REG_DWORD, &dwType, &canvasWidth, &dwSize);
    qDebug() << "Loading canvasWidth:" << canvasWidth << "Result:" << result;
    dwSize = sizeof(int);
    result = RegGetValueA(hKey, NULL, "canvasHeight", RRF_RT_REG_DWORD, &dwType, &canvasHeight, &dwSize);
    qDebug() << "Loading canvasHeight:" << canvasHeight << "Result:" << result;

    // Load background color
    char colorBuffer[256];
    dwSize = sizeof(colorBuffer);
    result = RegGetValueA(hKey, NULL, "backgroundColor", RRF_RT_REG_SZ, &dwType, colorBuffer, &dwSize);
    if (result == ERROR_SUCCESS) {
        backgroundColor = colorBuffer;
        qDebug() << "Loading backgroundColor:" << backgroundColor.c_str() << "Result:" << result;
    } else {
        qDebug() << "Failed to load backgroundColor. Result:" << result;
    }

    return true;
}

bool KConfig::saveConfig() {
    if (hKey == nullptr) {
        qDebug() << "Cannot save config: Registry key is not open.";
        return false;
    }

    LONG result;

    qDebug() << "Saving configuration to registry...";

    // Save window position and size
    result = RegSetValueExA(hKey, "windowX", 0, REG_DWORD, (const BYTE*)&windowX, sizeof(int));
    qDebug() << "Saving windowX:" << windowX << "Result:" << result;
    result = RegSetValueExA(hKey, "windowY", 0, REG_DWORD, (const BYTE*)&windowY, sizeof(int));
    qDebug() << "Saving windowY:" << windowY << "Result:" << result;
    result = RegSetValueExA(hKey, "windowWidth", 0, REG_DWORD, (const BYTE*)&windowWidth, sizeof(int));
    qDebug() << "Saving windowWidth:" << windowWidth << "Result:" << result;
    result = RegSetValueExA(hKey, "windowHeight", 0, REG_DWORD, (const BYTE*)&windowHeight, sizeof(int));
    qDebug() << "Saving windowHeight:" << windowHeight << "Result:" << result;

    // Save canvas size
    result = RegSetValueExA(hKey, "canvasWidth", 0, REG_DWORD, (const BYTE*)&canvasWidth, sizeof(int));
    qDebug() << "Saving canvasWidth:" << canvasWidth << "Result:" << result;
    result = RegSetValueExA(hKey, "canvasHeight", 0, REG_DWORD, (const BYTE*)&canvasHeight, sizeof(int));
    qDebug() << "Saving canvasHeight:" << canvasHeight << "Result:" << result;

    // Save background color
    result = RegSetValueExA(hKey, "backgroundColor", 0, REG_SZ, (const BYTE*)backgroundColor.c_str(), backgroundColor.length() + 1);
    qDebug() << "Saving backgroundColor:" << backgroundColor.c_str() << "Result:" << result;

    return true;
}
