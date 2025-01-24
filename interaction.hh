#ifndef INTERACTION_HH
#define INTERACTION_HH

#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleChange.hh"
#include "G4StepPoint.hh"
#include "G4DynamicParticle.hh"
#include "G4RandomTools.hh"
#include "G4Element.hh"
#include "G4NucleiProperties.hh"
#include "particle.hh"
#include "G4RandomDirection.hh"

class WimpWeakInteraction : public G4VDiscreteProcess {
public:

	WimpWeakInteraction(const G4String& processName = "WimpWeakInteraction");
	~WimpWeakInteraction() override;

    // Hauptmethoden
	G4bool IsApplicable(const G4ParticleDefinition& particle) override;
	G4double GetMeanFreePath(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition) override;
	G4VParticleChange* PostStepDoIt(const G4Track& track, const G4Step& step) override;
	
    //Element und molare Masse bestimmen
        virtual const G4Element* Element(const G4Material* material);
        virtual const G4double molare_Masse(const G4Material* material);
};

#endif

