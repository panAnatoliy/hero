void ReadSim(TString inputDir = "output_paralell/test1", Int_t NTHR = 1) 
{
//========== Histogram form =========================================
Int_t  Nreaction=0;
Int_t binNumb = 10000;
Double_t minBin = 0.;
Double_t maxBin = 3.;
Double_t binStep = (maxBin - minBin)/binNumb;
cout << "binStep: " << binStep << endl;
TString fileName;
//Namber Branches NUM = 18, old geometry NUM = 9
    Int_t NUM = 2;
    Int_t num =120; 

    TH1F* histo[NUM], *length[NUM] , *Aenerg[NUM];

    for (Int_t i = 0; i < 2; i++) {
    TString histName,lenghthistoName, Aenergy;
    histName.Form("hsito_%i", i);
    lenghthistoName.Form("length_%i", i);
    Aenergy.Form("Aenergy_%i", i);
    histo[i] = new TH1F(histName, histName, binNumb, minBin, maxBin);
    length[i] = new TH1F(lenghthistoName, lenghthistoName, 100, 0, 20.);
    Aenerg[i] = new TH1F(Aenergy, Aenergy, 1000, 0, 10.);
}
//===================================================================
// Int_t chekerNumberOfHistosMemersByPlates[4] = {0, 0, 0, 0 };
    

    // Loop over THR start
for (Int_t THR = 1; THR <= NTHR; THR++) {
    cout << "============ THR: " << THR << " ============" << endl;
    // Reding root file open
    fileName.Form("%s/sim_%d.root", inputDir.Data(), THR);
    TFile* file = new TFile(fileName, "READ");
    if (file->IsZombie()) {
    cerr << "File read error" << endl;
    return;
    }
    // Finding the tree in root file
    TTree* tree = (TTree*)file->Get("HERO");
    if (!tree) {
    cerr << "Tree read error" << endl;
    return;
    }
    // Branches reding
    TBranch* Scint_Br,* Metal_Br,*Br_Track;
    
    int layers=0;
    for(int  i=1; i< num ; i++){
    //TString Scint, Metal;
        Scint_Br = tree->GetBranch(Form("HEROdetectorvScint_%iPoint",i));
        Metal_Br = tree->GetBranch(Form("HEROdetectorvMetal_%iPoint",i));

    if (!Scint_Br) 
        cerr << "Scint i:" << i << " Branche read error." << endl;
    
    if (!Metal_Br) 
        cerr << "Metal i:" << i << " Branche read error." << endl; 
        
    
    Br_Track= tree->GetBranch("MCTrack");
        if (!Br_Track) 
        cerr  << "Br_Track Branche read error." << endl;
    

      TClonesArray* Arr_Scint,* Arr_Metal,*Arr_Track;
    
      Arr_Track=new TClonesArray("HEROMCTrack"); 
      Br_Track->SetAddress(&Arr_Track);
    
      Arr_Scint = new TClonesArray("HEROPoint");
      Scint_Br->SetAddress(&Arr_Scint);
      Arr_Metal = new TClonesArray("HEROPoint");
      Metal_Br->SetAddress(&Arr_Metal);
   
      
    UInt_t nEvents = tree->GetEntries();
//     cout << "Number of events is: " << nEvents << endl;
    UInt_t THRNeutrons = 0;
    UInt_t THRAlphas = 0;
//     HEROMCTrack* Track;
//     TIter Titer(Arr_Track);
    

    // Loop over events start
    for (UInt_t e = 0; e < nEvents; e++) {
//       cout << "Event: " << i << endl;
//       Br_Track->GetEntry(i);
    HEROMCTrack* Track, *ETrack;
    TIter Titer(Arr_Track);
    Br_Track->GetEntry(e);
    // Loop over plates start
//         
        for (int MS=0; MS < 2; MS ++){
            
             TIter * Iter;
//                                                             cout<< "Problem Hear"<<i<<e<<MS<<endl;
                HEROPoint* Point;
            if (MS==0){
                Scint_Br->GetEntry(e);
                Iter=new TIter (Arr_Scint);
            }
            if (MS==0){
                Metal_Br->GetEntry(e);
                Iter=new TIter (Arr_Metal);
            }
            Int_t neutronsNum = 0;
            Int_t alphaNum = 0;

        // Loop over points
            while (((Point = (HEROPoint*)Iter->Next()))) {
            if (Point->GetPID() == 1000020040)
            alphaNum++;
        
            if (Point->GetPID() == 2112)    neutronsNum++;

        // == - It writes neutrons, != - It writes alphas
//           if (Point->GetPID() == 1000020040){
//             continue;
            if (Point->GetPID() == 1000020040){
            Double_t curTimeIn = Point->GetTimeIn();
            Double_t curTimeOut = Point->GetTimeOut();
            Double_t dTime = curTimeOut - curTimeIn;
        //if (dTime < 1e-3) continue;
            Double_t ii = curTimeIn;
//           cout<<curTimeIn<<endl;
        // Loop fill hostograms
//           while (ii <= curTimeOut) {
//             //chekerNumberOfHistosMemersByPlates[j]++;
//             histo[j]->Fill((Float_t)ii);
//             ii += binStep;
//           }
//           histo[j]->Fill(curTimeOut); //This is factual lifetime of neuttrons, regarding the whole prototype!!!
        
            Int_t track_id= Point->GetTrackID();
//             cout <<Point->GetPID()<<endl;
//             cout<<track_id<<endl;
            Track =(HEROMCTrack*)Arr_Track->At(track_id);
            Int_t moth_Id =Track->GetMotherId(); //беру номер трэка родителя нейтрона
            if(moth_Id<0) continue;
            Int_t tpdg =Track->GetPdgCode(); //беру номер трэка родителя нейтрона
            Int_t moth_pdg = ((HEROMCTrack*)Arr_Track->At(moth_Id))->GetPdgCode();
            
//             cout<<tpdg<<"  "<<moth_pdg<<endl;
            Double_t En,Mn,Ea,Ma;
        if(moth_pdg==2112){ Nreaction++; 
            ETrack  = ((HEROMCTrack*)Arr_Track->At(moth_Id));
//             En = ETrack->GetEnergy();
            Mn = ETrack->GetMass();
            histo[1]->Fill(En-Mn);
            ETrack  = ((HEROMCTrack*)Arr_Track->At(track_id));
//              Ea = ETrack->GetEnergy();
            Ma = ETrack->GetMass();
//              cout<<Ea<<endl;
            
        Double_t curPIn = Point-> GetPIn();
        Double_t Ea = sqrt(curPIn*curPIn + Ma*Ma);
                    
//             Aenerg[j]->Fill(1000*(Ea-Ma));
//              cout<<E<<endl;
            }
            /*if(math_pdg ==2112 )*/
        Double_t X_in = Point->GetXIn();                
        Double_t Y_in = Point->GetYIn();
        Double_t Z_in = Point->GetZIn();
        Double_t X_out = Point->GetXOut();
        Double_t Y_out = Point->GetYOut();
        Double_t Z_out = Point->GetZOut();//For length histo !!!
//           Double_t Energy = Track->GetEnergy();
        if (moth_pdg==2112) length[1]->Fill(sqrt((X_out-X_in)*(X_out-X_in)+(Y_out-Y_in)*(Y_out-Y_in)+(Z_out-Z_in)*(Z_out-Z_in))*1e+4);
//             length[j]->Fill(Energy);
//         
        }
        } // loop over points end
//         cout << " neutronsNum: " << neutronsNum << endl;
//         cout << " alphaNum: " << alphaNum << endl;
        THRNeutrons += neutronsNum;
        THRAlphas += alphaNum;
    } // loop over Events end
    }
           Scint_Br -> Clear();
           Arr_Scint->Clear();
           Metal_Br -> Clear();
           Arr_Metal->Clear();
           Arr_Track->Clear();
           layers++;
    }
           cout <<"Layers"<<layers<<endl;
    // Close and clear
//     cout << "THRNeutrons: " << THRNeutrons << endl;
//     cout << "THRAlphas: " << THRAlphas << endl;
    cout << "----------------------------------------" << endl;
cout<<Nreaction<<"  "<<"Nreaction"<<endl;
//     }
    
        file->Close();
} // loop over THR end

for (Int_t i = 0; i < NUM; i++) {
    cout << "Plate: " << i << ", Enries: " <<  endl;
}
TCanvas* canv[NUM],* hcnv[NUM],*hcnvA[NUM] ;
for (Int_t i = 0; i < NUM; i++) {
    TString canvName;
    canvName.Form("canv_%d", i);
    canv[i] = new TCanvas(canvName, canvName, 800, 1000);

    canv[i]->cd();
    length[i]->Draw();
    length[i]->SetLineWidth(2);
    TAxis* xAx = (TAxis*)length[i]->GetXaxis();
    TAxis* yAx = (TAxis*)length[i]->GetYaxis();
    xAx->SetTitle("lentgth, mkm");
    yAx->SetTitle("alpha number");
    yAx->SetTitleSize(0.05);
    gPad->SetFrameLineWidth(5);
    gPad->SetGrid(2, 2);

}
for (Int_t i = 0; i < NUM; i++) {
    TString hcnvN;
    hcnvN = Form("hcnvN%i",i); 
    hcnv[i] = new TCanvas(hcnvN,hcnvN,800, 1000);
    hcnv[i]->cd();
    histo[i]->Draw();
}
for (Int_t i = 0; i < NUM; i++) {
    TString hcnvAe;
    hcnvAe = Form("hcnvAe%i",i); 
    hcnvA[i] = new TCanvas(hcnvAe,hcnvAe,800, 1000);
    hcnvA[i]->cd();
    Aenerg[i]->Draw();
}

TString outFileName = inputDir + "/" + "histo_out.root";

TFile* outFile = new TFile(outFileName, "RECREATE");
for (Int_t i = 0; i < NUM; i++) {
    histo[i]->Write();
    length[i]->Write();
    Aenerg[i]->Write();
}
}
