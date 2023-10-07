
{"Forage", InteractionMenuEntry(
               "Forage", [=, this](Object *&object)
               /************************/
               {
                   auto playerPosition = Player::Get()->GetPosition().ToIntPoint();
                   auto map_area =
                       World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                   if (map_area->GetTile(p->clicked_tile)->GetGroundType() == Hash("GroundGrass"))
                   /*****************************************************************************/
                   {
                       TextOutBox::Get()->Print("You search the area for foragble items.");
                       auto dice = rand() % 10;
                       if (dice < 2)
                       /***********/
                       {
                           Player::Get()->data.inventory.Add("ObjectBlueberry");
                           TextOutBox::Get()->Print("You found some blueberries.");
                       }
                       else if (dice < 3)
                       /******************/
                       {
                           Player::Get()->data.inventory.Add("ObjectStrawberry");
                           TextOutBox::Get()->Print("You found some strawberries.");
                       }
                       else if (dice < 4)
                       /******************/
                       {
                           Player::Get()->data.inventory.Add("ObjectHazelnut");
                           TextOutBox::Get()->Print("You found some hazelnuts.");
                       }
                       else if (dice < 5)
                       /******************/
                       {
                           Player::Get()->data.inventory.Add("ObjectWalnut");
                           TextOutBox::Get()->Print("You found some walnuts.");
                       }
                       else
                       /**/
                       {
                           TextOutBox::Get()->Print("You found nothing.");
                       }
                   }
               })},