#include "abstractgenerator.h"

AbstractGenerator::AbstractGenerator(QString distName):
    distName(distName)
{
    std::random_device rd;
    rndEngine.seed(rd());
}
