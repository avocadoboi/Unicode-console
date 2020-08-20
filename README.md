# Unicode console

Small class that makes console input/output with utf-8 easier in c++17.

## Usage
### Read (UTF-8 string) from console
```cpp
auto const input = Console::read<std::string>();
```
Or:
```cpp
std::string input;
Console::io >> input
```


### Write to console
```cpp
Console::println(u8"Here is a little unicode: åäö ÿëã")
        .println("And a number: ", 1234);
```
Or:
```cpp
Console::io << u8"Here is a little unicode: åäö ÿëã\n" 
            << "And a number: " << 1234 << '\n';
```


### Validate input from console
#### Only validate type (for numbers)
```cpp
auto const input = Console::readValidated<double>("That's not a number! Try again.");
```

#### Custom validation + type validation with single error message
```cpp
// input should be between -3.14 and 3.14
auto const input = Console::readValidated<double>(
    [](auto const input) {
        return input > -3.14 && input < 3.14;
    }, 
    "Invalid input! Try again."
);
```

#### Custom validation + type validation with separate error messages
```cpp
// input should be between -3.14 and 3.14
auto const input = Console::readValidated<double>(
    [](auto const input) {
        return input > -3.14 && input < 3.14;
    }, 
    "That's outside of the range! Try again.",
    "That's not a number!"
);
```

