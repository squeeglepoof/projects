To add in RAGS:

The high-level A* is called in the class "TypeGraphManager", in TypeGraphManager.h. Every UAV has access to one pointer to a TypeGraphManager object.

The easiest, but hackiest way to use RAGS, would be to copy over the 'astar()' function in Typegraphmanager with your RAGs algorithm. Less easy, but more proper, is to make a rags() function in the typegraphmanager, and then create a mode in UTMModesAndFiles for using RAGS, and add a switch statement for which mode UAVs use based on the mode set.


To modify domain parameters:

Look in the file UTMModesAndFiles.h. UTMModes has a constructor that sets the modes for the simulation. This is accessed in order to construct the domain and run the project.