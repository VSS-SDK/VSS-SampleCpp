//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_ISTATERECEIVERADAPTER_H
#define VSS_SAMPLE_ISTATERECEIVERADAPTER_H

#include <Domain/State.h>
#include <Domain/FieldTransformationType.h>

class IStateReceiverAdapter {
public:
    virtual vss::State receive(vss::FieldTransformationType) = 0;
};

#endif //VSS_SAMPLE_ISTATERECEIVERADAPTER_H
