#include "particle.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

MyCustomParticle* MyCustomParticle::theWIMP = nullptr;
// Singleton-Instanz
MyCustomParticle* MyCustomParticle::Instance() {
    return theWIMP;
}

// Definition des Partikels
MyCustomParticle* MyCustomParticle::Definition() {
    if(!theWIMP) {
        theWIMP = new MyCustomParticle();
        G4cout << "New WIMP created" << G4endl;
    }
    return theWIMP;
}

// Konstruktor
MyCustomParticle::MyCustomParticle() 
    : G4ParticleDefinition("wimp"/*Name*/, 1.0*GeV/*Masse*/, 0*GeV/*Ladung*/, .0/*Spin*/, 1/*Parität*/, 1, 1, 1, 1, 1, "boson", 143, 0, 9900015, false,-1, nullptr, false, "wimpModel",0,0) {

    SetProcessManager(new G4ProcessManager(this));
}
// Destruktor
MyCustomParticle::~MyCustomParticle() {}
