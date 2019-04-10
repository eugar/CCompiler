int fun ()
{
    int t = 1;
    int z = 5 + 3;
    //test
    return t;
}

int main (int v, int a)
{
    int z = fun();
    /*
     * block test
     */

    if(a < z)
    {
        int a = 8;
    }

    if(z > a)
    {
        int c = a;
    }

    while(a<z)
    {
        int b = 40;
    }

    return z;
}
