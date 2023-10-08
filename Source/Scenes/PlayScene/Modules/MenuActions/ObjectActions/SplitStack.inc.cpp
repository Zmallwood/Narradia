{"SplitStack", InteractionMenuEntry("Split", [=, this](Object *&object) {
     auto scene_gui = PlayScene::Get()->GetSceneGui();
     auto gui_window_split = std::make_shared<SplitStackGui>(object);
     scene_gui->AddGuiComponent(gui_window_split);
 })},