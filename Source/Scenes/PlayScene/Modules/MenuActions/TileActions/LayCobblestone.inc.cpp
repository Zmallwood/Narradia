{"LayCobblestone", InteractionMenuEntry("Lay cobblestone", [=, this](Object *&object) {
     auto map_area = World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
     TextOutBox::Get()->Print("Lay lay some cobblestone.");
     map_area->GetTile(p->clicked_tile_)->SetGroundType(Hash("GroundCobblestone"));
 })},