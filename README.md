# Operating systems practical test playground

## Commands

To create a new file for an exercise, use 
```cmd
make create type=[c|py|sh] name=[a-zA-Z0-9_]+ desc=[everything]
```

To compile and run a C file, use
```cmd
make run func=[a-zA-Z0-9_]+ options=[a-zA-Z0-9_]+
```
where `func` is the name of the function to run, and `options` are the options to pass to the function.

> ## Note:
> Every new `C` file will be created in the `src` folder, with default includes and a function that has the same name as the file. The function typing is $$(\text{int}, \text{char**}) \to \text{int}$$ The function will be called in the main function, with arguments passed from the main func.
>
> In addition, an extern declaration will be created in `ex.h` with the description passed at the creation of the new script.
>
> The `py` and `sh` files will be created in the `scripts` folder, with the same name as the file, and the same description as the `C` file.
>
> 
## Examples

Running the following command
```cmd
make create type="c" name="test" desc="This is a test"
```

creates a new file `test.c` in the `src` folder, with the following content:
```c
//some default includes...

int test(int argc, char** argv) {

    return EXIT_SUCCESS;
}
```

and a new entry in `ex.h`:
```c
/**
 * This is a test
 */
extern int test(int argc, char** argv);
```

Running the following command
```cmd
make run func="test" options="1 2 3"
```

will create a new executable `test` in the `bin` folder, and run the latter with the arguments `1 2 3`.

## Scripts

To create a new script, run
```cmd
make create type="py" name="your-script" desc="This is a script"
```

This will create a new file `your-script.py` in the `scripts` folder, with a function with the same name as the file that will be called in the main.

To run a script, either change the directory to `scripts` and run it with `python3 your-script.py`, or run
```cmd
make run-script script="your-script" options="1 2 3"
```

