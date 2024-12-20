#include "interaction.hh"

WimpWeakInteraction::WimpWeakInteraction(const G4String& processName)
    : G4VDiscreteProcess(processName) {}

WimpWeakInteraction::~WimpWeakInteraction() {}

G4bool WimpWeakInteraction::IsApplicable(const G4ParticleDefinition& particle) {
    // Nur für WIMPs anwendbar
    
    return particle.GetParticleName() == "wimp";
}

G4double WimpWeakInteraction::GetMeanFreePath(const G4Track& track, G4double, G4ForceCondition*) {
    // Beispiel: Setze die mittlere freie Weglänge
    return 1*mm;
}

G4VParticleChange* WimpWeakInteraction::PostStepDoIt(const G4Track& track, const G4Step& step) {
    // Definiere, was nach der Wechselwirkung passiert
    aParticleChange.Initialize(track);

    // Beispiel: Ändere die Energie des WIMPs
    G4double energyLoss = 10 * MeV;
    aParticleChange.ProposeEnergy(track.GetKineticEnergy() - energyLoss);

    // Beispiel: Streuung um einen kleinen Winkel
    G4ThreeVector direction = track.GetMomentumDirection();
    direction.rotateY(G4UniformRand()*50*deg); // Streuung
    aParticleChange.ProposeMomentumDirection(direction);

    G4cout << "PostStepDoIt: WIMP interaction triggered" << G4endl;

    return &aParticleChange;
}

