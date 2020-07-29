#include "exponentialgenerator.h"
#include <QDebug>
constexpr const qreal ExponentialGenerator::lambda;

qreal ExponentialGenerator::generate()
{
    return distribution(rndEngine);
}
