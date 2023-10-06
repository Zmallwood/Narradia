{"OpenContainer", InteractionMenuEntry(
                      "Open", [=, this](Object *&object)
                      /************************/
                      {
                          auto sceneGui = PlayScene::Get().GetSceneGui();
                          auto gui_window_object_container =
                              std::make_shared<OpenContainerGui>(object);
                          sceneGui->AddGuiComponent(gui_window_object_container);
                      })},