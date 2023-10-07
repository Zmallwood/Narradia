{"CreateWoodWallE", InteractionMenuEntry(
                        "Create wood wall (east)", [=, this](Object *&object)
                        /************************/
                        {
                            auto map_area =
                                World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                            auto woodWall = std::make_shared<Object>("ObjectWoodWallE");
                            woodWall->SetModelScaling(1.0f);
                            map_area->GetTile(p->clicked_tile)->AddObject(woodWall);
                        })},