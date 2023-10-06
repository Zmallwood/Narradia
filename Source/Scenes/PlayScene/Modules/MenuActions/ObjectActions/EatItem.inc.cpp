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
                 Player::Get().data.stats.stam =
                     std::min(Player::Get().data.stats.maxStam, Player::Get().data.stats.stam + 2);
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