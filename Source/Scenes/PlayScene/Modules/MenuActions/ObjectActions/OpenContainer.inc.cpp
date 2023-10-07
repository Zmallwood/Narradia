{"OpenContainer", InteractionMenuEntry(
                      "Open", [=, this](Object *&object)
                      /************************/
                      {
                          auto scene_gui = PlayScene::Get()->GetSceneGui();
                          auto gui_window_object_container =
                              std::make_shared<OpenContainerGui>(object);
                          scene_gui->AddGuiComponent(gui_window_object_container);
                      })},