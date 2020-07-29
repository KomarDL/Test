#include "uniformgenerator.h"
#include <QRandomGenerator>

qreal UniformGenerator::generate()
{
    return QRandomGenerator::global()->generateDouble();
}
