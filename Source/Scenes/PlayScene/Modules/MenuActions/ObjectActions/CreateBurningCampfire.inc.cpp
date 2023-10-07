{"CreateBurningCampfire",
 InteractionMenuEntry(
     "Light campfire", [=, this](Object *&object)
     /************************/
     {
         if (object->GetObjectType() == Hash("ObjectKindling"))
         /****************************************************/
         {
             Player::Get()->data.stats.stam = Player::Get()->data.stats.stam - 2;
             TextOutBox::Get()->Print("You light the campfire.");
             auto campfire = std::make_shared<Object>("ObjectBurningCampfire");
             object->GetParentObjectsCollection()->Add(campfire);
             if (object->GetWorkProgress() >= 1.0f)
                 RemoveObjectMirror(object);
         }
     })},