void Geo_Spherical_Detector()
{
    Double_t Scint_dz = 0.9;
    Double_t Metal_dz = 0.1;
    Int_t Num_Layers = 120.;
    
    // Create a zero rotation
    TGeoRotation* ggZeroRotation = new TGeoRotation("ggZeroRotation");
    
    // -------   Load media from media file   -----------------------------------
    FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/media.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    // --------------------------------------------------------------------------
    
    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/HERO_detector_Sphear.geo.root";
    // --------------------------------------------------------------------------
    
    // --------------   Create geometry and top volume  -------------------------
    gGeoManager = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoManager->SetName("DETgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoManager->SetTopVolume(top);
    // --------------------------------------------------------------------------
    
    // -----------------   Get and create the required media    -----------------
    FairGeoMedia*   geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
    
    FairGeoMedium    * air  = geoMedia->getMedium("air");
    if ( ! air  ) Fatal("Main", "FairMedium air   not found");
    geoBuild->createMedium(air);
    TGeoMedium* Air = gGeoManager->GetMedium("air");
    if ( !  Air ) Fatal("Main", "Medium  Air not found");
    
    FairGeoMedium    * SCint  = geoMedia->getMedium("FscScintVB");
    if ( ! SCint ) Fatal("Main", "FairMedium FscScintVB not found");
    geoBuild->createMedium(SCint);
    TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
    if ( ! Scint ) Fatal("Main", "Medium FscScintVB not found");
    
    FairGeoMedium    * Aluminum  = geoMedia->getMedium("aluminium");
    if ( ! Aluminum ) Fatal("Main", "FairMedium Aluminum not found");
    geoBuild->createMedium(Aluminum);
    TGeoMedium* aluminum = gGeoManager->GetMedium("aluminium");
    if ( ! aluminum ) Fatal("Main", "Medium aluminum not found");
    
    TGeoVolume* vAir_Sphere = gGeoManager->MakeSphere("vAirSphere", Air, 0, 125, 0.,180.,0.,0.);
    
    TGeoVolume* vScint_Sphere_[Num_Layers], *vMetal_Sphere_[Num_Layers]; 
    // Shapes and volumes ---------------------------------------------------------------
    for(int i=1 ; i< Num_Layers ; i++){
        int j=i-1;
        vScint_Sphere_[i] = gGeoManager->MakeSphere(Form("vScint_%i", i), Scint, 126-i*(Scint_dz+Metal_dz)-Scint_dz, 126-i*(Scint_dz+Metal_dz), 0.,180.,0.,0.);
        vScint_Sphere_[i] ->SetFillColor(kGreen);
        vScint_Sphere_[i] ->SetLineColor(kGreen);
        vScint_Sphere_[i] ->SetTransparency(60);
        vAir_Sphere ->AddNode(vScint_Sphere_[i],1);
        
        vMetal_Sphere_[i] = gGeoManager->MakeSphere(Form("vMetal_%i", i), Scint,125-i*(Scint_dz + Metal_dz), 126-i*(Scint_dz+Metal_dz)-Scint_dz, 0.,180.,0.,0.);
        vMetal_Sphere_[i] ->SetFillColor(kRed);
        vMetal_Sphere_[i] ->SetLineColor(kRed);
        vMetal_Sphere_[i] ->SetTransparency(60);
        vAir_Sphere ->AddNode(vMetal_Sphere_[i],1);
        
    }
    // Container for 1 detector
    TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");
    
    // Volume hierarchy -----------------------------------------------------------------
    vDetContainer->AddNode(vAir_Sphere, 1);
    
    top->AddNode(vDetContainer, 1);
    
    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->CheckGeometry();
//     gGeoManager->CheckGeometryFull();
    
    gGeoManager->GetTopVolume()->Draw("ogl");
    
    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    // --------------------------------------------------------------------------
}
