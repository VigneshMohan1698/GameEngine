//
//  WindowMac.cpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION


#include "../Core/EngineIncludeHeaders.hpp"
#include "WindowMac.hpp"
#include "iostream"
#include "stdio.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>


WindowMac::WindowMac(WindowConfig config)
{
    m_config = config;
}

void WindowMac::Startup()
{
    CreateWindow();
}

void WindowMac::CreateWindow()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "SDL could not be initialized: " <<
                  SDL_GetError();
    } else {
        std::cout << "SDL video system is ready to go\n";
    }
    m_sdlWindow = SDL_CreateWindow(m_config.windowName,
                                                SDL_WINDOWPOS_CENTERED,
                                                SDL_WINDOWPOS_CENTERED,
                                   m_config.windowDimensions.CGRect::size.width, m_config.windowDimensions.CGRect::size.height, SDL_WINDOW_SHOWN);

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(m_sdlWindow, &info) && info.subsystem == SDL_SYSWM_COCOA)
    {
        m_window = (NS::Window*)info.info.cocoa.window;
    }
   
    m_window->setTitle(NS::String::string( m_config.windowName, NS::StringEncoding::UTF8StringEncoding ) );
    m_window->makeKeyAndOrderFront( nullptr );
}

void WindowMac::Update()
{
    SDL_Event event;
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_X])
    {
        std::cout << "Key pressed: X";
    }
    while (SDL_PollEvent(&event)) {
        SDL_KeyboardEvent key = event.key;
        
        std::cout << key.type;
        if (event.type == SDL_QUIT) {
            SDL_Quit();
        }
        
        else if(event.type == SDL_KEYDOWN)
        {
            std::cout << "Hit key: " << event.key.keysym.sym;
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    SDL_Quit();
                    break;
                case SDLK_UP:
                    std::cout << "UP key pressed." << std::endl;
                    break;
                default:
                    break;
            }
        }
        
    }
}

void WindowMac::CreateMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    m_menu = NS::Menu::alloc()->init();
    NS::MenuItem* pAppMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* pAppMenu = NS::Menu::alloc()->init( NS::String::string( m_config.windowName, UTF8StringEncoding ) );

    NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( appName );
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate( pSender );
    } );

    NS::MenuItem* pAppQuitItem = pAppMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
    pAppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
    pAppMenuItem->setSubmenu( pAppMenu );

    NS::MenuItem* pWindowMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* pWindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback( "windowClose", [](void*, SEL, const NS::Object*){
        auto pApp = NS::Application::sharedApplication();
            pApp->windows()->object< NS::Window >(0)->close();
    } );
    NS::MenuItem* pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding ) );
    pCloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

    pWindowMenuItem->setSubmenu( pWindowMenu );

    m_menu->addItem( pAppMenuItem );
    m_menu->addItem( pWindowMenuItem );

    pAppMenuItem->release();
    pWindowMenuItem->release();
    pAppMenu->release();
    pWindowMenu->release();

    m_menu = m_menu->autorelease();
}

void WindowMac::Shutdown()
{
    m_window->release();
    m_menu->release();
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
}

NS::Menu *WindowMac::GetMenuBar() 
{
    return m_menu;
}

NS::Window *WindowMac::GetWindow() 
{
    return m_window;
}


