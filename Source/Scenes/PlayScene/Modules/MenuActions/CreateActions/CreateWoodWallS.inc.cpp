{"CreateWoodWallS", InteractionMenuEntry(
                        "Create wood wall (south)", [=, this](Object *&object)
                        /************************/
                        {
                            auto map_area =
                                World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                            auto wood_wall = std::make_shared<Object>("ObjectWoodWallS");
                            wood_wall->SetModelScaling(1.0f);
                            map_area->GetTile(p->clicked_tile_)->AddObject(wood_wall);
                        })},