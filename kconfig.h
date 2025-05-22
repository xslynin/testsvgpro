#ifndef KCONFIG_H
#define KCONFIG_H

#include <string>
#include <windows.h>

class KConfig {
public:
    KConfig();
    ~KConfig();

    // Load configuration from the registry
    bool loadConfig();

    // Save configuration to the registry
    bool saveConfig();

    // Configuration data members
    int windowX;
    int windowY;
    int windowWidth;
    int windowHeight;
    int canvasWidth;
    int canvasHeight;
    std::string backgroundColor; // Assuming background color can be stored as a string

private:
    HKEY hKey;
    const char* subKey = "SOFTWARE\\svgeditor";
};

#endif // KCONFIG_H
