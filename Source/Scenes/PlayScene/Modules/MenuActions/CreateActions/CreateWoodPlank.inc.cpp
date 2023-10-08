{"CreateWoodPlank", InteractionMenuEntry(
                        "Create wood plank", [=, this](Object *&object)
                        /************************/
                        {
                            if (object->GetObjectType() == Hash("ObjectWoodLog"))
                            /***************************************************/
                            {
                                Player::Get()->data.status_.stamina = Player::Get()->data.status_.stamina - 2;
                                TextOutBox::Get()->Print("You create a wood plank.");
                                auto wood_plank = std::make_shared<Object>("ObjectWoodPlank");
                                wood_plank->SetModelScaling(1.0f);
                                wood_plank->SetModelRotation(rand() % 360);
                                object->GetParentObjectsCollection()->Add(wood_plank);
                                RemoveObjectMirror(object);
                            }
                        })},