# Unicode console

Small class that makes console input/output with utf-8 easier in c++17.

## Usage
### Read (UTF-8 string) from console
```cpp
auto const input = unicode_console::read<std::string>();
```
Or:
```cpp
std::string input;
unicode_console::io >> input
```

### Read a line of UTF-8 text from console
```cpp
auto const input = unicode_console::readln();
```

### Write to console
```cpp
unicode_console::println("Here is a little unicode: åäö ÿëã")
        .println("And a number: ", 1234);
```
Or:
```cpp
unicode_console::io << "Here is a little unicode: åäö ÿëã\n" 
            << "And a number: " << 1234 << '\n';
```


### Validate input from console
#### Only validate type (for numbers)
```cpp
auto const input = unicode_console::read_validated<double>("That's not a number! Try again.");
```

#### Custom validation + type validation with single error message
```cpp
// input should be between -3.14 and 3.14
auto const input = unicode_console::read_validated<double>(
    [](auto const input) {
        return input > -3.14 && input < 3.14;
    }, 
    "Invalid input! Try again."
);
```

#### Custom validation + type validation with separate error messages
```cpp
// input should be between -3.14 and 3.14
auto const input = unicode_console::read_validated<double>(
    [](auto const input) {
        return input > -3.14 && input < 3.14;
    }, 
    "That's outside of the range! Try again.",
    "That's not a number!"
);
```

