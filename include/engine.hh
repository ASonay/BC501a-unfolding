
//METHODE OPERATION--------------------

class engine_body{public: void gravel(); void doroshenko();} engine;



//###################################################################################//
void engine_body::gravel()
{
  chi2=0; chi2dof=1000;
  for (int m=0;m<nj;m++)
    {
      BuildRecoil=0;
      for (int l=0;l<ni;l++){
	BuildRecoil+=flux[l]*respMatrix[m][l];
      }
      recoilCalculated[m]=BuildRecoil;
      re_recoil_vec[m][cont]=recoilCalculated[m];
     }
  for (int i=0;i<ni;i++)
    {
      ReConsF=0;
      WeightFactorS=0;
      BuildRecoil=0;
      
      t.Start();
      for (int j=0;j<nj;j++)
	{
	  WeightFactor=(respMatrix[j][i]*flux[i]*((measSpec[j]/pow(measSpecerr[j],2))))/recoilCalculated[j];
	  WeightFactorS+=WeightFactor;
	  ReConsF+=WeightFactor*log(measSpec[j]/recoilCalculated[j]);
	  chi2+=pow((recoilCalculated[j]-measSpec[j]),2)/pow(measSpecerr[j],2);
	}
      if (WeightFactorS!=0)
	flux[i]=flux[i]*exp(ReConsF/WeightFactorS);
      else
	flux[i]=0;
      flux_branch_vec[i][cont]=flux[i];
      
      t.Stop();
      tim=t.RealTime();
      SumT+=tim;
   }

  tim=SumT/(cont);
  dof = ((double)ni-1.0)*sqrt(((double)(nj)-1.0));
  chi2dof = chi2/dof;

  if (cont>=1){
    printf("Iteration No: %5d  ||  Chi2/d.o.f.: %e  ||  Time Counter: %4.1lf sec...\n"
	   ,cont
	   ,chi2dof
	   ,SumT
	   );
  }
  else{
    printf("\n\n\n");
    printf("Progress waiting for first initializing...\n");
    printf("\n\n\n");
  }  
}
//###################################################################################//



//###################################################################################//
void engine_body::doroshenko()
{
  chi2=0; chi2dof=1000;
  for (int m=0;m<=nj;m++)
    {
      BuildRecoil=0;
      for (int l=0;l<ni;l++)
	BuildRecoil+=flux[l]*respMatrix[m][l];
      recoilCalculated[m]=BuildRecoil;
      re_recoil_vec[m][cont]=recoilCalculated[m];
    }
  for (int i=0;i<ni;i++)
    {
      tot=0;
      totRM=0;
      
      t.Start();
      for (int j=0;j<nj;j++)
	{
	  if (recoilCalculated[j]==0)
	    tot+=0;
	  else
	    tot+=respMatrix[j][i]*measSpec[j]/recoilCalculated[j];
	  totRM+=respMatrix[j][i];
	  chi2+=pow((recoilCalculated[j]-measSpec[j]),2)/pow(measSpecerr[j],2);
	}
      if(totRM==0)
	totRM=0.00005;
      flux[i]=(flux[i]/totRM)*tot;
      flux_branch_vec[i][cont]=flux[i];
      t.Stop();
      tim=t.RealTime();
      SumT+=tim;
   }

  tim=SumT/(cont);
  dof = ((double)ni-1.0)*sqrt(((double)(nj)-1.0));
  chi2dof = chi2/dof;

  if (cont>=1){
    printf("Iteration No: %5d  ||  Chi2/d.o.f.: %e  ||  Time Counter: %4.1lf sec...\n"
	   ,cont
	   ,chi2dof
	   ,SumT
	   );
  }
  else{
    printf("\n\n\n");
    printf("Progress waiting for first initializing...\n");
    printf("\n\n\n");
  }  
}
//###################################################################################//
