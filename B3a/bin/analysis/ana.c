void ana(){
    TCanvas *c = new TCanvas;
    
    TFile *f = new TFile("../total.root", "READ");
    TDirectory *dir = (TDirectory*)f->Get("histo");
    TH1D *hist = (TH1D*)dir->Get("Globaltime");

    TF1 *fit = new TF1("fit", "[0]*exp([1]*x)", 0, 400);

    hist->GetXaxis()->SetTitle("Globaltime(ns)");
    hist->GetYaxis()->SetTitle("Counts");
    hist->Fit(fit);
    hist->Draw();
}