# Project Overview

The purpose of this project is to develop a C language program capable of performing a variety of calculations. The program is designed to process input data from a file, which includes a set of commands directing the program to carry out operations such as addition, subtraction, multiplication, division, exponentiation, modulo calculations in different number systems, as well as conversions between numeral systems. Upon completion, the program generates an output file that captures both the initial data and the resulting calculations.

# Compilation

### Required
- C compiler (e.g., GCC, MSVC, Clang)
- CMake

## Windows

```bash
mkdir build && cd build
cmake -G "Your Generator (e.g. Visual Studio 17 2022)" ..
```

## Linux and MacOS

```bash
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
```

# Usage

Run the following command in your terminal:

```bash
oa-count file_path
```

where `file_path` can be either an absolute or relative path to a text file. The file name must include an extension.

# Input

The program accepts text files with names in the format: **oa_00_in.txt**, **oa_01_in.txt**, etc.

## Supported Operations:
- Addition (+)
- Subtraction (-)
- Multiplication (*)
- Power (^)
- Division (/)
- Modulo (%)
- Base Conversion (e.g. from base 16 to base 10)

## Command format

Commands should follow this format:

1. Symbol of operation **(i.e., +, -, \*, ^, /, %)**
2. Base systems for conversion, specified from source to destination (if no operation specified, assumes conversion)
3. For conversion commands, enter the number to convert on the next line
4. For other operations, enter both arguments on their respective lines
5. Separate each line using a newline character
6. End the command with three consecutive newlines

Example input is provided below. For more sample inputs refer to the `examples` directory.

## Example input

 ```
^ 16

14

9



2 16

10111110101111000010000000000000000



/ 10

56

0



10 16

120



* 16

5F5E10000

D1



+ 8

567132454267112757001

541161346060641561006



* 10

132436254377099686453413089013100567546

1323487634648903546890542680531478012424



/ 13

C403AB

BCA



% 14

D468BAC

6859CD0



 ```

# Output

The program generates a text file with a name in the format: **oa_00_out.txt**, **oa_01_out.txt**, etc.

> Output content mirrors the input file except at locations with three consecutive newlines, where results appear separated by newlines.

## Error Detection

In case of errors, the result of a operation might be: **"Error"**, **"Timeout"**, or **"Invalid Operation"** along with error descriptions displayed in the terminal. Invalid formatted input file triggers the message **"Wrong file formatting!"**.

## Example output

<pre>
^ 16

14

9

<i><b>7735940000</b></i>

2 16

10111110101111000010000000000000000

<i><b>5F5E10000</b></i>

/ 10

56

0

<i><b>Error</b></i>

10 16

120

<i><b>78</b></i>

* 16

5F5E10000

D1

<i><b>4DDBCB10000</b></i>

+ 8

567132454267112757001

541161346060641561006

<i><b>1330314022347754540007</b></i>

* 10

132436254377099686453413089013100567546

1323487634648903546890542680531478012424

<i><b>175277745047308163008567685110218825059529723873990638867644744616974439191504</b></i>

/ 13

C403AB

BCA

<i><b>1047</b></i>

% 14

D468BAC

6859CD0

<i><i><b>192DCC</b></i></i>

</pre>

# Configuration

- The program supports conversions from **base-2** up to **base-16**
- Argument and result lengths have maximum values set to **40** and **80** characters respectively
- A single operation takes up to **5** seconds

These limitations can be adjusted in the `config.h` file within the source directory. Changes require recompiling the program from source.

# Implementation

For detailed information about the implementation, please refer to the `docs` folder and consult [this document](docs/Implementation.md).

## Unsupported Features

- Floating-point arithmetic
- Negative numbers
