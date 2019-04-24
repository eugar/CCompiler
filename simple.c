int a = 5;

/*char fun()
{
    int v = 10 + 9;

    return v;
}*/

int main ()
{
    int z = 7 + 2 * 6;
    //int b = 2 * 3 + 4;
    //char a = 'v';         // x86 gen doesn't recognize chars ?
    int g = 6 / 3;        // x86 gen doesn't recognize DIV ?
    //g = 5;                // not working in the IR gen yet!!

    return z+5*6;
}
