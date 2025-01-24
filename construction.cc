#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	
	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
	fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup");
	fMessenger->DeclareProperty("isEarth", isEarth, "Toggle Earth setup");
	
	nCols = 10;
	nRows = 10;
	
	DefineMaterial();
	
	xWorld = 10000*km;
	yWorld = 10000*km;
	zWorld = 15000*km;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	Kruste = new G4Material("Kruste", 2.7*g/cm3, 6);
	Kruste->AddElement(nist->FindOrBuildElement("O"), 48.7*perCent);
	Kruste->AddElement(nist->FindOrBuildElement("Si"), 29.7*perCent);
	Kruste->AddElement(nist->FindOrBuildElement("Al"), 8.5*perCent);
	Kruste->AddElement(nist->FindOrBuildElement("Fe"), 6.4*perCent);
	Kruste->AddElement(nist->FindOrBuildElement("Mg"), 4.2*perCent);
	Kruste->AddElement(nist->FindOrBuildElement("Ca"), 2.5*perCent);
	
	OMantel = new G4Material("OMantel", 3.5*g/cm3, 4);
	OMantel->AddElement(nist->FindOrBuildElement("O"), 46.8*perCent);
	OMantel->AddElement(nist->FindOrBuildElement("Mg"), 24.5*perCent);
	OMantel->AddElement(nist->FindOrBuildElement("Si"), 22.3*perCent);
	OMantel->AddElement(nist->FindOrBuildElement("Fe"), 6.4*perCent);
	
	UMantel = new G4Material("UMantel", 4.5*g/cm3, 4);
	UMantel->AddElement(nist->FindOrBuildElement("O"), 47.3*perCent);
	UMantel->AddElement(nist->FindOrBuildElement("Mg"), 23.6*perCent);
	UMantel->AddElement(nist->FindOrBuildElement("Si"), 22.6*perCent);
	UMantel->AddElement(nist->FindOrBuildElement("Fe"), 6.5*perCent);
		
	G4Material *liquidEisen = new G4Material("Flüssiges Eisen", 11.0*g/cm3, 1);
	liquidEisen->AddElement(nist->FindOrBuildElement("Fe"), 1);
	
	solidEisen = new G4Material("Festes Eisen", 12.8*g/cm3, 1);
	solidEisen->AddElement(nist->FindOrBuildElement("Fe"), 1);
	
	SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
	
	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);
	
	C = nist->FindOrBuildElement("C");
	
	Aerogel = new G4Material ("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H2O, 37.4*perCent);
	Aerogel->AddElement(C, 0.1*perCent);
	
	worldMat = nist ->FindOrBuildMaterial("G4_AIR");
	
	G4double energy[2] = {1.239841939*eV/0.4, 1.239841939*eV/0.2};
	G4double rindexAerogel[2] = {1.1, 1.1};
	G4double rindexWorld[2] = {1.0, 1.0};
	G4double rindexNaI[2] = {1.78, 1.78};
	G4double reflectivity[2] = {1.0, 1.0};
		
	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
	
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
	
	Aerogel->SetMaterialPropertiesTable(mptAerogel);
	
	Na = nist->FindOrBuildElement("Na");
	I = nist->FindOrBuildElement("I");
	NaI = new G4Material("NaI", 3.67*g/cm3, 2);
	NaI->AddElement(Na, 1);
	NaI->AddElement(I, 1);
	
	G4double fraction[2] = {1.0, 1.0};
	
	G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
	mptNaI->AddProperty("RINDEX", energy, rindexNaI, 2);
	mptNaI->AddProperty("SCINTILLATIONCOMPONENT1", energy, fraction, 2);
	mptNaI->AddConstProperty("SCINTILLATIONYIELD", 38./keV);
	mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.);
	mptNaI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250*ns);
	mptNaI->AddConstProperty("SCINTILLATIONYIELD1", 1.);
	
	NaI->SetMaterialPropertiesTable(mptNaI);
	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	mirrorSurface = new G4OpticalSurface("mirrorSurface");
	
	mirrorSurface->SetType(dielectric_metal);
	mirrorSurface->SetFinish(ground);
	mirrorSurface->SetModel(unified);
	
	G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
	
	mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
	
	mirrorSurface->SetMaterialPropertiesTable(mptMirror);
}



