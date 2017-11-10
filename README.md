# VTF Converter

Simple Linux binary to convert image files to VTF/VMT output using VTFLib.

This library was developed for use with a web service and a specific workflow, so it does not provide much in the way for options. 
There are existing tools for more flexible VTF work for that matter.

## Compiling

This project has been set up with Visual Studio with the intention of compiling and debugging via WSL (Windows Subsystem for Linux).
In theory you can build this with your IDE of choice, but no CMake files have been provided.

Pre-requisite packages: 
* libfreeimage-dev - Used for reading a common selection of image formats.
* build-essential - Simple quickstart package for compile tools.
* gdb - For Visual Studio / WSL.
* gdbserver - For Visual Studio / WSL.
* vtflib - (no apt package available, use the linux port provided here: https://github.com/panzi/VTFLib)
* libtxc-dxtn-s2tc0 - Dependency for vtflib to allow DXT compression.
