
//FILE OPERATION--------------------

class file_body{public: void read_RM(int bin); void read_recoil(const char* fopr); void read_guess(const char* fguess);} file;

//----------------------------------
bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

//###################################################################################//
void file_body::read_RM(int bin)
{

  double th = recoilT[0] - (bin/2.0)/1000.0;
  int step = bin/2;
  int min = (double)(th*1000.0+1)/(double)bin;
  int max = nj;
  double rme=0,rmc=0;
 
  printf("Response matrix bin size change from 2 -> %i\n",bin);
  printf("Threshold is set of as %4.2lf MeVee\n",th);
  printf("Response matrix is reading with the dimension of:  %d x %d\n\n",ni,nj);
  for (int i=0;i<ni;i++)
    {
      energy[i] = i*RMBinSize;
      sprintf (respFN,"ResponseMatrix/%dkeV.dat",(Int_t)energy[i]);
      responseFile = fopen(respFN,"r");
      for(int k=0;k<ni*20.0/bin;k++){
	rmc=0;
	for (int j=0;j<step;j++){
	  fscanf(responseFile,"%lf %lf\n",&dump,&rme);
	  if (dump>=th)
	    if (rme>=1.0)
	      rmc+=rme;
	}
	if (k>=min){
	  respMatrix[k-min][i]=rmc*1.0e-05*2.0/bin;
	}
      }
      fclose(responseFile);
      energy[i] /= 1000;
    }
}
//###################################################################################//



//###################################################################################//
void file_body::read_recoil(const char* fopr)
{
  if (!is_file_exist(fopr))
    {
      printf("\nYour recoil spectrum file doesn't exist!!!\n\n");
      exit(0);
    }
  
  measurementFile = fopen(fopr,"r");

  nj=0;
  while (!feof(measurementFile)){
    fscanf(measurementFile,"%lf %lf %lf\n",&recoilT[nj],&measSpec[nj],&measSpecerr[nj]);
    nj++;
  }
  fclose(measurementFile);
  printf("\nRecoil spectrum reading has done for %i line.\n\n",nj);
}
//###################################################################################//



//###################################################################################//
void file_body::read_guess(const char* fguess)
{
  if(!strcmp(fguess,"")){
    for(int i=0;i<ni;i++){
      fluxi[i]=1.0; flux[i]=fluxi[i];
    }
  }
  else{
    if (!is_file_exist(fguess)){
      printf("Your initial flux doesn't exist!! Straight line will be use instead of it.\n\n");
      for(int i=0;i<ni;i++){
	fluxi[i]=1.0; flux[i]=fluxi[i];
      }
    }
    else{
      flux0File = fopen(fguess,"r");
      for(int i=0;i<ni;i++)
	{
	  fscanf(flux0File,"%lf %lf\n",&dumpRow,&fluxi[i]);
	  if (fluxi[i]<=0)
	    fluxi[i]=1.0e-04;
	  flux[i]=fluxi[i];
	}
      fclose(flux0File);
    }
  }
}
//###################################################################################//
