{"CutGrass", InteractionMenuEntry(
                 "Cut grass", [=, this](Object *&object)
                 /************************/
                 {
                     if (object->GetObjectType() == Hash("ObjectTallGrass6"))
                     /******************************************************/
                     {
                         Player::Get()->data.stats.stam = Player::Get()->data.stats.stam - 2;
                         TextOutBox::Get()->Print("You cut some grass.");
                         RemoveObjectMirror(object);
                     }
                 })},