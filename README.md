# A basic terminal command dispatcher

This is a minimal shell interperter written in C.  It is designed to be
small and self contained making it ideal for embedding in microcontroller
projects to make running test or diagnostic commands via a debug serial
interface simple.

## Demo

There's a very basic demo you can build on a Linux host machine to try
out the basics.  From inside a fresh checkout simply run:

    cd demo
    mkdir build
    cd build
    cmake ..
    make

Then you can run `./ndsh_demo` to try it out.

## Extending

To add commands, the simplest approach is to make a list of structs with
the shell command as a string and a function pointer to the code to run
when the shell command is entered.  This is the approach taken in the
demo.

Alternatively you can override the default `command_dispatcher` by creating
your own function which finds and executes the command based on the list
of command arguments.  You could find scripts or binaries any way you want
and just return the result.

## Exit

Currently any command which fails (does not return 0) will cause the shell
to exit (like a bash script with `set -e`).  You can exit by hitting `Ctrl-D`
at a prompt as well.  Probably this behaviour will change and be made
configurable to make interactive shells more permissive to failed commands
at some point.
