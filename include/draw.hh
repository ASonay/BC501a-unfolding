
//DRAW OPERATION--------------------

class draw_body
{
public:
  void unfolding();
  void fill_data();
  TGraph *gflux, *gflux_init, *gcalc;
  TGraphErrors *gmeas;
} draw;


//###################################################################################//
void draw_body::unfolding()
{
  c1->Modified();
  
  c1->cd(1);
  gPad->SetTicky();
  gPad->SetTickx();

  gflux = new TGraph(ni,energy,flux);
  gflux->SetTitle("");
  gflux->GetXaxis()->SetTitle("Energy (MeV)");
  gflux->GetYaxis()->SetTitle("Normalized Fluence");
  gflux->SetLineWidth(6);
  gflux->SetLineColor(kRed-4);
  gflux->SetLineStyle(9);
  gflux->Draw("al");
  gflux_init = new TGraph(ni,energy,fluxi);
  gflux_init->SetTitle();
  gflux_init->SetLineWidth(6);
  gflux_init->SetLineColor(kGray+3);
  gflux_init->SetLineStyle(1);
  gflux_init->Draw("same l");
  gflux->Draw("same l");
	  
  leg = new TLegend(0.349013,0.754599,0.92218,0.915783);
  leg->SetFillStyle(0);
  leg->SetLineWidth(0);
  leg->AddEntry(gflux,"Unfolded spectra","l");
  leg->AddEntry(gflux_init,"Reference","l");
  leg->Draw();
  
  c1->cd(2);
  gPad->SetTicky();
  gPad->SetTickx();
  
  gmeas = new TGraphErrors(nj,recoilT,measSpec,0,measSpecerr);
  gmeas->SetTitle("");
  gmeas->SetLineWidth(3);
  gmeas->SetLineColor(kGray+3);
  gmeas->SetMarkerStyle(20);
  gmeas->SetMarkerColor(kAzure-3);
  gmeas->GetXaxis()->SetTitle("Light Output (MeV_{ee})");
  gmeas->GetYaxis()->SetTitle("Normalized Count (MeV^{-1})");
  gmeas->Draw("ap");
  gcalc = new TGraph(nj,recoilT,recoilCalculated);
  gcalc->SetTitle();
  gcalc->SetLineWidth(6);
  gcalc->SetLineColor(kRed-4);
  gcalc->SetLineStyle(9);
  gcalc->Draw("same l");
  
  leg = new TLegend(0.249013,0.754599,0.92218,0.915783);
  leg->SetFillStyle(0);
  leg->SetLineWidth(0);
  leg->AddEntry(gmeas,"Folded recoil spectra","ep");
  leg->AddEntry(gcalc,"Iterative fit","l");
  leg->Draw();
  c1->Update();
}
//###################################################################################//



//###################################################################################//
void draw_body::fill_data()
{
  TFile *f = new TFile("./output/output_data.root", "recreate");
  TTree *tr_unfold= new TTree("tr_unfold","unfolded data");
  TTree *tr_recoil= new TTree("tr_recoil","recoil data");
  tr_unfold->Branch("flux_eng",&flux_eng,"flux_eng/D");
  tr_recoil->Branch("re_recoil_eng",&re_recoil_eng,"re_recoil_eng/D");
  tr_unfold->Branch("flux_branch",flux_branch,"flux_branch[1000]/D");
  tr_recoil->Branch("re_recoil",re_recoil,"re_recoil[1000]/D");
  for (int i=0;i<=ni;i++)
    {
      flux_eng=(float)(i*RMBinSize)/1000;
      for (int j=0;j<IterationNo;j++)
	flux_branch[j]=flux_branch_vec[i][j];
      tr_unfold->Fill();
    }

  for (int i=0;i<=nj;i++)
    {
      re_recoil_eng=recoilT[i];
      for (int j=0;j<IterationNo;j++)
	re_recoil[j]=re_recoil_vec[i][j];
      tr_recoil->Fill();
    }
  
  f->Write();
}
//###################################################################################//
