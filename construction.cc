#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	
	fMessenger->DeclareProperty("isEarth", isEarth, "Toggle Earth setup");
	
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

	Vakuum = new G4Material("Vakuum", 1e-25*g/cm3, 1);
	Vakuum->AddElement(nist->FindOrBuildElement("H"), 1.0);
	
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
		
	liquidEisen = new G4Material("Fluessiges Eisen", 11.0*g/cm3, 1);
	liquidEisen->AddElement(nist->FindOrBuildElement("Fe"), 1);
	
	solidEisen = new G4Material("Festes Eisen", 12.8*g/cm3, 1);
	solidEisen->AddElement(nist->FindOrBuildElement("Fe"), 1);
	
	worldMat = nist ->FindOrBuildMaterial("Vakuum");
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
	
	//Kruste ueber Detektoren
	G4double rkruste_max = 6371*km;
	
	G4Sphere *kruste = new G4Sphere ("kruste", r_min, rkruste_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logickruste = new G4LogicalVolume(kruste, Kruste, "logickruste");
	G4VPhysicalVolume *physkruste = new G4PVPlacement(0, Earthposition, logickruste, "physkruste", logicWorld, false, 0, true);

	//Detektor
	G4double det_depth = 1 * km;
	G4double det_width = 100 * m;
	G4double rdet_max = rkruste_max - det_depth;
	
	G4Sphere *solidDetector = new G4Sphere("solidDetector", r_min, rdet_max, phi_start, phi_delta, theta_start, theta_delta);
	logicDetector = new G4LogicalVolume(solidDetector, Kruste, "logicDetector");
	physDetector = new G4PVPlacement(0, position, logicDetector, "physDetector", logickruste, false, 0, true);

	//Kruste unter Detektoren
	G4double rkruste2_max = rdet_max - det_width;
	
	G4Sphere *kruste2 = new G4Sphere ("kruste2", r_min, rkruste2_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logickruste2 = new G4LogicalVolume(kruste2, Kruste, "logickruste2");
	G4VPhysicalVolume *physkruste2 = new G4PVPlacement(0, position, logickruste2, "physkruste2", logicDetector, false, 0, true);
	
	// Obere Mantel
	G4double rom_max = 6351*km;
	
	G4Sphere *oMantel = new G4Sphere ("oMantel", r_min, rom_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicoMantel = new G4LogicalVolume(oMantel, OMantel, "logicoMantel");
	G4VPhysicalVolume *physoMantel = new G4PVPlacement(0, position, logicoMantel, "physoMantel", logickruste2, false, 0, true);
	
	// Unterer Mantel
	G4double rum_max = 5711*km;
	
	G4Sphere *uMantel = new G4Sphere ("uMantel", r_min, rum_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicuMantel = new G4LogicalVolume(uMantel, UMantel, "logicuMantel");
	G4VPhysicalVolume *physuMantel = new G4PVPlacement(0, position, logicuMantel, "physuMantel", logicoMantel, false, 0, true);
	
	// Fluessiger Kern
	G4double rfl_max = 3471*km;
	
	G4Sphere *fluessigkern = new G4Sphere ("fluessigkern", r_min, rfl_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicfluessigkern = new G4LogicalVolume(fluessigkern, liquidEisen, "logicfluessigkern");
	G4VPhysicalVolume *physfluessigkern = new G4PVPlacement(0, position, logicfluessigkern, "physfluessigkern", logicuMantel, false, 0, true);
	
	
	// Fester Kern
	G4double rfest_max = 1221*km;

	G4Sphere *festerkern = new G4Sphere ("festerkern", r_min, rfest_max, phi_start, phi_delta, theta_start, theta_delta);
	G4LogicalVolume *logicfesterkern = new G4LogicalVolume(festerkern, solidEisen, "logicfesterkern");
	G4VPhysicalVolume *physfesterkern = new G4PVPlacement(0, position, logicfesterkern, "physfesterkern", logicfluessigkern, false, 0, true);
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
