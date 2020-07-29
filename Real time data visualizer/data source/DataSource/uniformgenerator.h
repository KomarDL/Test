#ifndef UNIFORMGENERATOR_H
#define UNIFORMGENERATOR_H

#include "abstractgenerator.h"
#include <QMetaType>

class UniformGenerator : public AbstractGenerator
{
public:
    UniformGenerator() : AbstractGenerator("равномерное") {};
    virtual ~UniformGenerator() = default;
    UniformGenerator(const UniformGenerator&) = default;
    UniformGenerator &operator=(const UniformGenerator&) = default;

    qreal generate() override;
};

Q_DECLARE_METATYPE(UniformGenerator*)

#endif // UNIFORMGENERATOR_H
