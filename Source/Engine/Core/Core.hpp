//////////////////////////////////////////////////////////////////////
#pragma once
#include "Matter/Color.hpp"
#include "Matter/Colors.hpp"
#include "Matter/Point2.hpp"
#include "Matter/Size.hpp"
#include "Matter/Vertex3F.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class CursorTypes
    /*//////////////////*/
    {
        Normal,
        Hovering,
        Rotating,
        Attack
    };

    class Cursor : public Singleton<Cursor>
    /*///////////////////////////////////*/
    {
      public:
        Cursor();
        void Render() const;
        void Reset();
        void SavePosition();
        void RestoreSavedPosition() const;
        Point2 GetSavedPosition() const;
        void SetCursorType(CursorTypes);
        void LockMousePos() const;
        void UnlockMousePos() const;
        void HideThisFrame();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class Logger : public Singleton<Logger>
    /*///////////////////////////////////*/
    {
      public:
        Logger();
        ~Logger();
        void
            Log(std::string_view = "",
                const std::source_location = std::source_location::current());

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    void Log(const std::source_location location = std::source_location::current());

    struct TextLine
    /*///////////*/
    {
        std::string text;
        Color color;
    };

    class TextOutBox : public Singleton<TextOutBox>
    /*///////////////////////////////////////////*/
    {
      public:
        TextOutBox();
        void Update();
        void Render() const;
        void Print(std::string_view, Color = Colors::wheat);

      private:
        int GetMaxNumLines() const;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class Graphics : public Singleton<Graphics>
    /*///////////////////////////////////////*/
    {
      public:
        Graphics();
        SDL_Window *GetWindow() const;
        void ClearCanvas() const;
        void PresentCanvas() const;
        void Cleanup();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class GraphicsGl : public Singleton<GraphicsGl>
    /*///////////////////////////////////////////*/
    {
      public:
        GraphicsGl();
        void Cleanup();
        Color GetFogColorGround();
        Color GetFogColorObjects();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class Audio : public Singleton<Audio>
    /*/////////////////////////////////*/
    {
      public:
        Audio();
        void Cleanup();
        void PlaySound(std::string_view soundNameHash, int = 0);
        void PlayMusic(std::string_view musicNameHash);
        void StopPlaySound();
        void Mute();
        void Unmute();

      private:
        bool muted = false;
    };

    struct ModelPartKeyframe
    /*////////////////////*/
    {
        std::vector<Vertex3F> vertices;
    };

    struct ModelPartTimeline
    /*////////////////////*/
    {
        std::map<float, std::shared_ptr<const ModelPartKeyframe>> keyframes;
    };

    struct ModelPart
    /*////////////*/
    {
        ModelPart();
        int texNameHash = 0;
        std::unique_ptr<ModelPartTimeline> timeline;
    };

    struct Model
    /*////////*/
    {
        Model(int);
        const int animDuration = 0;
        std::vector<std::shared_ptr<const ModelPart>> modelParts;
    };

    enum class Directions
    /*/////////////////*/
    {
        Clockwise = 1,
        CounterClockwise = -1
    };

    Size GetCanvasSize();
    float GetAspectRatio();
    float ConvertWidthToHeight(float);
    std::string_view GetCurrentTime();
    std::string_view GetCurrentDateTime();
    const int Hash(const std::string_view &);
    Point2 GetMousePositionPx();
    Point2F GetMousePositionF();

    class KeyboardInput : public Singleton<KeyboardInput>
    /*/////////////////////////////////////////////////*/
    {
      public:
        KeyboardInput();
        void KeyDown(SDL_Keycode);
        void KeyUp(SDL_Keycode);
        bool AnyKeyHasBeenFired() const;
        bool KeyIsPressed(SDL_Keycode) const;
        bool KeyHasBeenFiredPickResult(SDL_Keycode);
        std::string_view PickTextInput();
        void ResetTextInput();
        void AppendTextInput(std::string_view);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class MouseButton
    /*/////////////*/
    {
      public:
        MouseButton();
        void Reset();
        void PressDown();
        void Release();
        int GetPressDuration() const;
        int ClickDurationPickResult();
        int ClickDurationPeekResult() const;
        void
        AddFiredAction(std::string_view, std::function<void()>, float = 0, int = 0, bool = false);
        void AddReleasedAction(
            std::string_view, std::function<void()>, float = 0, int = 0, bool = false);
        void PerformMouseAction();
        void ResetActions();
        static const int GetDefaultClickSpeed();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class MouseInput : public Singleton<MouseInput>
    /*///////////////////////////////////////////*/
    {
      public:
        MouseInput();
        void Reset();
        void MouseDown(Uint8);
        void MouseUp(Uint8);
        void MouseWheel(int);
        void MouseMove(Point2);
        int MouseWheelDeltaPickResult();
        void PerformMouseActions();
        void ResetMouseActions();
        Point2 GetMotionDeltaPickResults();
        MouseButton &GetLeftButton();
        MouseButton &GetMiddleButton();
        MouseButton &GetRightButton();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    struct MouseAction
    /*//////////////*/
    {
        std::function<void()> action;
        float priority = 0;
        Uint64 delay = 0;
        bool ensureIsExec = false;
    };

    class MouseActionManager
    /*////////////////////*/
    {
      public:
        MouseActionManager();
        void PerformFiredActions(int, bool, bool &);
        void PerformReleasedActions(int);
        bool FiredActionsContains(int);
        bool ReleasedActionsContains(int);
        void ClearFiredActions();
        void ClearReleasedActions();
        void AddFiredAction(int, MouseAction);
        void AddReleasedAction(int, MouseAction);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////