{"MineCaveEntrance", InteractionMenuEntry(
                         "Mine cave entrance", [=, this](Object *&object)
                         /**********************************************/
                         {
                             auto map_area =
                                 World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                             auto mine_entrance = std::make_shared<Object>("ObjectMineEntrance");
                             mine_entrance->SetModelScaling(0.7f);
                             map_area->GetTile(p->clicked_tile)->AddObject(mine_entrance);
                             TextOutBox::Get()->Print("You have created a mine entrance.");
                         })},