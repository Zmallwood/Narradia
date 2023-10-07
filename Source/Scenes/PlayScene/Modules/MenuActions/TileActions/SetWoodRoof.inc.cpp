{"SetWoodRoof", InteractionMenuEntry(
                    "Set wood roof", [=, this](Object *&object)
                    /*****************************************/
                    {
                        auto playerPosition = Player::Get()->GetPosition().ToIntPoint();
                        auto map_area =
                            World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                        TextOutBox::Get()->Print("Lay set some wood roof.");
                        map_area->GetTile(p->clicked_tile)->SetRoofType(Hash("RoofWood"));
                    })},