{"CreateStoneAxe",
 InteractionMenuEntry(
     "Create stone axe",
     [=, this](Object *&object)
     /************************/
     {
         if (object->GetObjectType() == Hash("ObjectSmallStone") ||
             object->GetObjectType() == Hash("ObjectBranch"))
         /*********************************************************/
         {
             if (Player::Get().data.inventory.ContainsObjectOfType("ObjectSmallStone") &&
                 Player::Get().data.inventory.ContainsObjectOfType("ObjectBranch"))
             /************************************************************************/
             {
                 TextOutBox::Get().Print("You create a stone axe.");
                 Player::Get().data.inventory.Add("ObjectStoneAxe");
                 if (object->GetObjectType() == Hash("ObjectSmallStone"))
                     Player::Get().data.inventory.RemoveObjectOfType("ObjectBranch");
                 else if (object->GetObjectType() == Hash("ObjectBranch"))
                     Player::Get().data.inventory.RemoveObjectOfType("ObjectSmallStone");
                 RemoveObjectMirror(object);
             }
             else
             /**/
             {
                 TextOutBox::Get().Print("You are missing materials to create a stone axe.");
             }
         }
     })},
    {"CreateWoodenBox", InteractionMenuEntry(
                            "Create wooden box",
                            [=, this](Object *&object)
                            /************************/
                            {
                                if (object->GetObjectType() == Hash("ObjectWoodPlank"))
                                /*****************************************************/
                                {
                                    Player::Get().data.stats.stam =
                                        Player::Get().data.stats.stam - 2;
                                    TextOutBox::Get().Print("You create a wooden box.");
                                    auto wooden_box = std::make_shared<Object>("ObjectWoodenBox");
                                    wooden_box->SetModelScaling(1.0f);
                                    wooden_box->SetModelRotation(rand() % 360);
                                    object->GetParentObjectsCollection()->Add(wooden_box);
                                    RemoveObjectMirror(object);
                                }
                            })},
    {"CreateWoodLog", InteractionMenuEntry(
                          "Create wood log",
                          [=, this](Object *&object)
                          /************************/
                          {
                              if (object->GetObjectType() == Hash("ObjectFelledTree"))
                              /******************************************************/
                              {
                                  Player::Get().data.stats.stam = Player::Get().data.stats.stam - 2;
                                  object->AddToWorkProgress(0.34f);
                                  auto percent = (int)(object->GetWorkProgress() * 100);
                                  TextOutBox::Get().Print(
                                      "You are chopping up the felled tree, progress: " +
                                      std::to_string(percent) + "%");
                                  auto wood_log = std::make_shared<Object>("ObjectWoodLog");
                                  wood_log->SetModelScaling(1.0f);
                                  wood_log->SetModelRotation(rand() % 360);
                                  object->GetParentObjectsCollection()->Add(wood_log);
                                  if (object->GetWorkProgress() >= 1.0f)
                                      RemoveObjectMirror(object);
                              }
                          })},
    {"CreateWoodPlank", InteractionMenuEntry(
                            "Create wood plank",
                            [=, this](Object *&object)
                            /************************/
                            {
                                if (object->GetObjectType() == Hash("ObjectWoodLog"))
                                /***************************************************/
                                {
                                    Player::Get().data.stats.stam =
                                        Player::Get().data.stats.stam - 2;
                                    TextOutBox::Get().Print("You create a wood plank.");
                                    auto wood_plank = std::make_shared<Object>("ObjectWoodPlank");
                                    wood_plank->SetModelScaling(1.0f);
                                    wood_plank->SetModelRotation(rand() % 360);
                                    object->GetParentObjectsCollection()->Add(wood_plank);
                                    RemoveObjectMirror(object);
                                }
                            })},
    {"CreateWoodWallE",
     InteractionMenuEntry(
         "Create wood wall (east)",
         [=, this](Object *&object)
         /************************/
         {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             auto woodWall = std::make_shared<Object>("ObjectWoodWallE");
             woodWall->SetModelScaling(1.0f);
             map_area->GetTile(p->clicked_tile)->AddObject(woodWall);
         })},
    {"CreateWoodWallN",
     InteractionMenuEntry(
         "Create wood wall (north)",
         [=, this](Object *&object)
         /************************/
         {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             auto woodWall = std::make_shared<Object>("ObjectWoodWallN");
             woodWall->SetModelScaling(1.0f);
             map_area->GetTile(p->clicked_tile)->AddObject(woodWall);
         })},
    {"CreateWoodWallS",
     InteractionMenuEntry(
         "Create wood wall (south)",
         [=, this](Object *&object)
         /************************/
         {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             auto wood_wall = std::make_shared<Object>("ObjectWoodWallS");
             wood_wall->SetModelScaling(1.0f);
             map_area->GetTile(p->clicked_tile)->AddObject(wood_wall);
         })},
    {"CreateWoodWallW",
     InteractionMenuEntry(
         "Create wood wall (west)",
         [=, this](Object *&object)
         /************************/
         {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             auto wood_wall = std::make_shared<Object>("ObjectWoodWallW");
             wood_wall->SetModelScaling(1.0f);
             map_area->GetTile(p->clicked_tile)->AddObject(wood_wall);
         })},
    {"MineCaveEntrance",
     InteractionMenuEntry(
         "Mine cave entrance", [=, this](Object *&object)
         /**********************************************/
         {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             auto mine_entrance = std::make_shared<Object>("ObjectMineEntrance");
             mine_entrance->SetModelScaling(0.7f);
             map_area->GetTile(p->clicked_tile)->AddObject(mine_entrance);
             TextOutBox::Get().Print("You have created a mine entrance.");
         })},