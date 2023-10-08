{"Fish", InteractionMenuEntry(
             "Fish", [=, this](Object *&object)
             /************************/
             {
                 auto map_area = World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                 if (map_area->GetTile(p->clicked_tile_)->GetGroundType() == Hash("GroundWater"))
                 /*****************************************************************************/
                 {
                     TextOutBox::Get()->Print("You caught some fish.");
                     for (auto i = 0; i < 100; i++)
                     /****************************/
                     {
                         if (Player::Get()->data.inventory_.objects_.count(i) == 0)
                         /***************************************************/
                         {
                             Player::Get()->data.inventory_.objects_.insert(
                                 {i, std::make_shared<Object>(Hash("ObjectFish"))});
                             break;
                         }
                     }
                 }
             })},