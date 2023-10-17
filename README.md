# Spectral
A cool dynamic library based game engine

## The idea

A game engine that uses dynamic libraries to store compiled scripts, and uses C/C++ for a scripting language.

Also very extensible, down to the most low-level things, for example, an extension can add a custom scripting language that has a custom compile and load functions.
A compiler called muic (integrated c99) will be added as an extension that adds support for bytecode compilation instead of dll/so compilation.


## Directory structure
 - include: all common things used by scripts and/or the engine
 - scripts: some test scripts (will later be replaced with `projects` for test projects)
 - src: engine code

