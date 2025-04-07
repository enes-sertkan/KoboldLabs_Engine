#pragma once

#include "aTurretPart.h"
#include "sObject.h"

class aTurretNeck : public aTurretPart
{
public:
	Object* headConnection = nullptr;



    aTurretNeck* Clone() const override
    {
        aTurretNeck* clone = new aTurretNeck(*this);
        clone->object = nullptr;
        clone->factory = factory;
        clone->headConnection = nullptr;
        return clone;
    }

}; 