{"LayCobblestone",
 InteractionMenuEntry(
     "Lay cobblestone",
     [=, this](Object *&object)
     /************************/
     {
         auto playerPosition = Player::Get().GetPosition().ToIntPoint();
         auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
         TextOutBox::Get().Print("Lay lay some cobblestone.");
         map_area->GetTile(p->clicked_tile)->SetGroundType(Hash("GroundCobblestone"));
     })},
    {"LayWoodFloor",
     InteractionMenuEntry(
         "Lay wood floor", [=, this](Object *&object)
         /************************/
         {
             auto playerPosition = Player::Get().GetPosition().ToIntPoint();
             auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
             TextOutBox::Get().Print("Lay lay some wood floor.");
             map_area->GetTile(p->clicked_tile)->SetGroundType(Hash("GroundWoodFloor"));
         })},