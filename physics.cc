#include "physics.hh"
#include "particle.hh"
#include "interaction.hh"

MyPhysicsList::MyPhysicsList()
{
	//RegisterPhysics (new G4EmStandardPhysics());
	//RegisterPhysics (new G4OpticalPhysics());
	//RegisterPhysics (new G4DecayPhysics());
	//RegisterPhysics (new G4RadioactiveDecayPhysics());	

}

MyPhysicsList::~MyPhysicsList()
{}

void MyPhysicsList::ConstructParticle()
{
  MyCustomParticle::Definition();
}

void MyPhysicsList::ConstructProcess()
{
  AddTransportation();

  G4ParticleDefinition* particle = MyCustomParticle::Definition();
  G4ProcessManager* pmanager = particle->GetProcessManager();
  if(!pmanager)
  	G4cout << "No pmanager" << G4endl;
  auto weakInteraction = new WimpWeakInteraction();
  pmanager->AddDiscreteProcess(weakInteraction);
  		//pmanager->SetProcessOrderingToFirst(weakInteraction, idxPostStep);
     	//}
  if (pmanager) {
  		G4cout << "Registering WimpWeakInteraction for particle: " 
     		<< particle->GetParticleName() << G4endl;
  }
  auto processList = pmanager->GetProcessList();
  if (processList) {
      G4cout << "Processes registered for particle: " 
  	   << particle->GetParticleName() << G4endl;
      for (size_t i = 0; i < processList->size(); ++i) {
  	G4cout << " - " << (*processList)[i]->GetProcessName() << G4endl;
      }
  }

}