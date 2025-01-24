#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	/*G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("wimp");*/
	fParticleSource = new G4GeneralParticleSource();

	/*G4ThreeVector pos(0*km, 0*km, 0.);
	G4ThreeVector mom(0., 0., 1.);
	G4double Radius = 6000*km;
	G4double Energy = 100*GeV;

	// Set particle
	fParticleSource->GetCurrentSource()->SetParticleDefinition(particle);
	
	// Set position
	fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisType("Circle");
	fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisShape("Circle");
	fParticleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
	fParticleSource->GetCurrentSource()->GetPosDist()->SetRadius(Radius);

	// Set angular distribution
	fParticleSource->GetCurrentSource()->GetAngDist()->SetAngDistType("direction");
	fParticleSource->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(mom);

	// Set energy
	fParticleSource->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
	fParticleSource->GetCurrentSource()->GetEneDist()->SetMonoEnergy(Energy);

	// Set number of particles per event
	fParticleSource->SetNumberOfParticles(1);*/
}


MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleSource;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{	
	fParticleSource->GeneratePrimaryVertex(anEvent);
}
