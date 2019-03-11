
// -------------------------------------------------------------------------
// -----                      OLVMCTrack header file                    -----
// -----                  Created 11/05/15  by V. Schetinin            -----
// -------------------------------------------------------------------------

/** OLVMCTrack.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing Monte Carlo tracks processed by the ERStack.
 ** A MCTrack can be a primary track put into the simulation or a
 ** secondary one produced by the transport through decay or interaction.
 **
 **
 **/


#ifndef OLVMCTRACK_H_
#define OLVMCTRACK_H_

#include "ERDetectorList.h"

#include "TObject.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TParticle.h"
 
enum ExpertTrackingStatus{
    Entering,
    Exiting,
    Inside,
    Stop
};


class TParticle;

class OLVMCTrack : public TObject
{

 public:

  /**  Default constructor  **/
  OLVMCTrack();


  /**  Standard constructor  **/
  OLVMCTrack(Int_t pdgCode, Int_t motherID, Double_t px, Double_t py,
       Double_t pz, Double_t x, Double_t y, Double_t z,
       Double_t t, Int_t nPoints);

  /**  Copy constructor  **/
  OLVMCTrack(const OLVMCTrack& track);


  /**  Constructor from TParticle  **/
  OLVMCTrack(TParticle* particle);


  /**  Destructor  **/
  virtual ~OLVMCTrack();


  /**  Output to screen  **/
  void Print(Int_t iTrack=0) const;

  //Int_t AccNeuRad() const {return GetNPoints(kNEURAD);}
  /**  Accessors  **/

  Int_t    GetPdgCode()  const { return fPdgCode; }
  Int_t    GetMotherId() const { return fMotherId; }
  Double_t GetPx()       const { return fPx; }
  Double_t GetPy()       const { return fPy; }
  Double_t GetPz()       const { return fPz; }
  Double_t GetStartX()   const { return fStartX; }
  Double_t GetStartY()   const { return fStartY; }
  Double_t GetStartZ()   const { return fStartZ; }
  Double_t GetStartT()   const { return fStartT; }
  Double_t GetMass()     const;
  Double_t GetCharge()   const;
  Double_t GetEnergy()   const;
  Double_t GetPt()       const { return TMath::Sqrt(fPx*fPx+fPy*fPy); }
  Double_t GetP()      const { return TMath::Sqrt(fPx*fPx+fPy*fPy+fPz*fPz); }
  Double_t GetRapidity() const;
  void GetMomentum(TVector3& momentum) const;
  void Get4Momentum(TLorentzVector& momentum) const;
  void GetStartVertex(TVector3& vertex) const;
  TLorentzVector GetVector();
  Double_t GetTheta();
  Double_t GetPhi();


  /** Accessors to the number of MCPoints in the detectors **/
  Int_t GetNPoints(DetectorId detId)  const;


  /**  Modifiers  **/
  void SetMotherId(Int_t id) { fMotherId = id; }
  void SetNPoints(Int_t iDet, Int_t np);

private:
  Double_t CalculateMass();
  Double_t CalculateEnergy();

  Int_t fID;

  /**  PDG particle code  **/
  Int_t  fPdgCode;

  /**  Index of mother track. -1 for primary particles.  **/
  Int_t  fMotherId;

  /** Momentum components at start vertex [GeV]  **/
  Double32_t fPx, fPy, fPz;

  /** Coordinates of start vertex [cm, ns]  **/
  Double32_t fStartX, fStartY, fStartZ, fStartT;

  Int_t fNPoints;
  Double_t fMass;
  Double_t fEnergy;
  Double_t fTheta,fPhi;

  TLorentzVector fMomentum;

  ClassDef(OLVMCTrack,2);

};



// ==========   Inline functions   ========================================

inline Double_t OLVMCTrack::GetEnergy() const {
  return fEnergy;
}


inline void OLVMCTrack::GetMomentum(TVector3& momentum) const {
  momentum.SetXYZ(fPx,fPy,fPz);
}


inline void OLVMCTrack::Get4Momentum(TLorentzVector& momentum) const {
  momentum.SetXYZT(fPx,fPy,fPz,fEnergy);
}


inline void OLVMCTrack::GetStartVertex(TVector3& vertex) const {
  vertex.SetXYZ(fStartX,fStartY,fStartZ);
}


#endif /* OLVMCTRACK_H_ */