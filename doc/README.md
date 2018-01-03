Graphics engine
===============

This project is ostensibly for vizualising time series, but secretly a game engine.

Time series vizualiser
======================

This OpenGL project will take in time series data, like say bank transaction data or a program execution profile. It will vizualise these data in different modes.



Dimension homog
--------

This mode will add another, homogenous dimension to the data, indicating visually where outliers are, such as bottlenecks/timesinks/large costs etc.
A number of optional, geometric tricks using e.g. voronoi tesselation using some extra points will be included. 


Dimension color
------------
This mode will add a color dimension to the data, to further 


Mode 0
------
This mode will simply draw a graph of the data, just straight up honest graphs.


Mode blend
------------

This mode can be added to any of the dimensions, physical or otherwise, to give the dimension a continuous curvature, where sensible.

Mode exaggerate
---------------
This mode will square the value at the point.











