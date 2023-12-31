{"Forage", InteractionMenuEntry("Forage", [=, this](Object *&object) {
     auto map_area = World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
     if (map_area->GetTile(p->clicked_tile_)->GetGroundType() == Hash("GroundGrass")) {
         TextOutBox::Get()->Print("You search the area for foragble items.");
         auto dice = rand() % 10;
         if (dice < 2) {
             Player::Get()->data_.inventory_.Add("ObjectBlueberry");
             TextOutBox::Get()->Print("You found some blueberries.");
         }
         else if (dice < 3) {
             Player::Get()->data_.inventory_.Add("ObjectStrawberry");
             TextOutBox::Get()->Print("You found some strawberries.");
         }
         else if (dice < 4) {
             Player::Get()->data_.inventory_.Add("ObjectHazelnut");
             TextOutBox::Get()->Print("You found some hazelnuts.");
         }
         else if (dice < 5) {
             Player::Get()->data_.inventory_.Add("ObjectWalnut");
             TextOutBox::Get()->Print("You found some walnuts.");
         }
         else {
             TextOutBox::Get()->Print("You found nothing.");
         }
     }
 })},