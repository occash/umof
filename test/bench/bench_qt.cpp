#include "qt_test.h"

#include <celero/Celero.h>

#include <QMetaObject>
#include <QMetaMethod>

class QtFixture : public celero::TestFixture
{
public:
    virtual void setUp(int64_t experimentValue)
    {
        const QMetaObject *meta = t.metaObject();
        m = meta->method(meta->indexOfMethod("func(int,int)"));
    }

    virtual void tearDown()
    {
    }

public:
    Test t;
    QMetaMethod m;

};

BENCHMARK_F(FuncDefs, Qt, QtFixture, 100, 100000)
{
    int res;
    celero::DoNotOptimizeAway(m.invoke(&t, Q_RETURN_ARG(int, res), Q_ARG(int, 1), Q_ARG(int, 2)));
}
