{"CreateWoodenBox", InteractionMenuEntry(
                        "Create wooden box", [=, this](Object *&object)
                        /************************/
                        {
                            if (object->GetObjectType() == Hash("ObjectWoodPlank"))
                            /*****************************************************/
                            {
                                Player::Get()->data.stats.stam = Player::Get()->data.stats.stam - 2;
                                TextOutBox::Get()->Print("You create a wooden box.");
                                auto wooden_box = std::make_shared<Object>("ObjectWoodenBox");
                                wooden_box->SetModelScaling(1.0f);
                                wooden_box->SetModelRotation(rand() % 360);
                                object->GetParentObjectsCollection()->Add(wooden_box);
                                RemoveObjectMirror(object);
                            }
                        })},