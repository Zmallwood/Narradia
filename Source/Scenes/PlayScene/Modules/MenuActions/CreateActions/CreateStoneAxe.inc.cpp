{"CreateStoneAxe",
 InteractionMenuEntry(
     "Create stone axe", [=, this](Object *&object)
     /************************/
     {
         if (object->GetObjectType() == Hash("ObjectSmallStone") ||
             object->GetObjectType() == Hash("ObjectBranch"))
         /*********************************************************/
         {
             if (Player::Get()->data.inventory_.ContainsObjectOfType("ObjectSmallStone") &&
                 Player::Get()->data.inventory_.ContainsObjectOfType("ObjectBranch"))
             /************************************************************************/
             {
                 TextOutBox::Get()->Print("You create a stone axe.");
                 Player::Get()->data.inventory_.Add("ObjectStoneAxe");
                 if (object->GetObjectType() == Hash("ObjectSmallStone"))
                     Player::Get()->data.inventory_.RemoveObjectOfType("ObjectBranch");
                 else if (object->GetObjectType() == Hash("ObjectBranch"))
                     Player::Get()->data.inventory_.RemoveObjectOfType("ObjectSmallStone");
                 RemoveObjectMirror(object);
             }
             else
             /**/
             {
                 TextOutBox::Get()->Print("You are missing materials to create a stone axe.");
             }
         }
     })},