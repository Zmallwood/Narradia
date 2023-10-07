{"CreateWoodWallN", InteractionMenuEntry(
                        "Create wood wall (north)", [=, this](Object *&object)
                        /************************/
                        {
                            auto map_area =
                                World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                            auto woodWall = std::make_shared<Object>("ObjectWoodWallN");
                            woodWall->SetModelScaling(1.0f);
                            map_area->GetTile(p->clicked_tile_)->AddObject(woodWall);
                        })},