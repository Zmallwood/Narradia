{"EatItem",
 InteractionMenuEntry(
     "Eat item", [=, this](Object *&object)
     /************************/
     {
         if (object)
         /*********/
         {
             if (object->GetObjectType() == Hash("ObjectRedApple") ||
                 object->GetObjectType() == Hash("ObjectCookedFi.hpp"))
             /********************************************************/
             {
                 Player::Get()->data_.status_.stamina =
                     std::min(Player::Get()->data_.status_.max_stamina, Player::Get()->data_.status_.stamina + 2);
                 object->DecreaseQuantity();
                 if (object->GetQuantity() == 0)
                 /*****************************/
                 {
                     RemoveObjectMirror(object);
                     object = nullptr;
                 }
             }
         }
     })},