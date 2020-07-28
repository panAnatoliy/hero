void Geo_Spherical_Detector()
{
    Double_t R_min = 0.;
    Double_t R_max = 125.;
    Double_t dz = 150.;
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
    
    FairGeoMedium    * SCint  = geoMedia->getMedium("FscScintVB");
    if ( ! SCint ) Fatal("Main", "FairMedium FscScintVB not found");
    geoBuild->createMedium(SCint);
    TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
    if ( ! Scint ) Fatal("Main", "Medium FscScintVB not found");
    
    
    // Shapes and volumes ---------------------------------------------------------------
    
    TGeoVolume* vSphere = gGeoManager->MakeSphere("vSphere", Scint, R_min, R_max, 0.,180.,0.,0.);
    
    // Container for 1 detector
    TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");
    
    // Volume hierarchy -----------------------------------------------------------------
    vDetContainer->AddNode(vSphere, 1);
    
    top->AddNode(vDetContainer, 1);
    
    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->CheckGeometry();
    gGeoManager->CheckGeometryFull();
    
    gGeoManager->GetTopVolume()->Draw("ogl");
    
    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    // --------------------------------------------------------------------------
}

