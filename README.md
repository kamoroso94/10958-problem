# 10958-problem
A command line program that brute forces all possible expressions using digits in ascending order to compute a given number.  This project was inspired by the video, [The 10,958 Problem](https://youtu.be/-ruC5A9EzzE) by Numberphile and presented by Matt Parker.  Fair warning, it is extremely inefficient, and therefore can take anywhere from seconds to days to finish depending on the input.

## How to Compile
You must include the math library.
```
gcc findexpr.c -lm -o findexpr
```

## How to Run
Supply a number to find an expression for as an argument.
```
./findexpr 10958
```

## Sample Output
Sometimes the brute force search turns up interesting results.  I imagined 1+2+3+4+5+6+7+8+9=45 would come up with the simple sum first, but there was a different expression it found first.
```
./findexpr 45
Success? y
45 = (123)+(((4)-(((5)-(6))*(7)))-(89))
```
