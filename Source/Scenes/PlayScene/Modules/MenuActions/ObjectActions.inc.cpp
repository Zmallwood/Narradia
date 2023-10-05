{"ChopDownTree",
 InteractionMenuEntry(
     "Chop down tree",
     [=, this](Object *&object)
     /************************/ {
         if (object)
         /*********/ {
             if (object->GetObjectType() == Hash("ObjectTree1") ||
                 object->GetObjectType() == Hash("ObjectTree2"))
             /***************************************************/ {
                 Player::Get().data.stats.stam = Player::Get().data.stats.stam - 2;
                 object->AddToWorkProgress(0.2f);
                 auto percent = (int)(object->GetWorkProgress() * 100);
                 TextOutBox::Get().Print(
                     "You are chopping the tree, progress: " + std::to_string(percent) + "%");
                 if (object->GetWorkProgress() >= 1.0f)
                 /************************************/ {
                     auto felled_tree = std::make_shared<Object>("ObjectFelledTree");
                     felled_tree->SetModelScaling(2.0f);
                     felled_tree->SetModelRotation(rand() % 360);
                     object->GetParentObjectsCollection()->Add(felled_tree);
                     RemoveObjectMirror(object);
                 }
             }
         } else
         /****/ {
             TextOutBox::Get().Print("The tree does no longer exist.");
         }
     })},
    {"CutGrass", InteractionMenuEntry(
                     "Cut grass",
                     [=, this](Object *&object)
                     /************************/ {
                         if (object->GetObjectType() == Hash("ObjectTallGrass6"))
                         /******************************************************/ {
                             Player::Get().data.stats.stam = Player::Get().data.stats.stam - 2;
                             TextOutBox::Get().Print("You cut some grass.");
                             RemoveObjectMirror(object);
                         }
                     })},
    {"EatItem", InteractionMenuEntry(
                    "Eat item",
                    [=, this](Object *&object)
                    /************************/ {
                        if (object)
                        /*********/ {
                            if (object->GetObjectType() == Hash("ObjectRedApple") ||
                                object->GetObjectType() == Hash("ObjectCookedFi.hpp"))
                            /********************************************************/ {
                                Player::Get().data.stats.stam = std::min(
                                    Player::Get().data.stats.maxStam,
                                    Player::Get().data.stats.stam + 2);
                                object->DecreaseQuantity();
                                if (object->GetQuantity() == 0)
                                /*****************************/ {
                                    RemoveObjectMirror(object);
                                    object = nullptr;
                                }
                            }
                        }
                    })},
    {"CreateBurningCampfire",
     InteractionMenuEntry(
         "Light campfire",
         [=, this](Object *&object)
         /************************/ {
             if (object->GetObjectType() == Hash("ObjectKindling"))
             /****************************************************/ {
                 Player::Get().data.stats.stam = Player::Get().data.stats.stam - 2;
                 TextOutBox::Get().Print("You light the campfire.");
                 auto campfire = std::make_shared<Object>("ObjectBurningCampfire");
                 object->GetParentObjectsCollection()->Add(campfire);
                 if (object->GetWorkProgress() >= 1.0f) RemoveObjectMirror(object);
             }
         })},
    {"OpenContainer", InteractionMenuEntry(
                          "Open",
                          [=, this](Object *&object)
                          /************************/ {
                              auto sceneGui = PlayScene::Get().GetSceneGui();
                              auto gui_window_object_container =
                                  std::make_shared<OpenContainerGui>(object);
                              sceneGui->AddGuiComponent(gui_window_object_container);
                          })},
    {"SplitStack", InteractionMenuEntry(
                       "Split", [=, this](Object *&object)
                       /************************/ {
                           auto sceneGui = PlayScene::Get().GetSceneGui();
                           auto gui_window_split = std::make_shared<SplitStackGui>(object);
                           sceneGui->AddGuiComponent(gui_window_split);
                       })},