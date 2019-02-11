# CCompiler Design Document
------------------------------------
## Usage:
ccompiler - compile a C file
Usage:
./ccompiler [FILE]
Optional arguments:
-h displays this help menu
-s displays list of tokens
-p displays parse tree

## Keywords:
* break
* char
* double
* else
* float
* for
* goto
* if
* int
* return
* short
* while

## Code Standards:

### 1
Curly Braces ('{' and '}') will be placed on their own line. For example:
```
int main(int argc, char * const argv[])
{
    return 0;
}
```

### 2
Member variables will have a preceding "m\_" followed by CamelCase naming:
```
class Compiler
{
private:
    Scanner m_scanner;
    vector<Token> m_tokenList;
```
...
etc.
