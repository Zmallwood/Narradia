{"SetWoodRoof", InteractionMenuEntry("Set wood roof", [=, this](Object *&object) {
     auto map_area = World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
     TextOutBox::Get()->Print("Lay set some wood roof.");
     map_area->GetTile(p->clicked_tile_)->SetRoofType(Hash("RoofWood"));
 })},