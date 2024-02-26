//
//  WindowMac.hpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//

#ifndef WindowMac_hpp
#define WindowMac_hpp

#include <cassert>
#include <stdio.h>
#include <iostream>
#include "../Core/Rgba8.hpp"

struct WindowConfig
{
    const char* windowName = nullptr;
    CGRect windowDimensions;
};

class WindowMac 
{
public:
    WindowMac(WindowConfig config);
    ~WindowMac() {};
    
    void Startup();
    void Shutdown();
    
    void CreateMenuBar();
    void CreateWindow();
    
    NS::Menu*  GetMenuBar();
    NS::Window* GetWindow();
    
private:
    NS::Window* m_window = nullptr;
    NS::Menu*   m_menu = nullptr;
    WindowConfig m_config;
};
#endif /* WindowMac_hpp */
