#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	
	G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }
	
	virtual G4VPhysicalVolume *Construct();
	
private:
	G4LogicalVolume *logicDetector;
	virtual void ConstructSDandField();
	
	G4int nCols, nRows;
	
	G4Box *solidWorld, *solidRadiator, *solidDetector, *solidScintillator, *solidEarth;
	G4LogicalVolume *logicWorld, *logicRadiator, *logicScintillator, *logicEarth;
	G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, *physScintillator, *physEarth;
	
	G4GenericMessenger *fMessenger;
	
	G4LogicalVolume *fScoringVolume;
	
	G4Material *Vakuum, *worldMat, *Kruste, *OMantel, *UMantel, *liquidEisen, *solidEisen;
	
	void DefineMaterial();
	
	void ConstructEarth();
	
	G4double xWorld, yWorld, zWorld;
	
	G4bool isCherenkov, isScintillator, isEarth, isEarth2, isEarth3, isEarth4;
};

#endif
