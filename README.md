# Unicode console

Small class that makes console input/output with utf-8 easier in c++.

## Usage
### Read (UTF-8 string) from console
```cpp
auto input = console.read<std::string>();
```
Or:
```cpp
std::string input;
console >> input
```


### Write to console
```cpp
console.println(u8"Here is a little unicode: åäö ÿëã")
       .println("And a number: ", 1234);
```
Or:
```cpp
console << u8"Here is a little unicode: åäö ÿëã\n" 
        << "And a number: " << 1234 << '\n';
```


### Validate input from console
#### Only validate type (for numbers)
```cpp
auto input = console.readValidated<double>("That's not a number! Try again.");
```

#### Custom validation + type validation with single error message
```cpp
// input should be between -3.14 and 3.14
auto input = console.readValidated<double>(
    [](auto input) {
        return input > -3.14 && input < 3.14;
    }, 
    "Invalid input! Try again."
);
```

#### Custom validation + type validation with separate error messages
```cpp
// input should be between -3.14 and 3.14
auto input = console.readValidated<double>(
    [](auto input) {
        return input > -3.14 && input < 3.14;
    }, 
    "That's outside of the range! Try again.",
    "That's not a number!"
);
```

