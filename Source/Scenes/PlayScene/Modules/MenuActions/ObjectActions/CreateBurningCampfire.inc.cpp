{"CreateBurningCampfire", InteractionMenuEntry("Light campfire", [=, this](Object *&object) {
     if (object->GetObjectType() == Hash("ObjectKindling")) {
         Player::Get()->data_.status_.stamina = Player::Get()->data_.status_.stamina - 2;
         TextOutBox::Get()->Print("You light the campfire.");
         auto campfire = std::make_shared<Object>("ObjectBurningCampfire");
         object->GetParentObjectsCollection()->Add(campfire);
         if (object->GetWorkProgress() >= 1.0f)
             RemoveObjectMirror(object);
     }
 })},