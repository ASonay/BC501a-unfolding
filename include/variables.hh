//KEY PARAMETERS------------------------------------------
int IterationNo=300;    //Number of iteration (MAX)

//SMOOTHING-----------------------------------------------
int av=3;               //Avarage size
int av_t=(av-1)/2;
Double_t totav=0;

//FILE VARIABLES------------------------------------------
FILE *finalFlux;
FILE *flux0File;
FILE *responseFile;
FILE *measurementFile;
Double_t dump;
Double_t dumpRow;

char respFN[200];
char fopw[200];
char base[200];
char iw[200];
char temp[200];
  
int RMBinSize=20;
int StartE=0;
int EndE=20000;

const int bin_i=(EndE-StartE)/RMBinSize;
const int bin_j=1000;

int ni=bin_i;
int nj=bin_j;

Double_t flux[10000];
Double_t fluxi[10000];
Double_t respMatrix[10000][1000];
Double_t measSpec[10000];
Double_t measSpecerr[10000];
Double_t recoilT[10000];
Double_t recoilCalculated[10000];
Double_t energy[10000];

//FILLING VARIABLES---------------------------------------
Double_t flux_branch_vec[10000][1000];
Double_t re_recoil_vec[10000][1000];
Double_t flux_branch[10000];
Double_t re_recoil[10000];
Double_t flux_eng;
Double_t re_recoil_eng;

//METHODE VARIABLES---------------------------------------
int cont=0;
Double_t chi2,chi2dof,dof;
//GRAVEL
Double_t ReConsF=0;
Double_t WeightFactor=0;
Double_t WeightFactorS=0;
Double_t BuildRecoil=0;
//DOROSHENKO
Double_t totRM=0;
Double_t tot=0;

//OTHER VARIABLES-----------------------------------------
Double_t tim=0;
Double_t SumT=0.0;

TStopwatch t; 
TLegend *leg;

TCanvas *c1 = new TCanvas("c1","Unfolding",1460,830);

void BaseFile(const char* fname);
