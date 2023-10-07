{"LayWoodFloor", InteractionMenuEntry(
                     "Lay wood floor", [=, this](Object *&object)
                     /************************/
                     {
                         auto map_area =
                             World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                         TextOutBox::Get()->Print("Lay lay some wood floor.");
                         map_area->GetTile(p->clicked_tile)->SetGroundType(Hash("GroundWoodFloor"));
                     })},