#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    void RemoveObjectMirror(Object *&);
    std::shared_ptr<Object> MoveObject(Object *);
}