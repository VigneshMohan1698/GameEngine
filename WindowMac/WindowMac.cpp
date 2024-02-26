//
//  WindowMac.cpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//

#include "../Core/EngineIncludeHeaders.hpp"
#include "WindowMac.hpp"

WindowMac::WindowMac(WindowConfig config)
{
    m_config = config;
}

void WindowMac::Startup()
{
    CreateWindow();
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


void WindowMac::CreateWindow()
{
    m_window = NS::Window::alloc()->init(
                                         m_config.windowDimensions,
                                         NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled,
                                         NS::BackingStoreBuffered,
                                         false );
    
    m_window->setTitle(NS::String::string( m_config.windowName, NS::StringEncoding::UTF8StringEncoding ) );
    m_window->makeKeyAndOrderFront( nullptr );
}

void WindowMac::Shutdown()
{
    m_window->release();
    m_menu->release();
}

NS::Menu *WindowMac::GetMenuBar() 
{
    return m_menu;
}

NS::Window *WindowMac::GetWindow() 
{
    return m_window;
}


