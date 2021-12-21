Bfc is a simple, high-performance compiler for .bf programs. It features an optimizer that speeds up the code by removing redundancies.

## Usage
To build it, just run `make test`, which should create an executable file called `test`. You can use this program to compile .bf files. This will result in the generation of a .S file, which is an assembly file. It's *highly* recommended that you use the O2 option, as the others are just for debugging purposes.
```sh
./test tests/mandelbrot.bf -o output.S
```
Once this assembly file is created, you can use `gcc` to create an object file and then an executable. What I normally do is:
```sh
./test tests/mandelbrot.bf -o output.S
gcc -c output.S -o output.o
gcc output.o -o output
./output
```