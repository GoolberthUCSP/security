# File infector

A simple file infector with no replication. Developed in c++ for executable files in linux, this will just infect the executables inside the folder.

## Execution

+ Run *make*.
+ If it's the first time running in your PC, you have to update the virus size parameter in *virus.cpp*. To get It's size, use *ls -l test/virus*. Then save changes and compile again running *make*.
+ Move to *test* folder.
+ Run *./main*, it's an simple example of hello world in c++.
+ Run *./virus*, it will infect the executables in the same folder than itself.
+ Run *./main* again, it will be infect and just will print *I'm a virus*.