void MyDetectorConstruction::ConstructEarth()
{	
	G4double r_min = 0*km;
	G4double phi_start = 0*deg;
	G4double phi_delta = 360*deg;
	G4double theta_start = 0*deg;
	G4double theta_delta = 180*deg;
	G4ThreeVector Earthposition = G4ThreeVector(0,0,7000*km);
	G4ThreeVector position = G4ThreeVector(0,0,0);
	
	//Kruste über Detektoren
	G4double rkruste_min = 0*km;
	G4double rkruste_max = 6371*km;
	
	G4Sphere *kruste = new G4Sphere ("kruste", rkruste_min, rkruste_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logickruste = new G4LogicalVolume(kruste, Kruste, "logickruste");
	G4VPhysicalVolume *physkruste = new G4PVPlacement(0, Earthposition, logickruste, "physkruste", logicWorld, false, 0, true);

	//Detektor
	G4double det_depth = 1 * km;
	G4double det_width = 100 * m;
	G4double rdet_max = rkruste_max - det_depth;
	
	G4Sphere *solidDetector = new G4Sphere("solidDetector", r_min, rdet_max, phi_start, phi_delta, theta_start, theta_delta);
	logicDetector = new G4LogicalVolume(solidDetector, Kruste, "logicDetector");
	physDetector = new G4PVPlacement(0, position, logicDetector, "physDetector", logickruste, false, 1, true);

	//Kruste unter Detektoren
	G4double rkruste2_min = 0*m;
	G4double rkruste2_max = rdet_max - det_width;
	
	G4Sphere *kruste2 = new G4Sphere ("kruste2", rkruste2_min, rkruste2_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logickruste2 = new G4LogicalVolume(kruste2, Kruste, "logickruste2");
	G4VPhysicalVolume *physkruste2 = new G4PVPlacement(0, position, logickruste2, "physkruste2", logicDetector, false, 0, true);
	
	// Obere Mantel
	G4double rom_min = 0*km;
	G4double rom_max = 6351*km;
	
	G4Sphere *oMantel = new G4Sphere ("oMantel", rom_min, rom_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicoMantel = new G4LogicalVolume(oMantel, OMantel, "logicoMantel");
	G4VPhysicalVolume *physoMantel = new G4PVPlacement(0, position, logicoMantel, "physoMantel", logickruste2, false, 0, true);
	
	// Unterer Mantel
	G4double rum_min = 0*km;
	G4double rum_max = 5711*km;
	
	G4Sphere *uMantel = new G4Sphere ("uMantel", rum_min, rum_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicuMantel = new G4LogicalVolume(uMantel, solidEisen, "logicuMantel");
	G4VPhysicalVolume *physuMantel = new G4PVPlacement(0, position, logicuMantel, "physuMantel", logicoMantel, false, 0, true);
	
	// Fester Kern
	G4double rfest_min = 0*km;
	G4double rfest_max = 3471*km;
	
	G4Sphere *festerkern = new G4Sphere ("kern", rfest_min, rfest_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicfesterkern = new G4LogicalVolume(festerkern, solidEisen, "logicfesterkern");
	G4VPhysicalVolume *physfesterkern = new G4PVPlacement(0, position, logicfesterkern, "physfesterkern", logicuMantel, false, 0, true);
	
	// Flüssiger Kern
	G4double rfl_min = 0.0*km;
	G4double rfl_max = 1221*km;
	
	G4Sphere *flüssigkern = new G4Sphere ("flüssigkern", rfl_min, rfl_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicflüssigkern = new G4LogicalVolume(flüssigkern, solidEisen, "logicflüssigkern");
	G4VPhysicalVolume *physflüssigkern = new G4PVPlacement(0, position, logicflüssigkern, "physflüssigkern", logicfesterkern, false, 0, true);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{	
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	ConstructEarth();
	
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
	if(logicDetector != NULL)
		logicDetector->SetSensitiveDetector(sensDet);
}
