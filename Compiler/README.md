# Compiler project

- [Compiler project](#compiler-project)
    - [Build](#build)
        - [Requirements](#requirements)
        - [Configure](#configure)
        - [Compilation](#compilation)
        - [Testsuite](#testsuite)
    - [Language reference](#language-reference)
        - [Variables](#variables)
        - [Stdin and stdout](#stdin-and-stdout)
        - [If and while](#if-and-while)
        - [Functions](#functions)
        - [Arrays](#arrays)
    - [Graphics math](#graphics-math)

### Build
##### Requirements 

* SFML
* CMake

##### Configure

```bash 
mkdir build 
cd build 
cmake ../ 
cmake --build .
```

##### Compilation

```bash 
./mcl -f mcl-project.mcl -o mcl-out.ll
clang mcl-out.ll
./a.out
```

##### Testsuite

```bash 
cd build && ruby ../scripts/test.rb
```

### Language reference

To see examples, go to the **tests** folder.

##### Variables
```cpp
x = 1;
y = x + 6 - (2 * x)
```
C - style math expressions
##### Stdin and stdout
```cpp
x = 2 * ?
print x + 2
```
##### If and while
```cpp
x = 6;
if x < 10 {
  print x;
}
while (x < 10) {
  x = x + 1;
  print x;
}
```
##### Functions
```python3
def foo {
  ret 1 + ?;
}
print [foo];

# And usage arguments

def foo | x y {
  if x < y {
    ret x;
  }
  ret y;
}
print [foo ? ?];
```

Have to mark, functions are in scope and can have the same 
names as variables or arrays

```python3
if foo < 10 {
  def foo {
    ret 1;
  }
  print [foo] + foo;
}
```

##### Arrays
```cpp
@init arr 7@

x = 0;
while x < 7 {
  @set arr x ?@ // set name idx value
  x = x + 1;
}

x = 0;
while x < 7 {
  print @get arr x@; // get name idx
  x = x + 1;
}
```

Set and init is completed expressions.

Get is rvalue.

### Graphics math

![Math explanation](Heating.jpg)