Mountain Collection
===================


Project Setup:
-------------

The following PROJECT SETTINGS are required:
 -Enable Virtual Texture Support      [Enabled]
 -Generate Mesh Distance Fields       [Enabled]

The following PROJECT SETTINGS are recommended:
 -Dynamic Global illumination Method  [None]
 -Shadow Map Method                   [Shadow Maps]
 -Allow Static Lighting               [Disabled]


Landscape Setup:
---------------

 1. Assign the included M_MasterLandscape to your landscape.
 2. Add two Virtual Texture Array Elements in your landscape details tab.
    -Set one to the included RVT_Height, 
    -Set one to the included RVT_Mat,
    -Click Create Volumes.
 3.For each created RVT volume in the outliner you must click Set Bounds.
   Whenever you edit the landscape you will need to set bounds again.

If your grass textures are broken, RVT has not been set up correctly:
 -Virtual Textures must be enabled for Runtime Virtual Textures to work,
 -Both RVT volumes must be assigned to the landscape,
 -The bounds must be set by clicking Set Bounds.