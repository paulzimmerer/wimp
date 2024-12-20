#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

class MyCustomParticle : public G4ParticleDefinition {
public:
    static MyCustomParticle* Definition();
    static MyCustomParticle* Instance();
    
private:
    MyCustomParticle();
    ~MyCustomParticle();
};

#endif

