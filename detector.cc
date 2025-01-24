#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack();
	/*G4int trackID = track->GetTrackID();

    	// Prüfen, ob dieses Teilchen bereits einen Hit ausgelöst hat
    	if (trackedParticles.find(trackID) != trackedParticles.end())
	    {
		return false; // Ignoriere diesen Hit
	    }
	        
	trackedParticles.insert(trackID);*/

	
	//track->SetTrackStatus(fStopAndKill);
	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	G4ThreeVector posWIMP = preStepPoint->GetPosition();
	G4ThreeVector momWIMP = preStepPoint->GetMomentum();
	G4ThreeVector dirWIMP = preStepPoint->GetMomentumDirection();
		
	G4double kinEnergy = track->GetKineticEnergy();
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	
	G4int copyNo = touchable->GetCopyNumber();
	
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	
	#ifndef G4MULTITHREADED
	G4cout << "WIMP position: " << posWIMP << G4endl;
	G4cout << "Copy number: " << copyNo << G4endl;
	G4cout << "Detector position: " << posDetector << G4endl;
	G4cout << "Energie: " << momWIMP.mag() << G4endl;
	G4cout << "KinEnergie: " << kinEnergy << G4endl;
	G4cout << "Richtung: " << dirWIMP << G4endl;
	#endif
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4cout << "Event: " << evt + 1 << G4endl;
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->FillNtupleIColumn(0, 0, evt);
	man->FillNtupleDColumn(0, 1, kinEnergy);
	man->FillNtupleDColumn(0, 2, dirWIMP[0]);
	man->FillNtupleDColumn(0, 3, dirWIMP[1]);
	man->FillNtupleDColumn(0, 4, dirWIMP[2]);
	man->AddNtupleRow(0);
	
	man->FillNtupleIColumn(1, 0, evt);
	man->FillNtupleDColumn(1, 1, kinEnergy);
	man->FillNtupleDColumn(1, 2, posWIMP[0]);
	man->FillNtupleDColumn(1, 3, posWIMP[1]);
	man->FillNtupleDColumn(1, 4, posWIMP[2]);
	man->AddNtupleRow(1);
	
	//man->FillNtupleIColumn(3, 0, evt);
	//man->FillNtupleDColumn(3, 1, kinEnergy);
	//man->AddNtupleRow(3);

	/*man->FillNtupleIColumn(1, 0, evt);
	man->FillNtupleDColumn(1, 1, posDetector[0]);
	man->FillNtupleDColumn(1, 2, posDetector[1]);
	man->FillNtupleDColumn(1, 3, posDetector[2]);
	man->AddNtupleRow(1);*/

	return true;
}
