{"SplitStack", InteractionMenuEntry(
                   "Split", [=, this](Object *&object)
                   /************************/
                   {
                       auto sceneGui = PlayScene::Get()->GetSceneGui();
                       auto gui_window_split = std::make_shared<SplitStackGui>(object);
                       sceneGui->AddGuiComponent(gui_window_split);
                   })},