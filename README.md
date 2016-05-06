This is a very simple example of how to embed R in Python. Other mechanisms are out there to do this (e.g. [Rpy](http://rpy.sourceforge.net/) and [pyRserve](https://pypi.python.org/pypi/pyRserve/)) but this might be useful if you have simple requirements and want to limit the number of dependencies you have.

Remember that if you are using this on a server, R is not reentrant, so this may be racy if you don't set it up right.


## Introduction

The function defined here evaluates an arbitrary set of R expressions, and will return the result of the last if it is a string or vector of reals. Extending this to other types would be straight forward (though perhaps fiddly), but at the time of writing this was just a demonstration project.

## Files
- init.h, init.cpp: Funcitons to setup and tear down R,
- library_attach.cpp: Calls the functions defined in init.h when the library is attached/detached,
- PythonEmbeddedR.cpp: Defines the one function of this package, and
- init.cpp: Defines the functions declared in init.h.

## Compiling

On windows, you can just use the solution provided, after modifying the include and library paths (via project->properties) to suite your environment.

If your using GCC or any other compiler, then you can probably figure it out yourself (... and I mean that respectfully), though if you need a hand, please ask and I will document it.

## Notes

You will need to have:

- The environment variable R_HOME set.
- The R shared library on the path.
