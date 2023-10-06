{"ChopDownTree",
 InteractionMenuEntry(
     "Chop down tree", [=, this](Object *&object)
     /************************/
     {
         if (object)
         /*********/
         {
             if (object->GetObjectType() == Hash("ObjectTree1") ||
                 object->GetObjectType() == Hash("ObjectTree2"))
             /***************************************************/
             {
                 Player::Get().data.stats.stam = Player::Get().data.stats.stam - 2;
                 object->AddToWorkProgress(0.2f);
                 auto percent = (int)(object->GetWorkProgress() * 100);
                 TextOutBox::Get().Print(
                     "You are chopping the tree, progress: " + std::to_string(percent) + "%");
                 if (object->GetWorkProgress() >= 1.0f)
                 /************************************/
                 {
                     auto felled_tree = std::make_shared<Object>("ObjectFelledTree");
                     felled_tree->SetModelScaling(2.0f);
                     felled_tree->SetModelRotation(rand() % 360);
                     object->GetParentObjectsCollection()->Add(felled_tree);
                     RemoveObjectMirror(object);
                 }
             }
         }
         else
         /**/
         {
             TextOutBox::Get().Print("The tree does no longer exist.");
         }
     })},