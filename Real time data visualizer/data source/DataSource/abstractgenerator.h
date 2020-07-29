#ifndef ABSTRACTGENERATOR_H
#define ABSTRACTGENERATOR_H

#include <QString>
#include <QMetaType>
#include <random>

class AbstractGenerator
{

public:
    AbstractGenerator(QString distName = "Абстрактый генератор");
    virtual ~AbstractGenerator() = default;
    AbstractGenerator(const AbstractGenerator&) = default;
    AbstractGenerator &operator=(const AbstractGenerator&) = default;

    //in types compatible with QVariant can't be pure virtual methods
    virtual qreal generate() { return 0; }
    QString distributionName() const { return distName; }

protected:
    std::default_random_engine  rndEngine;
private:
    QString distName;
};

Q_DECLARE_METATYPE(AbstractGenerator*)

#endif // ABSTRACTGENERATOR_H
