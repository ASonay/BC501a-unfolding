/*
This is external program to re-drawing last unfolding.
Your file input must be same with last run.

Parameters:
  
  --ino: Iteration number which one you want to access.
  --fopr: Recoil spectrum, same with last run.
  --BinSize: Corresponded bining for recoil spectrum.
  --fguess: You can set some external flux to the comparison.

2017/12/10 - Anil Sonay
*/
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"


#include "include/variables.hh"
#include "include/file.hh"
#include "include/draw.hh"
#include "include/engine.hh"
#include "include/post.hh"


void canmaker(int ino,
	      const char* fopr = "./input/AmBe_sim_bin20.dat",  
	      int BinSize = 20,
	      const char* fguess = ""      
)
{
  
  TFile *f = new TFile("./output/output_data.root");
  TTree *tr_recoil = (TTree*)f->Get("tr_recoil");
  TTree *tr_unfold = (TTree*)f->Get("tr_unfold");
  TTree *tr_time = (TTree*)f->Get("tr_time");
  
   //-----------------------------------------------------------------------------------------
  file.read_recoil(fopr);
  file.read_guess(fguess);
  //-----------------------------------------------------------------------------------------

 
  Double_t rc[1000],flx[1000];
  TBranch *recalc;
  TBranch *getflx;

  recalc = tr_recoil->GetBranch("re_recoil");
  recalc->SetAddress(rc);
  getflx = tr_unfold->GetBranch("flux_branch");
  getflx->SetAddress(flx);
  
  for (int i=0;i<ni;i++)
    energy[i] = i*RMBinSize;
      
  for (int it=0;it<ino;it++)
    {
      for (int i=0;i<nj;i++)
	{
	  recalc->GetEntry(i);
	  re_recoil_vec[i][it]=rc[it];
	}
      for (int i=0;i<ni;i++)
	{
	  energy[i] = i*RMBinSize;
	  getflx->GetEntry(i);
	  flux_branch_vec[i][it]=flx[it];
	}
      printf("reading data \%%%1.1f completed...\n",((Float_t)(it+1)*100)/(Float_t)(ino));
    }
  
  c1->Divide(2,1);
  for (int it=0;it<ino;it++)
    {
      for (int i=0;i<nj;i++)
	recoilCalculated[i]=re_recoil_vec[i][it];
      for (int i=0;i<ni;i++)
	flux[i]=flux_branch_vec[i][it];
      draw.unfolding();
    }
  
}
