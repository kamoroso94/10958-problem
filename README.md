# 10958-problem
A command line program that brute forces all possible expressions using digits in ascending order to compute a given number.  This project was inspired by the video, [The 10,958 Problem](https://youtu.be/-ruC5A9EzzE) by Numberphile and presented by Matt Parker.  Fair warning, it is extremely inefficient, and therefore can take anywhere from seconds to days to finish depending on the input.

## How to Compile
You must include the math library.
```sh
gcc findexpr.c -lm -o findexpr
```

## How to Run
Supply a number to find an expression for as an argument.
```sh
./findexpr 10958
```
