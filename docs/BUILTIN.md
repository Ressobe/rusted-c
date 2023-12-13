# Built-in Functions

The Rusted-C comes with a set of built-in functions that provide essential functionalities. 
These functions can be used directly in your rusted-c programs to perform various tasks. 
Here is an overview of the available built-in functions:

## 1. `print`

The `print` function is used to output values to the console.

**Example:**

```javascript
print("Hello, world!")
```

## 2. `clear`

The `clear` function is used to clear the console or terminal screen.

**Example:**

```javascript
clear()
```

## 3. `sqrt`

The `sqrt` function returns the square root of a given number.

**Example:**

```javascript
const result = sqrt(16);
print(result) // Output: 4
```

## 4. `pow`

The `pow` function raises a given number to the power of another.

**Example:**

```javascript
const result = pow(2, 3);
print(result) // Output: 8
```

## 5. `round`

The `round` function rounds a floating-point number to the nearest integer.

**Example:**

```javascript
const result = round(3.14);
print(result) // Output: 3
```

## 6. `floor`

The `floor` function returns the largest integer less than or equal to a given number.

**Example:**

```javascript
const result = floor(4.9);
print(result) // Output: 4
```

## 7. `min`

The `min` function returns the smallest of the provided values.

**Example:**

```javascript
const result = min(2, 5, 1);
print(result) // Output: 1
```

## 8. `max`

The `max` function returns the largest of the provided values.

**Example:**

```javascript
const result = max(2, 5, 1);
print(result) // Output: 5
```

## 9. `abs`

The `abs` function returns the absolute value of a given number.

**Example:**

```javascript
const result = abs(-3);
print(result) // Output: 3
```

## 10. `input`

The `input` function is used to receive input from the user.

**Example:**

```javascript
const userInput = input("Enter a value: ");
print("You entered:", userInput)
```

## 11. `number`

The `number` function converts a string to a numeric value.

**Example:**

```javascript
const numValue = number("42");
print(numValue) // Output: 42
```

## 12. `string`

The `string` function converts a numeric value to a string.

**Example:**

```javascript
const stringValue = string(42);
print(stringValue) // Output: "42"
```

## 13. `len`

The `len` function returns the length of a string.

**Example:**

```javascript
const length = len("Hello");
print(length) // Output: 5
```

## 14. `exit`

The `exit` function terminates the program.

**Example:**

```javascript
exit()
```

Feel free to use these built-in functions in your Rusted-C to enhance their functionality. 
Refer to the provided examples and modify them according to your requirements.
