/*
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
*/


#include<math.h>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"

#include "include/variables.hh"
#include "include/file.hh"
#include "include/draw.hh"
#include "include/engine.hh"
#include "include/post.hh"


void unfold(const char* fguess = "",                           //Initial flux
	    const char* fopr = "./input/AmBe_sim_bin20.dat",   //Recoil spectrum
	    int BinSize = 20,                                  //Set your bining
	    int model = 0                                      /*Set method from two defined
								 model.Default setting 
								 choosen for GRAVEL method*/
)
{
  //-----------------------------------------------------------------------------------------
  //SOME TUNING------------------------------------------------------------------------------
  
  //Get name of the file and set up files base on this.
  BaseFile(fopr); 
  gStyle->SetLabelOffset(-0.89,"Y");
  //-----------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------

  
  //-----------------------------------------------------------------------------------------
  //READ INPUT-------------------------------------------------------------------------------
  file.read_recoil(fopr);        //Read recoil spectrum
  file.read_guess(fguess);       //Read initital flux
  file.read_RM(BinSize);         //Read & Config response matrix. (Or. Bin size settled 2 keVee)
  //-----------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------
  

  //-----------------------------------------------------------------------------------------
  //CALCULATE FLUX---------------------------------------------------------------------------
  c1->Divide(2,1);               //Global canvas defined to redraw each iteration
  chi2dof=1000;
  //Stop iteration by chi2 but this may not be proper way. (d.o.f. may need to be fix)
  //IterationNo = 50;            //Limit of the iteration.
  while( chi2dof >= .1)
    {
      if (model==0)
	engine.gravel();         //Unfolding by GRAVEL method
      else
	engine.doroshenko();     //Unfolding by DOROSHENKO method
      post.avarage();            //Smoothing to flux
      draw.unfolding();          //Re-Draw each iteration
      cont++;
      //stop iteration if exceed to limit
      if (cont > IterationNo) break;
    }
  draw.fill_data();              //Create NTuple for all process.
  sprintf(iw,"./image/%s.png",base);
  c1->Print(iw);
  //-----------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------

  
  //-----------------------------------------------------------------------------------------
  //WRITE OUTPUT-----------------------------------------------------------------------------
  sprintf(fopw,"./output/flux_%s.dat",base);
  finalFlux = fopen(fopw,"w+");
  for(int i=0;i<ni;i++)
    fprintf(finalFlux,"%lf %e\n",energy[i],flux[i]); 
  fclose(finalFlux);
  
  TFile *foo = new TFile(Form("spectrums_%s.root",base),"recreate");
  draw.gflux->Write("gflux");
  draw.gflux_init->Write("gflux_init");
  draw.gmeas->Write("gmeas");
  draw.gcalc->Write("gcalc");

  foo->Close();
  //-----------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------
  
}

void BaseFile(const char* fname){
  int cpyl;
  for (cpyl=200;cpyl>=0;cpyl--)
    if (fname[cpyl] == '.') break;
  int cpyu;
  for (cpyu=200;cpyu>=0;cpyu--)
    if (fname[cpyu] == '/') break;
  if (cpyl-cpyu-1>cpyu+1)
    strncpy(base, fname+cpyu+1, cpyl-cpyu-1);
  else
    strncpy(base, fname+cpyu+1,200);
}
