#ifndef EXPONENTIALGENERATOR_H
#define EXPONENTIALGENERATOR_H

#include "abstractgenerator.h"
#include <QMetaType>

class ExponentialGenerator : public AbstractGenerator
{
public:
    ExponentialGenerator() : AbstractGenerator("экспоненциальное") {};
    virtual ~ExponentialGenerator() = default;
    ExponentialGenerator(const ExponentialGenerator&) = default;
    ExponentialGenerator &operator=(const ExponentialGenerator&) = default;

    qreal generate() override;

private:
    static constexpr qreal lambda = 5.;
    std::exponential_distribution<qreal> distribution{lambda};
};

Q_DECLARE_METATYPE(ExponentialGenerator*)

#endif // EXPONENTIALGENERATOR_H
