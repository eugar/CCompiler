# CCompiler Design Document
------------------------------------
## Compilation:
This project uses [CMake](https://cmake.org/) to generate a makefile and compile.
Please install CMake and generate the CMakeFiles first.
Steps (for macOS) are shown below:

### Step 1
![stepone](misc/one.png)

* Go to [cmake.org/download](https://cmake.org/download) and download cmake-3.x-Darwin-x86_64.dmg

### Step 2
![steptwo](misc/two.png)

* Open the downloaded .dmg file and click the agree button (if you agree to the terms)

### Step 3
![stepthree](misc/three.png)

* Drag the CMake app into your applications folder to install it

### Step 4
![stepfour](misc/four.png)

* Find CMake and open it

![stepfourr](misc/fourr.png)

* Click "Open"

### Step 5
![stepfive](misc/five.png)

* Click "Browse Source..." and set it to this directory

* Click "Browse Build..." and set it to this directory

* Click "Configure"

* Click "Generate"

* The Makefile has now been generated!

### Step 6
![stepsix](misc/six.png)

* Navigate to this project via command line

* Type ``make`` -- you should see output similar to what is shown

* The executable has now been created and you are ready to run the compiler!


## Usage:
ccompiler - compile a C file
```
./ccompiler [FILE]

Optional arguments:
-h displays this help menu
-s displays list of tokens
-p displays parse tree
```

## Limitations:
* The compiler, in its current state, does not process 'for' loops -- it only recognizes them.

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
    vector<Tok> m_tokenList;
```
...
etc.
