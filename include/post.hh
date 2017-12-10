
class post_body{public: void avarage();} post;


void post_body::avarage()
{
  if (((Float_t)(cont)*100)/(Float_t)(IterationNo)<50||((Float_t)(cont)*100)/(Float_t)(IterationNo)>50)
    {
      for (int i=av_t;i<=ni-av_t;i++)
	{
	  totav=0;
	  for (int j=0;j<av;j++)
	    totav+=flux[i+j-av_t]/av;
	  flux[i]=totav;
	}
    }
  for (int i=0;i<=ni;i++)
    flux_branch_vec[i][cont]=flux[i];
}
