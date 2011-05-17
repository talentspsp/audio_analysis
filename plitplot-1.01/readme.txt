==============================================================================

        Plit Plot - An EPS graph drawing library in C++
        v1.01

        Copyright (c) 2003-2005 Laurent de Soras
        Including code by Olli Niemitalo

==============================================================================



Contents:

1. Legal
2. What is Plit Plot ?
3. Using Plit Plot
4. Compilation
5. History
6. Contact



1. Legal
--------

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Check the file license.txt to get full information about the license.

This package also contains code by Olli Niemitalo and modified by Laurent de
Soras. See EPSPlot.h for details.



2. What is Plit Plot ?
----------------------

Plit Plot is a library for drawing graphs in EPS file from a C++ program.
The library is very simple, don't expect prodigious functions. I made Plit
Plot in order to draw nice-looking graphs for a technical article. Indeed,
after having implemented some algorithms crunching numbers, and wanted to
output quickly the results without the need of an external drawing program.

I started with the excellent EPSPlot from Olli Niemitalo, then added a few
basic objects covering my needs: 2D plot with axis, linear and log scales,
legends and easy interfacing with the data source program.



3. Using Plit Plot
------------------

Create an EPSPlot object. It will be the main drawing tool. Then create a
Gridaxis object. Attach the EPSPlot object to it.

Configure the axis and render the background with the Gridaxis object.
Use it to render the curves or discrete point sets. At any moment you can
access the EPSPlot object to add exta-shapes, change the pen and filling
patterns, etc. See examples in main.cpp for details.

In Plit Plot, there are three coordinate systems:

- Points (1/72 inch), used by EPSPlot

- Graph coordinates, used by GraphPrim. They are relative to the graphic
bounding box, meaning that the range [0 ; 1] is in the box

- "value" coordinates, depending on what is displayed in the graph. Used
by the Axis and Gridaxis objects.

Generally you'll use only the value coordinates, but you can convert them
to graph coordinates with the functions provided in Axis object, in order
to use the GraphPrim functions.

I admit Plit Plot design is a bit wobbly...

Plit Plot is intended to be portable, but currently has been tested only on
MS Windows / MS Visual C++ 6.0 and GCC 3.3.3.



4. Compilation
--------------

Drop all the .h, .cpp and .hpp files into your project or makefile.

main.cpp is for testing purpose only, do not include them if you just need to
use the library.

Plit Plot (library classes and test program) may be compiled in two versions:
release and debug. Debug version has checks that could slow down the code.
Define NDEBUG to set the release mode. For example, the command line for GCC
would look like:

Debug mode:
g++ -Wall -o plitplot_debug.exe *.cpp

Release mode:
g++ -Wall -o plitplot_release.exe -DNDEBUG -O3 *.cpp



5. History
----------

v1.01 (2004.02.21)
    - Fixed a few details to make it compile with GCC.

v1.00 (2004.02.20)
    - Initial release.



6. Contact
----------

Please address any comment, bug report or flame to:

Laurent de Soras
laurent.de.soras@club-internet.fr
http://ldesoras.free.fr

