#include "TAElement.h"

void main()
{
    TAInt x;
    TADouble d;
    TACeil y(d); // denotes y is the ceiling of d
    TABool b;
    TALessThan t1(x, y); // denotes x < y
    TAAnd t2(b, t1); // denotes b and t1
    t2.list();// should print: (& b (< x (ceiling d ) ) )
    cout << endl;
    x.set(5);
    d.set(2.3);
    b.set(true);
    TABool resultt1;
    t1.evaluate(resultt1);
    resultt1.list();// should print: false
    cout << endl;
    d.set(5.3);
    t1.evaluate(resultt1);
    resultt1.list();// should print: true
    cout << endl;
}