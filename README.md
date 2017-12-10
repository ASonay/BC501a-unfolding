This is source code for unfolding method that can be used in ROOT, 
which is reconstruct neutron spectrum from recoil spectrum. 
In the same purpose, code can be used to solve back to gamma 
line from electron recoil spectrum by changing response matrix.

Author: Anil Sonay
Date: Dec. 2017

anilsonay@gate.sinica.edu.tw / @gmail.com

The source code has prepared for GRAVEL and Doroshenko method which are 
defined in engine.hh. Response matrix has been created for neutron proton 
scattering inside the BC501-a liquid scintillator. The bin size of the 
recoil spectrums has been set for 2 keV, which can be change in the 
program by users and created for each 20 keV incident neutron.

User defined parameters;

  --Initial flux: You can set your initial flux manually, Proper 
bining is 20 keV. If you don't it will be straight line at one.
  --Recoil spectrum: This is your main input must be defined.
0 and negative numbers are not acceptable. First column must be
energy in MeV unit, second column count and the third column 
error. Your first energy will taken as threshold.
  --Bin size: Recoil spectrum bin size must be defined. Response
matrix will modified accounted to this. Your bining must be power
of two (2^n).
  --Method: Default model is setting up to 0 for GRAVEL method,
any other input will run the Doroshenko.

You can also create your own response matrix. Current one has
1000x10000 dimension for 20x2 keV bin size. If you gonna do 
change for dimension, you need to change some values in 
variables.hh, file variables section.
-------------------------------------------------------------
-------------------------------------------------------------
Example run:

A-) Unfolding

root -l unfold.cpp
root -l 'unfold.cpp("./input/AmBe_initial_n.dat","",20,1)'
root -l 'unfold.cpp("","./input/AmBe_sim_bin40.dat",40)'

B-) Re-Drawing

root -l 'canmaker.C(20)'
root -l 'canmaker.C(10,"",20,"./input/AmBe_initial_n.dat")'
root -l 'canmaker.C(30,"./input/AmBe_sim_bin40.dat",40,"")'
-------------------------------------------------------------
-------------------------------------------------------------