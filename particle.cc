#include "particle.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

// Singleton-Instanz
MyCustomParticle* MyCustomParticle::Instance() {
    static MyCustomParticle instance;
    return &instance;
}

// Definition des Partikels
MyCustomParticle* MyCustomParticle::Definition() {
    return Instance();
}

// Konstruktor
MyCustomParticle::MyCustomParticle() 
    : G4ParticleDefinition("wimp"/*Name*/, 100*MeV/*Masse*/, 0*GeV/*Ladung*/, .0/*Spin*/, 1/*Parität*/, 1, 1, 1, 1, 1, "lepton", 143, 0, 9900015, false,-1, nullptr, false, "wimpModel",0,0) {

    SetProcessManager(new G4ProcessManager(this));
}
// Destruktor
MyCustomParticle::~MyCustomParticle() {}
