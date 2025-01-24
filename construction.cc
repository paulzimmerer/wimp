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
	
	xWorld = 20000*km;
	yWorld = 20000*km;
	zWorld = 20000*km;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	Granit = new G4Material("Granit", 2.7*g/cm3, 2);
	Granit->AddElement(nist->FindOrBuildElement("O"), 1);
	Granit->AddElement(nist->FindOrBuildElement("Si"), 1);
	
	Peridotit = new G4Material("Peridotit", 4.0*g/cm3, 2);
	Peridotit->AddElement(nist->FindOrBuildElement("O"), 1);
	Peridotit->AddElement(nist->FindOrBuildElement("Si"), 1);
	
	G4Material *liquidEisen = new G4Material("Flüssiges Eisen", 12.0*g/cm3, 1);
	liquidEisen->AddElement(nist->FindOrBuildElement("Fe"), 1);
	
	solidEisen = new G4Material("Festes Eisen", 13.0*g/cm3, 1);
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



void MyDetectorConstruction::ConstructEarth4()
{	
	G4double n_phi = 1;
	G4double n_theta = 2;
	G4double r_min = 6371*km;
	G4double r_max = 6371.0000000001*km;
	G4double phi_delta = 360/n_phi*deg;
	G4double theta_delta = 180/n_theta*deg;
	
	G4ThreeVector position = G4ThreeVector(0,0,7000*km);
	
	for(G4int i=0; i<n_phi; i++) {
	
		for(G4int j=0; j<n_theta; j++) {
		
			G4double phi_start = i*phi_delta;
			G4double theta_start = j*theta_delta;
	
			G4Sphere *solidDetector = new G4Sphere("kugel", r_min, r_max, phi_start, phi_delta, theta_start, theta_delta);

			logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
			
			MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
			if(logicDetector != NULL)
				logicDetector->SetSensitiveDetector(sensDet);
			
			physDetector = new G4PVPlacement(0, position, logicDetector, "physDetector", logicWorld, false, i*n_theta+j, true);
		}
	}
	
	G4double rü_min = 5711*m;
	G4double rae_max = 6371*km;
	G4double phi_start = 0*deg;
	G4double phi_delta = 360*deg;
	G4double theta_start = 0*deg;
	G4double thetaä_delta = 180*deg;
	
	G4Sphere *äußere_Schale = new G4Sphere ("äußere_Schale", rä_min, rä_max, phi_start, phiä_delta, theta_start, thetaä_delta);
	G4LogicalVolume *logicäußereSchale = new G4LogicalVolume(äußere_Schale, Granit, "logicäußereSchale");
	G4VPhysicalVolume *physäußereSchale = new G4PVPlacement(0, position, logicäußereSchale, "physäußereSchale", logicWorld, false, 0, true);
	
	G4double ri_min = 3471*km;
	G4double ri_max = 5711*km;
	
	G4Sphere *innere_Schale = new G4Sphere ("innere_Schale", ri_min, ri_max, phi_start, phiä_delta, theta_start, thetaä_delta);
	G4LogicalVolume *logicinnereSchale = new G4LogicalVolume(innere_Schale, Peridotit, "logicinnereSchale");
	G4VPhysicalVolume *physinnereSchale = new G4PVPlacement(0, position, logicinnereSchale, "physinnereSchale", logicWorld, false, 0, true);
	
	G4double rk_min = 0.0*km;
	G4double rk_max = 3471*m;
	
	G4Sphere *kern = new G4Sphere ("kern", rk_min, rk_max, phi_start, phiä_delta, theta_start, thetaä_delta);
	G4LogicalVolume *logickern = new G4LogicalVolume(kern, solidEisen, "logickerne");
	G4VPhysicalVolume *physkern = new G4PVPlacement(0, position, logickern, "physkerne", logicWorld, false, 0, true);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{	
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	ConstructEarth4();
	
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
	if(logicDetector != NULL)
		logicDetector->SetSensitiveDetector(sensDet);
}
