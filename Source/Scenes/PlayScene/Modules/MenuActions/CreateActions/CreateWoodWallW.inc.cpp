{"CreateWoodWallW", InteractionMenuEntry(
                        "Create wood wall (west)", [=, this](Object *&object)
                        /************************/
                        {
                            auto playerPosition = Player::Get().GetPosition().ToIntPoint();
                            auto map_area =
                                World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
                            auto wood_wall = std::make_shared<Object>("ObjectWoodWallW");
                            wood_wall->SetModelScaling(1.0f);
                            map_area->GetTile(p->clicked_tile)->AddObject(wood_wall);
                        })},