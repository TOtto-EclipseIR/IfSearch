#include <eirType/CircularQueue.h>

void eirTypeTest::testCircularQueue(void)
{
    CircularQueue<int> i_cirq;
    QVERIFY(i_cirq.isNull());
    i_cirq.fill(123);
    QVERIFY(i_cirq.isNull());
    i_cirq.allocate(5);
    QVERIFY( ! i_cirq.isNull());
    QVERIFY(i_cirq.isEmpty());
    QCOMPARE(i_cirq.capacity(), 8);
    i_cirq.enqueue(4);
    QVERIFY( ! i_cirq.isEmpty());
    QCOMPARE(i_cirq.size(), 1);
    int i4 = i_cirq.dequeue();
    QVERIFY(i_cirq.isEmpty());
    QCOMPARE(i4, 4);
}

