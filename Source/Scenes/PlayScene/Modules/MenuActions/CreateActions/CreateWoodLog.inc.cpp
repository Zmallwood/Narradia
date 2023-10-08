{"CreateWoodLog", InteractionMenuEntry("Create wood log", [=, this](Object *&object) {
     if (object->GetObjectType() == Hash("ObjectFelledTree")) {
         Player::Get()->data_.status_.stamina = Player::Get()->data_.status_.stamina - 2;
         object->AddToWorkProgress(0.34f);
         auto percent = (int)(object->GetWorkProgress() * 100);
         TextOutBox::Get()->Print(
             "You are chopping up the felled tree, progress: " + std::to_string(percent) + "%");
         auto wood_log = std::make_shared<Object>("ObjectWoodLog");
         wood_log->SetModelScaling(1.0f);
         wood_log->SetModelRotation(rand() % 360);
         object->GetParentObjectsCollection()->Add(wood_log);
         if (object->GetWorkProgress() >= 1.0f)
             RemoveObjectMirror(object);
     }
 })},