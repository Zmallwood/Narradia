//////////////////////////////////////////////////////////////////////
#include "Core.hpp"
#include "Assets.hpp"
#include "Scenes/BasicScenes.hpp"
#include "Scenes/EditorScene/EditorScene.hpp"
#include "Scenes/PlayScene/SubScenes/GameSaveScene.hpp"
#include "GuiCore.hpp"
#include "Scenes/MapGenerationScene/MapGenerateScene.hpp"
#include "Scenes/EditorScene/SubScenes/MapLoadScene.hpp"
#include "Scenes/EditorScene/SubScenes/MapSaveScene.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Scenes/PlayScene/Gui/PlaySceneGuiCore.hpp"
#include "Rendering.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
/*////////////////////////////*/ {
    Narradia::GameEngine::Get().Run();
    return 0;
} // Function
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameEngine::Pimpl
    /*///////////////////*/ {
      public:
        bool running = true;
    }; // Class

    GameEngine::GameEngine()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void GameEngine::Run() const
    /*////////////////////////*/ {
        Logger::Create();
        srand(static_cast<unsigned int>(time(nullptr)));
        Log();
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        /************************************/ {
            std::cout << "SDL could not initialize! SDL Error: " << std::string(SDL_GetError())
                      << std::endl;
            return;
        }
        Log();
        Graphics::Create();
        Log();
        Audio::Create();
        Log();
        TextOutBox::Create();
        TextOutBox::Get().Print("Game started.");
        Log();
        GameLoop().Run();
        SDL_Quit();
    } // Function

    void GameEngine::StopGame()
    /*///////////////////////*/ { p->running = false; } // Function

    const bool GameEngine::IsRunning() const
    /*////////////////////////////////////*/ { return p->running; } // Function

    void GameLoop::Run() const
    /*//////////////////////*/ {
        Log();
        World::Create();
        Log();
        SceneManager::Get().InitializeScenes();
        Log();
        EventPoller eventPoller;
        while (GameEngine::Get().IsRunning())
        /*********************************/ {
            Log();
            Cursor::Get().Reset();
            TextOutBox::Get().Update();
            Log();
            Graphics::Get().ClearCanvas();
            Log();
            SceneManager::Get().UpdateCurrentView();
            Log();
            eventPoller.PollEvents();
            Log();
            MouseInput::Get().PerformMouseActions();
            Log();
            SceneManager::Get().RenderCurrentView();
            Log();
            SceneManager::Get().FinalizeCurrentView();
            Log();
            TextOutBox::Get().Render();
            Log();
            Cursor::Get().Render();
            Log();
            Graphics::Get().PresentCanvas();
        }
        Log();
        Graphics::Get().Cleanup();
        Log();
        Audio::Get().Cleanup();
    } // Function

    void EventPoller::PollEvents() const
    /*////////////////////////////////*/ {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        /********************************/ {
            switch (event.type)
            /*****************/ {
            case SDL_QUIT:
                GameEngine::Get().StopGame();
                break;
            case SDL_KEYDOWN:
                KeyboardInput::Get().KeyDown(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                KeyboardInput::Get().KeyUp(event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                MouseInput::Get().MouseDown(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                MouseInput::Get().MouseUp(event.button.button);
                break;
            case SDL_MOUSEMOTION:
                MouseInput::Get().MouseMove({event.motion.xrel, event.motion.yrel});
                break;
            case SDL_TEXTINPUT:
                KeyboardInput::Get().AppendTextInput(event.text.text);
                break;
            case SDL_MOUSEWHEEL:
                MouseInput::Get().MouseWheel(event.wheel.y);
                break;
            }
        }
    } // Function

    class SceneBase::Pimpl
    /*/////////////////*/ {
      public:
        std::shared_ptr<SceneGui> sceneGui;
    }; // Class

    SceneBase::SceneBase()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void SceneBase::Enter()
    /*//////////////////*/ {} // Function

    void SceneBase::Update()
    /*///////////////////*/ {
        p->sceneGui->Update();
        UpdateDerived();
    } // Function

    void SceneBase::Render()
    /*///////////////////*/ {
        Log();
        RenderDerived();
        Log();
        p->sceneGui->Render();
        Log();
        RenderAfterGuiDerived();
    } // Function

    void SceneBase::Finalize()
    /*/////////////////////*/ {} // Function

    SceneGui *SceneBase::GetSceneGui()
    /*///////////////////////////*/ { return p->sceneGui.get(); } // Function

    void SceneBase::CreateGui()
    /*//////////////////////*/ { SetSceneGui(std::make_shared<SceneGui>()); } // Function

    void SceneBase::RenderAfterGuiDerived()
    /*//////////////////////////////////*/ {} // Function

    void SceneBase::SetSceneGui(std::shared_ptr<SceneGui> newSceneGui)
    /*///////////////////////////////////////////////////////////*/ {
        p->sceneGui = newSceneGui;
    } // Function

    class SceneManager::Pimpl
    /*////////////////////*/ {
      public:
        std::map<Scenes, std::shared_ptr<SceneBase>> scenes;
        Scenes currentView = Scenes::None;
    }; // Class

    SceneManager::SceneManager()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void SceneManager::InitializeScenes()
    /*///////////////////////////////*/ {
        Log();
        p->scenes[Scenes::Intro] = std::make_shared<IntroScene>();
        Log();
        p->scenes[Scenes::MainMenu] = std::make_shared<MainMenuScene>();
        Log();
        p->scenes[Scenes::MapTypeSelection] = std::make_shared<MapTypeSelectionScene>();
        Log();
        p->scenes[Scenes::UserMapSelection] = std::make_shared<UserMapSelectionScene>();
        Log();
        p->scenes[Scenes::MapGeneration] = std::make_shared<MapGenerateScene>();
        Log();
        p->scenes[Scenes::GameSave] = std::make_shared<GameSaveScene>();
        Log();
        p->scenes[Scenes::Play] = PlayScene::GetPointer();
        Log();
        p->scenes[Scenes::Editor] = std::make_shared<EditorScene>();
        Log();
        p->scenes[Scenes::MapSave] = std::make_shared<MapSaveScene>();
        Log();
        p->scenes[Scenes::MapLoad] = std::make_shared<MapLoadScene>();
        p->currentView = Scenes::Intro;
    } // Function

    void SceneManager::UpdateCurrentView()
    /*/////////////////////////////////*/ {
        Log();
        if (p->scenes.count(p->currentView))
        /**********************************/ {
            Log();
            p->scenes.at(p->currentView)->Update();
        };
    } // Function

    void SceneManager::RenderCurrentView() const
    /*///////////////////////////////////////*/ {
        Log();
        if (p->scenes.count(p->currentView))
        /**********************************/ {
            Log();
            p->scenes.at(p->currentView)->Render();
        };
        Log();
    } // Function

    void SceneManager::FinalizeCurrentView()
    /*///////////////////////////////////*/ {
        if (p->scenes.count(p->currentView))
        /**********************************/ {
            Log();
            p->scenes.at(p->currentView)->Finalize();
        };
    } // Function

    void SceneManager::ChangeView(Scenes newScene)
    /*////////////////////////////////////////*/ {
        p->currentView = newScene;
        MouseInput::Get().Reset();
        MouseInput::Get().ResetMouseActions();
        p->scenes.at(p->currentView)->Enter();
    } // Function

    Scenes SceneManager::GetCurrentView()
    /*///////////////////////////////*/ { return p->currentView; } // Function

    class Cursor::Pimpl
    /*///////////////*/ {
      public:
        static constexpr float kCursorSize = 0.04f;
        const int hashCursorDefault = Hash("CursorDefault");
        const int hashCursorAttack = Hash("CursorAttack");
        const int hashCursorHovering = Hash("CursorHovering");
        const int hashCursorRotating = Hash("CursorRotating");
        GLuint idxCursorImage;
        Point2 savedMousePosPx;
        CursorTypes currCursorType = CursorTypes::Normal;
        bool visibleThisFrame = true;
    }; // Class

    Cursor::Cursor()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        SDL_ShowCursor(0);
        p->idxCursorImage = Renderer2DImages::Get().NewImage();
    } // Function

    void Cursor::Render() const
    /*///////////////////////*/ {
        if (!p->visibleThisFrame) return;
        const auto mousePos = GetMousePositionF();
        const auto usedCursorSize =
            SizeF{Pimpl::kCursorSize, ConvertWidthToHeight(Pimpl::kCursorSize)};
        const auto bounds = RectangleF{
            mousePos.Translate(-usedCursorSize.width / 2.0f, -usedCursorSize.height / 2.0f),
            usedCursorSize};
        int imageNameHash;
        switch (p->currCursorType)
        /************************/ {
        case CursorTypes::Hovering:
            imageNameHash = p->hashCursorHovering;
            break;
        case CursorTypes::Rotating:
            imageNameHash = p->hashCursorRotating;
            break;
        case CursorTypes::Attack:
            imageNameHash = p->hashCursorAttack;
            break;
        case CursorTypes::Normal:
        default:
            imageNameHash = p->hashCursorDefault;
        }
        Renderer2DImages::Get().DrawImage(imageNameHash, p->idxCursorImage, bounds);
    } // Function

    void Cursor::Reset()
    /*////////////////*/ {
        p->currCursorType = CursorTypes::Normal;
        p->visibleThisFrame = true;
    } // Function

    void Cursor::SavePosition()
    /*///////////////////////*/ { p->savedMousePosPx = GetMousePositionPx(); } // Function

    void Cursor::RestoreSavedPosition() const
    /*/////////////////////////////////////*/ {
        SDL_WarpMouseInWindow(
            Graphics::Get().GetWindow(), p->savedMousePosPx.x, p->savedMousePosPx.y);
    } // Function

    Point2 Cursor::GetSavedPosition() const
    /*///////////////////////////////////*/ { return p->savedMousePosPx; } // Function

    void Cursor::SetCursorType(CursorTypes newCursorType)
    /*/////////////////////////////////////////////////*/ {
        p->currCursorType = newCursorType;
    } // Function

    void Cursor::LockMousePos() const
    /*/////////////////////////////*/ { SDL_SetRelativeMouseMode(SDL_TRUE); } // Function

    void Cursor::UnlockMousePos() const
    /*///////////////////////////////*/ { SDL_SetRelativeMouseMode(SDL_FALSE); } // Function

    void Cursor::HideThisFrame()
    /*////////////////////////*/ { p->visibleThisFrame = false; } // Function

    class Logger::Pimpl
    /*///////////////*/ {
      public:
        static constexpr std::string_view logDirPath = "Logs";
        std::ofstream logFile;
        bool enabled = true;
    }; // Class

    Logger::Logger()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        std::filesystem::remove_all(std::string(SDL_GetBasePath()) + Pimpl::logDirPath.data());
        std::filesystem::create_directory(
            std::string(SDL_GetBasePath()) + Pimpl::logDirPath.data());
        auto logFileName = "Log." + std::string(GetCurrentDateTime().data()) + ".txt";
        p->logFile.open(
            std::string(SDL_GetBasePath()) + Pimpl::logDirPath.data() + "/" + logFileName,
            std::ofstream::out | std::ofstream::trunc);
    } // Function

    Logger::~Logger()
    /*/////////////*/ { p->logFile.close(); } // Function

    void Logger::Log(std::string_view message, const std::source_location location)
    /*///////////////////////////////////////////////////////////////////////////*/ {
        if (!p->enabled) return;
        auto stdstr = std::string(location.file_name());
        auto fileNameStart = stdstr.find("Source/") + 7;
        auto fileName = stdstr.substr(fileNameStart);
        std::string logText = std::string(GetCurrentTime().data()) + "." +
                              std::to_string(SDL_GetTicks() % 1000) + ")" + message.data() + "|" +
                              fileName + "(" + std::to_string(location.line()) + ")" +
                              "                                ";
        p->logFile.seekp(0);
        p->logFile << logText << std::flush;
    } // Function

    void Log()
    /*//////*/ { Logger::Get().Log(); } // Function

    class TextOutBox::Pimpl
    /*///////////////////*/ {
      public:
        RenderId glIdImage = 0;
        RenderId idSplitLine;
        RenderId idCommandLineInputArrow;
        RenderId idInputText;
        std::vector<TextLine> textLines;
        std::vector<GLuint> glIdTextLines;
        const RectangleF bounds = {0.0f, 0.8f, 0.3f, 0.2f};
        const float textLineHeight = 0.02f;
        const float kSplitLineHeight = 0.001f;
        bool enabled = true;
        bool inputActive = false;
        int cursorPosition = 0;
        std::string inputText;
    }; // Class

    TextOutBox::TextOutBox()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        p->glIdImage = Renderer2DImages::Get().NewImage();
        p->idSplitLine = Renderer2DImages::Get().NewImage();
        p->idCommandLineInputArrow = Renderer2DImages::Get().NewImage();
        p->idInputText = TextRenderer::Get().NewString();
        auto maxNumLines = GetMaxNumLines();
        for (auto i = 0; i < maxNumLines; i++)
            p->glIdTextLines.push_back(TextRenderer::Get().NewString());
    } // Function

    void TextOutBox::Update()
    /*/////////////////////*/ {
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_RETURN)) {
            p->inputActive = !p->inputActive;
        }
        if (p->inputActive) {
            auto newTextInput = KeyboardInput::Get().PickTextInput();
            p->inputText.insert(p->cursorPosition, newTextInput);
            p->cursorPosition += newTextInput.length();
        }
    } // Function

    void TextOutBox::Render() const
    /*///////////////////////////*/ {
        if (!p->enabled) return;
        auto usedBounds = p->bounds;
        if (SceneManager::Get().GetCurrentView() == Scenes::Play)
            usedBounds = usedBounds.Translate(0.0f, -ExperienceBar::kBarHeight);
        Renderer2DImages::Get().DrawImage("TextOutBoxBack", p->glIdImage, usedBounds);
        auto maxNumLines = GetMaxNumLines();
        for (auto i = 0; i < maxNumLines; i++)
        /************************************/ {
            auto textLineIndex = static_cast<int>(p->textLines.size()) - maxNumLines + i;
            if (textLineIndex >= 0)
            /*********************/ {
                auto linePosY = usedBounds.y + (i + 1) * p->textLineHeight;
                Point2F position = {usedBounds.x + 0.01f, linePosY};
                TextRenderer::Get().DrawString(
                    p->glIdTextLines.at(i), p->textLines.at(textLineIndex).text, position,
                    p->textLines.at(textLineIndex).color);
            }
        }
        auto rect = RectangleF{
            0.0f, usedBounds.y + usedBounds.height - 1.3f * p->textLineHeight, p->bounds.width,
            p->kSplitLineHeight};
        Renderer2DImages::Get().DrawImage("Wheat", p->idSplitLine, rect);
        if (p->inputActive) {
            auto rectArrow = RectangleF{
                0.0f, usedBounds.y + usedBounds.height - 1.3f * p->textLineHeight,
                p->textLineHeight, p->textLineHeight};
            Renderer2DImages::Get().DrawImage(
                "CommandLineInputArrow", p->idCommandLineInputArrow, rectArrow);
            auto posText =
                rectArrow.GetPosition().Translate(p->textLineHeight, p->textLineHeight / 2);
            auto usedText = p->inputText;
            if (SDL_GetTicks() % 600 < 300)
                usedText.insert(p->cursorPosition, "|");
            else
                usedText.insert(p->cursorPosition, " ");
            TextRenderer::Get().DrawString(p->idInputText, usedText, posText);
        }
    } // Function

    void TextOutBox::Print(std::string_view text, Color textColor)
    /*//////////////////////////////////////////////////////////*/ {
        if (!p->enabled) return;
        std::string printedText = std::string(GetCurrentTime().data()) + "." +
                                  std::to_string(SDL_GetTicks() % 1000) + ") " + text.data();
        p->textLines.push_back({printedText, textColor});
    } // Function

    int TextOutBox::GetMaxNumLines() const
    /*//////////////////////////////////*/ {
        return static_cast<int>(p->bounds.height / p->textLineHeight) - 2;
    } // Function

    class Graphics::Pimpl
    /*/////////////////*/ {
      public:
        const std::string_view windowTitle = "Narradia";
        std::shared_ptr<SDL_Window> window;
        const Size configResolution = {1600, 900};
#if defined(_DEBUG) || !defined(NDEBUG)
        const Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
#else
        const Uint32 windowFlags =
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    }; // Class

    Graphics::Graphics()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        p->window = std::shared_ptr<SDL_Window>(
            SDL_CreateWindow(
                p->windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                p->configResolution.width, p->configResolution.height, p->windowFlags),
            SdlDeleter());
        if (p->window == nullptr)
        /***********************/ {
            std::cout << "Window could not be created! SDL Error: " << std::string(SDL_GetError())
                      << std::endl;
            return;
        }
        if (SDL_GL_CreateContext(p->window.get()) == nullptr)
        /***************************************************/ {
            std::cout << "OpenGL context could not be created! SDL Error: "
                      << std::string(SDL_GetError()) << std::endl;
            return;
        }
        GraphicsGl::GetPointer();
        TextRenderer::Create();
    } // Function

    SDL_Window *Graphics::GetWindow() const
    /*************************************/ { return p->window.get(); } // Function

    void Graphics::ClearCanvas() const
    /********************************/ {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } // Function

    void Graphics::PresentCanvas() const
    /**********************************/ { SDL_GL_SwapWindow(p->window.get()); } // Function

    void Graphics::Cleanup()
    /**********************/ { GraphicsGl::Get().Cleanup(); } // Function

    class GraphicsGl::Pimpl
    /*********************/ {
      public:
        static constexpr bool kCullFace = false;
        const Color fogColorGround = Colors::mildBlue;
        const Color fogColorObjects = Colors::mildBlue;
        const Color clearColor = Colors::black;
    }; // Class

    GraphicsGl::GraphicsGl()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        glewExperimental = GL_TRUE;
        auto glewError = glewInit();
        if (glewError != GLEW_OK)
            printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
        if (SDL_GL_SetSwapInterval(0) < 0)
            printf("Warning: Unable to disable VSync! SDL Error: %s\n", SDL_GetError());
        Renderer2DSolidColors::Create();
        Renderer2DImages::Create();
        RendererTiles::Create();
        RendererModels::Create();
        RendererBillboardImages::Create();
        glClearColor(p->clearColor.r, p->clearColor.g, p->clearColor.b, p->clearColor.a);
        if (Pimpl::kCullFace)
        /*******************/ {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ImageBank::Get().LoadImages();
        ModelBank::Get().LoadModels();
    } // Function

    void GraphicsGl::Cleanup()
    /*//////////////////////*/ {
        Renderer2DSolidColors::Get().Cleanup();
        Renderer2DImages::Get().Cleanup();
        RendererTiles::Get().Cleanup();
        RendererModels::Get().Cleanup();
        RendererBillboardImages::Get().Cleanup();
        ImageBank::Get().Cleanup();
    } // Function

    Color GraphicsGl::GetFogColorGround()
    /*/////////////////////////////////*/ { return p->fogColorGround; } // Function

    Color GraphicsGl::GetFogColorObjects()
    /*//////////////////////////////////*/ { return p->fogColorObjects; } // Function

    Audio::Audio()
    /*//////////*/ {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) return;
        AudioBank::Get().LoadAudioFiles();
    } // Function

    void Audio::Cleanup()
    /*/////////////////*/ { AudioBank::Get().Cleanup(); } // Function

    void Audio::PlaySound(std::string_view soundNameHash, int channel)
    /*//////////////////////////////////////////////////////////////*/ {
        if (muted) return;
        if (Mix_Playing(0) == 0 || channel != 0)
            Mix_PlayChannel(channel, AudioBank::Get().GetSound(Hash(soundNameHash)), 0);
        Mix_Volume(channel, MIX_MAX_VOLUME / 4);
    } // Function

    void Audio::PlayMusic(std::string_view musicNameHash)
    /*/////////////////////////////////////////////////*/ {
        if (Mix_PlayingMusic() == 0)
            Mix_PlayMusic(AudioBank::Get().GetMusic(Hash(musicNameHash)), -1);
    } // Function

    void Audio::StopPlaySound()
    /*///////////////////////*/ { Mix_HaltChannel(0); } // Function

    void Audio::Mute()
    /*//////////////*/ {
        Mix_PauseMusic();
        Mix_Volume(-1, 0);
        muted = true;
    } // Function

    void Audio::Unmute()
    /*////////////////*/ {
        Mix_ResumeMusic();
        Mix_Volume(-1, MIX_MAX_VOLUME / 4);
        muted = false;
    } // Function

    ModelPart::ModelPart()
        : timeline(std::make_unique<ModelPartTimeline>())
    /*/////////////////////////////////////////////////*/ {} // Function

    Model::Model(int animDuration_)
        : animDuration(animDuration_)
    /*/////////////////////////////*/ {} // Function

    Size GetCanvasSize()
    /*////////////////*/ {
        int width;
        int height;
        SDL_GetWindowSize(Graphics::Get().GetWindow(), &width, &height);
        return {width, height};
    } // Function

    float GetAspectRatio()
    /*//////////////////*/ {
        auto canvasSize = GetCanvasSize();
        return static_cast<float>(canvasSize.width) / canvasSize.height;
    } // Function

    float ConvertWidthToHeight(float width)
    /*///////////////////////////////////*/ { return width * GetAspectRatio(); } // Function

    std::string_view GetCurrentTime()
    /*/////////////////////////////*/ {
        time_t now = time(0);
        char buffer[80];
        auto pTstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%X", pTstruct);
        return buffer;
    } // Function

    std::string_view GetCurrentDateTime()
    /*/////////////////////////////////*/ {
        time_t now = time(0);
        char buffer[80];
        auto pTstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%Y.%m.%d.%H.%M.%S", pTstruct);
        return buffer;
    } // Function

    const int Hash(const std::string_view &text)
    /*////////////////////////////////////////*/ {
        return std::hash<std::string_view>{}(text);
    } // Function

    Point2 GetMousePositionPx()
    /*///////////////////////*/ {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    } // Function

    Point2F GetMousePositionF()
    /*///////////////////////*/ {
        auto canvasSize = GetCanvasSize();
        auto mousePosPx = GetMousePositionPx();
        auto x = static_cast<float>(mousePosPx.x) / canvasSize.width;
        auto y = static_cast<float>(mousePosPx.y) / canvasSize.height;
        return {x, y};
    } // Function

    class KeyboardInput::Pimpl
    /*//////////////////////*/ {
      public:
        std::string textInput;
        std::set<SDL_Keycode> pressedKeys;
        std::set<SDL_Keycode> firedKeys;
    }; // Class

    KeyboardInput::KeyboardInput()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void KeyboardInput::KeyDown(SDL_Keycode key)
    /*////////////////////////////////////////*/ {
        if (p->pressedKeys.count(key) == 0) p->firedKeys.insert(key);
        p->pressedKeys.insert(key);
    } // Function

    void KeyboardInput::KeyUp(SDL_Keycode key)
    /*//////////////////////////////////////*/ { p->pressedKeys.erase(key); } // Function

    bool KeyboardInput::AnyKeyHasBeenFired() const
    /*//////////////////////////////////////////*/ { return p->firedKeys.size() > 0; } // Function

    bool KeyboardInput::KeyIsPressed(SDL_Keycode key) const
    /*///////////////////////////////////////////////////*/ {
        return p->pressedKeys.count(key) > 0;
    } // Function

    bool KeyboardInput::KeyHasBeenFiredPickResult(SDL_Keycode key)
    /*//////////////////////////////////////////////////////////*/ {
        auto result = p->firedKeys.count(key) > 0;
        p->firedKeys.erase(key);
        return result;
    } // Function

    std::string_view KeyboardInput::PickTextInput()
    /*///////////////////////////////////////////*/ {
        auto result = p->textInput;
        p->textInput = "";
        return result;
    } // Function

    void KeyboardInput::ResetTextInput()
    /*////////////////////////////////*/ { p->textInput = ""; } // Function

    void KeyboardInput::AppendTextInput(std::string_view toAppend)
    /*//////////////////////////////////////////////////////////*/ {
        p->textInput.append(toAppend);
    } // Function

    class MouseButton::Pimpl
    /*////////////////////*/ {
      public:
        static constexpr int defaultClickSpeed = 200;
        bool isPressed = false;
        Uint64 ticksButtonDown = 0;
        int clickDuration = 0;
        bool hasBeenFired = false;
        bool hasBeenReleased = false;
        MouseActionManager actionMngr;
    }; // Class

    MouseButton::MouseButton()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void MouseButton::Reset()
    /*/////////////////////*/ { p->isPressed = false; } // Function

    void MouseButton::PressDown()
    /*/////////////////////////*/ {
        p->isPressed = true;
        p->hasBeenFired = true;
        p->hasBeenReleased = false;
        p->clickDuration = 0;
        p->ticksButtonDown = SDL_GetTicks();
    } // Function

    void MouseButton::Release()
    /*///////////////////////*/ {
        p->isPressed = false;
        p->hasBeenReleased = true;
        p->clickDuration = SDL_GetTicks() - p->ticksButtonDown;
    } // Function

    int MouseButton::GetPressDuration() const
    /*/////////////////////////////////////*/ {
        return SDL_GetTicks() - p->ticksButtonDown;
    } // Function

    int MouseButton::ClickDurationPickResult()
    /*//////////////////////////////////////*/ {
        auto result = p->clickDuration;
        p->clickDuration = Pimpl::defaultClickSpeed;
        return result;
    } // Function

    int MouseButton::ClickDurationPeekResult() const
    /*////////////////////////////////////////////*/ { return p->clickDuration; } // Function

    void MouseButton::PerformMouseAction()
    /*//////////////////////////////////*/ {
        Log();
        if (p->hasBeenFired)
            p->actionMngr.PerformFiredActions(p->ticksButtonDown, p->isPressed, p->hasBeenFired);
        else
            p->actionMngr.ClearFiredActions();
        Log();
        if (p->hasBeenReleased)
            p->actionMngr.PerformReleasedActions(p->ticksButtonDown);
        else
            p->actionMngr.ClearReleasedActions();
    } // Function

    void MouseButton::AddFiredAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/ {
        if (p->hasBeenFired == true) return;
        auto id = Hash(uniqueName);
        if (p->actionMngr.FiredActionsContains(id)) return;
        p->actionMngr.AddFiredAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    } // Function

    void MouseButton::AddReleasedAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/ {
        if (p->hasBeenReleased == true) return;
        auto id = Hash(uniqueName);
        if (p->actionMngr.ReleasedActionsContains(id)) return;
        p->actionMngr.AddReleasedAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    } // Function

    void MouseButton::ResetActions()
    /*////////////////////////////*/ {
        p->actionMngr.ClearFiredActions();
        p->hasBeenFired = false;
        p->actionMngr.ClearReleasedActions();
        p->hasBeenReleased = false;
    } // Function

    const int MouseButton::GetDefaultClickSpeed()
    /*/////////////////////////////////////////*/ { return Pimpl::defaultClickSpeed; } // Function

    class MouseInput::Pimpl
    /*///////////////////*/ {
      public:
        Point2 motionDelta = {0, 0};
        int mouseWheelDelta = 0;
        MouseButton leftButton;
        MouseButton middleButton;
        MouseButton rightButton;
    }; // Class

    MouseInput::MouseInput()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void MouseInput::Reset()
    /*////////////////////*/ {
        p->leftButton.Reset();
        p->middleButton.Reset();
        p->rightButton.Reset();
        p->mouseWheelDelta = 0;
        p->motionDelta = {0, 0};
        ResetMouseActions();
    } // Function

    void MouseInput::MouseDown(Uint8 mouseButton)
    /*/////////////////////////////////////////*/ {
        switch (mouseButton)
        /******************/ {
        case SDL_BUTTON_LEFT:
            p->leftButton.PressDown();
            break;
        case SDL_BUTTON_MIDDLE:
            p->middleButton.PressDown();
            break;
        case SDL_BUTTON_RIGHT:
            p->rightButton.PressDown();
            break;
        }
    } // Function

    void MouseInput::MouseUp(Uint8 mouseButton)
    /*///////////////////////////////////////*/ {
        switch (mouseButton)
        /******************/ {
        case SDL_BUTTON_LEFT:
            p->leftButton.Release();
            break;
        case SDL_BUTTON_MIDDLE:
            p->middleButton.Release();
            break;
        case SDL_BUTTON_RIGHT:
            p->rightButton.Release();
            break;
        }
    } // Function

    void MouseInput::MouseWheel(int deltaScroll)
    /*////////////////////////////////////////*/ { p->mouseWheelDelta += deltaScroll; } // Function

    void MouseInput::MouseMove(Point2 motionDelta_)
    /*///////////////////////////////////////////*/ { p->motionDelta = motionDelta_; } // Function

    int MouseInput::MouseWheelDeltaPickResult()
    /*///////////////////////////////////////*/ {
        auto resultMouseWheelDelta = p->mouseWheelDelta;
        p->mouseWheelDelta = 0;
        return resultMouseWheelDelta;
    } // Function

    void MouseInput::PerformMouseActions()
    /*//////////////////////////////////*/ {
        Log();
        p->leftButton.PerformMouseAction();
        Log();
        p->middleButton.PerformMouseAction();
        Log();
        p->rightButton.PerformMouseAction();
    } // Function

    void MouseInput::ResetMouseActions()
    /*////////////////////////////////*/ {
        p->leftButton.ResetActions();
        p->middleButton.ResetActions();
        p->rightButton.ResetActions();
    } // Function

    Point2 MouseInput::GetMotionDeltaPickResults()
    /*//////////////////////////////////////////*/ {
        auto result = p->motionDelta;
        p->motionDelta = {0, 0};
        return result;
    } // Function

    MouseButton &MouseInput::GetLeftButton()
    /*////////////////////////////////////*/ { return p->leftButton; } // Function

    MouseButton &MouseInput::GetMiddleButton()
    /*//////////////////////////////////////*/ { return p->middleButton; } // Function

    MouseButton &MouseInput::GetRightButton()
    /*/////////////////////////////////////*/ { return p->rightButton; } // Function

    class MouseActionManager::Pimpl
    /*///////////////////////////*/ {
      public:
        std::map<int, MouseAction> firedActions;
        std::map<int, MouseAction> releasedActions;
    }; // Class

    MouseActionManager::MouseActionManager()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////*/ {} // Function

    void
    MouseActionManager::PerformFiredActions(int ticksButtonDown, bool isPressed, bool &hasBeenFired)
    /*////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::function<void()> firedAction;
        float firedPriority = -1;
        Uint64 ticksPerform = 0;
        int key = 0;
        bool highestPrioNotReady = false;
        std::set<int> firedToDelete;
        Log();
        for (auto it = p->firedActions.begin(); it != p->firedActions.end();)
        /*******************************************************************/ {
            if (it->second.priority > firedPriority)
            /**************************************/ {
                firedPriority = it->second.priority;
                firedAction = it->second.action;
                ticksPerform = ticksButtonDown + it->second.delay;
                key = it->first;
                auto diff = (int)(ticksPerform - SDL_GetTicks());
                if (diff > 0)
                /***********/ {
                    highestPrioNotReady = true;
                    it++;
                    continue;
                }
            }
            if (false == it->second.ensureIsExec) firedToDelete.insert(it->first);
            it++;
        }
        Log();
        if (highestPrioNotReady)
        /**********************/ {
            int prevPriority = -1;
            for (auto it = p->firedActions.begin(); it != p->firedActions.end();)
            /*******************************************************************/ {
                if (it->second.priority < firedPriority && it->second.priority > prevPriority)
                /****************************************************************************/ {
                    key = it->first;
                    firedAction = it->second.action;
                    prevPriority = ticksButtonDown + it->second.delay;
                    ticksPerform = ticksButtonDown + it->second.delay;
                    firedPriority = it->second.priority;
                }
                it++;
            }
        }
        Log();
        if (SDL_GetTicks() >= ticksPerform)
        /*********************************/ {
            if (firedAction)
            /**************/ {
                if (isPressed) firedAction();
                p->firedActions.erase(key);
            }
        }
        Log();
        for (auto key : firedToDelete)
            p->firedActions.erase(key);
        Log();
        if (p->firedActions.size() == 0 && isPressed == false) hasBeenFired = false;
    } // Function

    void MouseActionManager::PerformReleasedActions(int ticksButtonDown)
    /*////////////////////////////////////////////////////////////////*/ {
        std::function<void()> releasedAction;
        float releasedPriority = -1;
        Uint64 ticksPerform = 0;
        int key = 0;
        bool highestPrioNotReady = false;
        std::set<int> releasedToDelete;
        Log();
        for (auto it = p->releasedActions.begin(); it != p->releasedActions.end();)
        /*************************************************************************/ {
            Log();
            if (it->second.priority > releasedPriority)
            /*****************************************/ {
                releasedPriority = it->second.priority;
                releasedAction = it->second.action;
                ticksPerform = ticksButtonDown + it->second.delay;
                key = it->first;
                auto diff = (int)(ticksPerform - SDL_GetTicks());
                Log();
                if (diff > 0)
                /***********/ {
                    highestPrioNotReady = true;
                    it++;
                    continue;
                }
            }
            Log();
            if (false == it->second.ensureIsExec) releasedToDelete.insert(it->first);
            it++;
        }
        Log();
        if (highestPrioNotReady)
        /**********************/ {
            int prevPriority = -1;
            for (auto it = p->releasedActions.begin(); it != p->releasedActions.end();)
            /*************************************************************************/ {
                if (it->second.priority < releasedPriority && it->second.priority > prevPriority)
                /*******************************************************************************/ {
                    key = it->first;
                    releasedAction = it->second.action;
                    prevPriority = ticksButtonDown + it->second.delay;
                    ticksPerform = ticksButtonDown + it->second.delay;
                    releasedPriority = it->second.priority;
                }
                it++;
            }
        }
        Log();
        if (SDL_GetTicks() >= ticksPerform)
        /*********************************/ {
            Log();
            if (releasedAction)
            /*****************/ {
                Log();
                releasedAction();
                Log();
                p->releasedActions.erase(key);
            }
        }
        Log();
        for (auto key : releasedToDelete)
            p->releasedActions.erase(key);
    } // Function

    void MouseActionManager::ClearFiredActions()
    /*////////////////////////////////////////*/ { p->firedActions.clear(); } // Function

    void MouseActionManager::ClearReleasedActions()
    /*///////////////////////////////////////////*/ { p->releasedActions.clear(); } // Function

    bool MouseActionManager::FiredActionsContains(int actionId)
    /*///////////////////////////////////////////////////////*/ {
        return p->firedActions.count(actionId);
    } // Function

    bool MouseActionManager::ReleasedActionsContains(int actionId)
    /*//////////////////////////////////////////////////////////*/ {
        return p->releasedActions.count(actionId);
    } // Function

    void MouseActionManager::AddFiredAction(int actionId, MouseAction action)
    /*/////////////////////////////////////////////////////////////////////*/ {
        p->firedActions.insert({actionId, action});
    } // Function

    void MouseActionManager::AddReleasedAction(int actionId, MouseAction action)
    /*////////////////////////////////////////////////////////////////////////*/ {
        p->releasedActions.insert({actionId, action});
    } // Function
}
//////////////////////////////////////////////////////////////////////