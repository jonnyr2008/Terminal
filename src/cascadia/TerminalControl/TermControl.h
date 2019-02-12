#pragma once

#include "TermControl.g.h"
#include <winrt/TerminalConnection.h>
#include "../../renderer/base/Renderer.hpp"
#include "../../renderer/dx/DxRenderer.hpp"
#include "../../cascadia/TerminalCore/Terminal.hpp"

namespace winrt::TerminalControl::implementation
{
    struct TermControl : TermControlT<TermControl>
    {
        TermControl();
        TermControl(TerminalControl::TerminalSettings settings);

        Windows::UI::Xaml::UIElement GetRoot();
        Windows::UI::Xaml::Controls::UserControl GetControl();

        winrt::event_token TitleChanged(TerminalControl::TitleChangedEventArgs const& handler);
        void TitleChanged(winrt::event_token const& token) noexcept;
        winrt::event_token ConnectionClosed(TerminalControl::ConnectionClosedEventArgs const& handler);
        void ConnectionClosed(winrt::event_token const& token) noexcept;

        hstring GetTitle();

        void Close();

        void ScrollViewport(int viewTop);
        int GetScrollOffset();
        winrt::event_token ScrollPositionChanged(TerminalControl::ScrollPositionChangedEventArgs const& handler);
        void ScrollPositionChanged(winrt::event_token const& token) noexcept;

        void SwapChainChanged();
        ~TermControl();

    private:
        winrt::event<TerminalControl::TitleChangedEventArgs> _titleChangeHandlers;
        winrt::event<TerminalControl::ConnectionClosedEventArgs> _connectionClosedHandlers;
        winrt::event<TerminalControl::ScrollPositionChangedEventArgs> _scrollPositionChangedHandlers;

        TerminalConnection::ITerminalConnection _connection;
        bool _initializedTerminal;

        Windows::UI::Xaml::Controls::UserControl _controlRoot;
        Windows::UI::Xaml::Controls::Grid _root;
        Windows::UI::Xaml::Controls::Grid _fakeScrollRoot;
        Windows::UI::Xaml::Controls::SwapChainPanel _swapChainPanel;
        Windows::UI::Xaml::Controls::ScrollViewer _scrollViewer;
        event_token _connectionOutputEventToken;

        ::Microsoft::Terminal::Core::Terminal* _terminal;

        std::unique_ptr<::Microsoft::Console::Render::Renderer> _renderer;
        std::unique_ptr<::Microsoft::Console::Render::DxEngine> _renderEngine;

        TerminalControl::TerminalSettings _settings;
        bool _closing;
        int _bottomPadding = 0;

        void _Create();
        void _ApplySettings();
        void _InitializeTerminal();
        void KeyHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e);
        void CharacterHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::CharacterReceivedRoutedEventArgs const& e);

        void _SendInputToConnection(const std::wstring& wstr);
        void _SwapChainSizeChanged(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::SizeChangedEventArgs const& e);
    };
}

namespace winrt::TerminalControl::factory_implementation
{
    struct TermControl : TermControlT<TermControl, implementation::TermControl>
    {
    };
}
