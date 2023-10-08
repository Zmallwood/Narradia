{"CutGrass", InteractionMenuEntry(
                 "Cut grass", [=, this](Object *&object)
                 /************************/
                 {
                     if (object->GetObjectType() == Hash("ObjectTallGrass6"))
                     /******************************************************/
                     {
                         Player::Get()->data.status_.stamina = Player::Get()->data.status_.stamina - 2;
                         TextOutBox::Get()->Print("You cut some grass.");
                         RemoveObjectMirror(object);
                     }
                 })},