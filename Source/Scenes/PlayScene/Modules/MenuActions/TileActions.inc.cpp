{"Fish", InteractionMenuEntry(
             "Fish",
             [=, this](Object *&object)
             /************************/ {
                 auto playerPosition = Player::Get().GetPosition().ToIntPoint();
                 auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
                 if (map_area->GetTile(p->clicked_tile)->GetGroundType() == Hash("GroundWater"))
                 /*****************************************************************************/ {
                     TextOutBox::Get().Print("You caught some fish.");
                     for (auto i = 0; i < 100; i++)
                     /****************************/ {
                         if (Player::Get().data.inventory.objects.count(i) == 0)
                         /***************************************************/ {
                             Player::Get().data.inventory.objects.insert(
                                 {i, std::make_shared<Object>(Hash("ObjectFish"))});
                             break;
                         }
                     }
                 }
             })},
    {"Forage",
     InteractionMenuEntry(
         "Forage",
         [=, this](Object *&object)
         /************************/ {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             if (map_area->GetTile(p->clicked_tile)->GetGroundType() == Hash("GroundGrass"))
             /*****************************************************************************/ {
                 TextOutBox::Get().Print("You search the area for foragble items.");
                 auto dice = rand() % 10;
                 if (dice < 2)
                 /***********/ {
                     Player::Get().data.inventory.Add("ObjectBlueberry");
                     TextOutBox::Get().Print("You found some blueberries.");
                 } else if (dice < 3)
                 /******************/ {
                     Player::Get().data.inventory.Add("ObjectStrawberry");
                     TextOutBox::Get().Print("You found some strawberries.");
                 } else if (dice < 4)
                 /******************/ {
                     Player::Get().data.inventory.Add("ObjectHazelnut");
                     TextOutBox::Get().Print("You found some hazelnuts.");
                 } else if (dice < 5)
                 /******************/ {
                     Player::Get().data.inventory.Add("ObjectWalnut");
                     TextOutBox::Get().Print("You found some walnuts.");
                 } else
                 /****/ {
                     TextOutBox::Get().Print("You found nothing.");
                 }
             }
         })},
    {"LayCobblestone",
     InteractionMenuEntry(
         "Lay cobblestone",
         [=, this](Object *&object)
         /************************/ {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             TextOutBox::Get().Print("Lay lay some cobblestone.");
             map_area->GetTile(p->clicked_tile)->SetGroundType(Hash("GroundCobblestone"));
         })},
    {"LayWoodFloor",
     InteractionMenuEntry(
         "Lay wood floor",
         [=, this](Object *&object)
         /************************/ {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             TextOutBox::Get().Print("Lay lay some wood floor.");
             map_area->GetTile(p->clicked_tile)->SetGroundType(Hash("GroundWoodFloor"));
         })},
    {"SetWoodRoof", InteractionMenuEntry(
                        "Set wood roof", [=, this](Object *&object)
                        /*****************************************/ {
                            auto playerPosition = Player::Get().GetPosition().ToIntPoint();
                            auto map_area =
                                World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
                            TextOutBox::Get().Print("Lay set some wood roof.");
                            map_area->GetTile(p->clicked_tile)->SetRoofType(Hash("RoofWood"));
                        })},