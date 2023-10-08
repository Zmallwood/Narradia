{"CreateWoodenBox", InteractionMenuEntry("Create wooden box", [=, this](Object *&object) {
     if (object->GetObjectType() == Hash("ObjectWoodPlank")) {
         Player::Get()->data_.status_.stamina = Player::Get()->data_.status_.stamina - 2;
         TextOutBox::Get()->Print("You create a wooden box.");
         auto wooden_box = std::make_shared<Object>("ObjectWoodenBox");
         wooden_box->SetModelScaling(1.0f);
         wooden_box->SetModelRotation(rand() % 360);
         object->GetParentObjectsCollection()->Add(wooden_box);
         RemoveObjectMirror(object);
     }
 })},