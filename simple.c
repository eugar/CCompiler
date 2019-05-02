
// Not so simple any more ¯\_(ツ)_/¯

int func()
{
    int a = 5;
    int c = 0;

    while (c < 5)
    {
        if (c > 0) {
            a = a * c;
        }
        c = c + 1;
    }

    return a;
}

int square()
{
    int a = 5;
    a = a * a;
    return a;
}

int five()
{
    return 5;
}

int main ()
{
    int a = 5;
    int t = 1 + (a + 7);
    int f = square();

    while(a < t)
    {
        if (a > 8)
        {
            f = f - 1;
        }
        a = a + 1;
    }

    if(f < 5)
    {
        a = func();
    }
    else
    {
        a = (f + 27) * t;
    }

    return a;
}
