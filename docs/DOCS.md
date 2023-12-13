# Rusted-C 

## Overview

Rusted-C is a minimalistic programming language designed for ease of use and simplicity. 
It features basic constructs for expressing mathematical computations and includes variables, functions, control flow instructions and user input.

Variables can have these types:
   - number (float or int) 
   - string (characters) 
   - null (no value)


The language is interpreted by the provided C++ interpreter.



## Syntax


### Constants and Variables

Constants are defined using the `const` keyword, and variables are implicitly declared.
Variables of type const must have an initialized value during declaration.
However, non-const variables, if not initialized during declaration, default to a null value.

```javascript
const PI = 3.14; // 3.14 
let x = 4; // 4
let y; // null
```

### Mathematical Operations

Basic mathematical operations such as addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), power (`pow`), and square root (`sqrt`) are supported.

```python
10 + 20 - (3 * 4) / 2 * 2 

pow(2, 3) * sqrt(16)
```


### Printing

The `print` function is available for outputting values to the console.

```python
print("Hello world")
```


### Functions 

**Description of Function Declaration and Invocation:**

**Function Declaration:**

In the Simple Language, functions are declared using the `func` keyword, followed by the function name and a list of parameters within parentheses. The function body is enclosed in curly braces `{}`.

```go
func addNumbers(a, b) {
  return a + b;
}
```

- **Declaration:** The `func` keyword is used to declare a function named `addNumbers` that takes two parameters, `a` and `b`. The function body calculates the sum of `a` and `b` and returns the result.

**Function Invocation:**

To call or invoke a function in the Simple Language, you use the function name followed by parentheses, providing the required arguments within the parentheses.

```javascript
const result = addNumbers(5, 7);
print("The sum is:", result)
```

- **Invocation:** The `addNumbers` function is invoked with arguments `5` and `7`. The returned result is stored in the variable `result`, and then the `print` function is used to display the result.

**Example Usage**

```go
func greet(name) {
  print("Hello, " + name + "!");
}

const userName = "John";
greet(userName)
```

In this example, a function named `greet` is declared to print a greeting message. The function is then invoked with the `userName` variable as an argument, resulting in the output "Hello, John!".

For a list of built-in functions in the Simple Language, refer to the [Built-in Functions documentation](BUILTIN.md).

Functions provide a way to encapsulate reusable pieces of code, enhancing the modularity and readability of the program.


### Conditional Statements

**Description of the If-Else Statement:**

The `if-else` statement is an extension of the `if` statement, providing an alternative code block to be executed when the specified condition is false. The basic structure of an `if-else` statement looks like the following:

```javascript
if (condition) {
  // Code block to be executed if the condition is true
} else {
  // Code block to be executed if the condition is false
}
```

- **Condition:** The code block within the `if` statement is executed if the specified condition is true.

- **Body (if true):** The block of code within the first set of curly braces `{}` represents the body of the `if` statement. This code is executed when the condition is true.

- **Body (if false):** The block of code within the second set of curly braces `{}` following `else` represents the alternative body of the statement. This code is executed when the condition is false.

**Example Usage:**

```javascript
const temperature = 25;

if (temperature > 30) {
  print("It's a hot day!")
} else {
  print("It's not too hot today.")
}
```

In this example, the `if-else` statement checks whether the `temperature` is greater than 30. If the condition is true, it prints a message indicating a hot day. Otherwise, if the condition is false, it prints a message indicating that it's not too hot. The `if-else` statement provides a way to handle both true and false conditions, allowing for more versatile decision-making in the program.

### Loop statement

**Description of the While Loop:**

The `while` loop is a construct that enables the repetitive execution of a block of code based on a specified condition. 
In the context of the Rusted-C, the basic structure of a `while` loop looks like the following:

```javascript
while (condition) {
  // Code block to be executed repeatedly
}
```

- **Condition:** The loop continues to execute as long as the specified condition is true.

- **Body:** The block of code within the curly braces `{}` represents the body of the loop. This code is executed repeatedly as long as the condition remains true.

- **Iteration:** Within the loop, variables can be manipulated, and operations can be performed, influencing the flow of the loop.

- **Termination:** The loop terminates when the specified condition becomes false.

**Example Usage**:

```javascript
let i = 1;
const n = 5;

while (i <= n) {
  print("Iteration:", i)
  i = i + 1;
}
```

In this example, the loop prints the current iteration number while `i` is less than or equal to `n`. 
The loop iterates five times, demonstrating the basic structure and functionality of the `while` loop.

## Notes

- The language follows a C-style syntax with function-oriented programming features.
- It is designed for educational purposes and simplicity, making it suitable for beginners.
- The provided interpreter executes the rusted-c code, demonstrating the practical application of the language.

Feel free to explore and experiment with the language using the provided examples and modify the interpreter for further customization.
