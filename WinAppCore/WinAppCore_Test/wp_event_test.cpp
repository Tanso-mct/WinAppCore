#include "pch.h"

#include "WACore.h"

static WACore::WPEventSet gWPEventSet;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    gWPEventSet.Call(hWnd, msg, msg, wParam, lParam);

    switch (msg)
    {
    case WM_PAINT:
        // Returning DefWindowProc with WM_PAINT will stop the updating, so WM_PAINT is returned 0.
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

class Item : public WACore::IContainable
{
private:
    std::string name_;

public:
    Item(std::string_view name) : name_(name)
    {
        std::cout << name_ << " is created." << std::endl;
    }

    virtual ~Item() override
    {
        std::cout << name_ << " is destroyed." << std::endl;
    }
};

class WindowEvent : public WACore::IWPEvent
{
public:
    WindowEvent() = default;
    virtual ~WindowEvent() override = default;

    void OnSetFocus(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Window is focused." << std::endl;
    }

    void OnKillFocus(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Window is unfocused." << std::endl;
    }

    void OnSize(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Window size is changed." << std::endl;
    }

    void OnUpdate(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        // std::cout << "Window is updated." << std::endl;
    }

    void OnMove(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Window is moved." << std::endl;
    }

    void OnDestroy(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Window is destroyed." << std::endl;
        PostQuitMessage(0);
    }

    void OnKeyDown(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Key is pressed." << std::endl;
    }

    void OnKeyUp(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Key is released." << std::endl;
    }

    void OnMouse(std::unique_ptr<WACore::IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        std::cout << "Mouse event." << std::endl;
    }
};

TEST(WinAppCore_window_procedure_event, WPEventCall)
{
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WindowProc,
        0,
        0,
        nullptr,
        LoadIcon(nullptr, IDI_APPLICATION),
        LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        nullptr,
        L"WPEventCall",
        LoadIcon(nullptr, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&wcex)) EXPECT_TRUE(false);

    HWND hWnd = CreateWindow(
        L"WPEventCall",
        L"WPEventCall",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    if (!hWnd) EXPECT_TRUE(false);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // Support functions are used to register event functions.
    WACore::RegisterWPEventFuncs(gWPEventSet);

    gWPEventSet.Caller()->AddKey(hWnd);
    gWPEventSet.Caller()->AddEvent(hWnd, std::make_unique<WindowEvent>());

    std::unique_ptr<WACore::IContainer> container = std::make_unique<WACore::Container>();
    container->Add(std::make_unique<Item>("Item1"));
    container->Add(std::make_unique<Item>("Item2"));
    gWPEventSet.Caller()->AddContainer(hWnd, std::move(container));

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        // Process any messages in the queue.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}