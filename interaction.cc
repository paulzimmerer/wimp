#include "interaction.hh"

WimpWeakInteraction::WimpWeakInteraction(const G4String& processName)
    : G4VDiscreteProcess(processName) {}

WimpWeakInteraction::~WimpWeakInteraction() {}

G4bool WimpWeakInteraction::IsApplicable(const G4ParticleDefinition& particle) {
    // Nur für WIMPs anwendbar
    
    return particle.GetParticleName() == "wimp";
}
     
const G4double WimpWeakInteraction::molare_Masse(const G4Material* material){

    G4int n_elements = material->GetNumberOfElements();
    G4double mol_ges = 0;
    
    for(int i=0; i < n_elements; i++){
        auto fraction = material->GetFractionVector()[i];
    	auto mol_el = material->GetElement(i)->GetA() / (g/mole) * fraction;
        mol_ges = mol_ges + mol_el;
	}
    return mol_ges;
}


G4double WimpWeakInteraction::GetMeanFreePath(const G4Track& track, G4double, G4ForceCondition*) {
   
    // Hole das Material, in dem sich das Teilchen befindet   
    const G4Material* material = track.GetMaterial();
     
    // Hole die Teilchendichte des Materials
    G4double density = material->GetDensity() / (g/cm3);
    G4double molarMass = molare_Masse(material);
    G4double nAtoms = (density / molarMass) * CLHEP::Avogadro; // Teilchendichte in Atomen/cm^3

    // Berechne den Wirkungsquerschnitt 
    G4double crossSection = 1e-31 * cm2; // 

    // Berechne die mittlere freie Weglänge
    G4double meanFreePath = 1.0 / (nAtoms * crossSection) * 1000;
    
    //G4cout << "Material: " << material<<G4endl;
    /*G4cout << "Avogadro: " << CLHEP::Avogadro<<G4endl;
    G4cout << "Dichte: " << density<<G4endl;
    G4cout << "Molare Masse: " << molarMass<<G4endl;
    G4cout << "Anzahl Atome: " << nAtoms<<G4endl;*/
    //G4cout << "Wirkungsquerschnitt: " << crossSection<<G4endl;
    //G4cout << "Mittlere Freie Weglänge: " << meanFreePath<<G4endl;
    
    return meanFreePath;
}

const G4Element* WimpWeakInteraction::Element(const G4Material* material){
    G4int n_elements = material->GetNumberOfElements();
    G4double random = G4UniformRand();
    //G4cout << "Zufallszahl: " << random << G4endl;
    G4double C = 0;
    
    //G4cout << "Zufallszahl: " << random << G4endl;
    for(int i=0; i < n_elements; i++){
    	auto element = material->GetElement(i);
    	auto fraction = material->GetFractionVector()[i];
    	auto A = element->GetA() / (g/mole);
    	C = C + fraction/A;
    }
    
    for(int i=0; i < n_elements; i++){
    	auto element = material->GetElement(i);
    	auto fraction = material->GetFractionVector()[i];
    	auto A = material->GetElement(i)->GetA() / (g/mole);
    	auto abundance = fraction/A/C;
    	if(random <= abundance){
    	    	//G4cout << "Atomanteil: " << abundance << G4endl;   	
		return element;
	}
	else{
		random = random - abundance;
	}
    }
    return nullptr;
}

G4VParticleChange* WimpWeakInteraction::PostStepDoIt(const G4Track& track, const G4Step& step) {
    //Steplength > 0!
    G4double steplength = track.GetStepLength();
    
    if (steplength == 0){
        aParticleChange.Initialize(track);
        //G4cout << "PostStepDoIt: WIMP interaction NOT triggered" << G4endl;
    	return &aParticleChange;}
    	
    // Definiere, was nach der Wechselwirkung passiert
    aParticleChange.Initialize(track);

    // Isotrope Streuung
    
    G4ThreeVector newdirection = G4RandomDirection();
    
    aParticleChange.ProposeMomentumDirection(newdirection);

    // Ändere die Energie des WIMPs
    G4ThreeVector olddirection = track.GetMomentumDirection();
    G4double angle_l = olddirection.angle(newdirection);
    G4double energy = track.GetKineticEnergy();
    G4double m_particle = MyCustomParticle::Definition()->GetPDGMass();
    
    //G4cout << "PostStepDoIt: WIMP interaction triggered" << G4endl;
    const G4Material* material = track.GetMaterial();
    
    const G4Element* element = Element(material);
    G4double Z = element->GetZ();
    G4double A = element->GetA() / (g/mole);
    G4double m_element = G4NucleiProperties::GetNuclearMass(A,Z);
    G4double B = m_element / m_particle;
    G4double alpha = ((B-1)/(B+1)) * ((B-1)/(B+1));
    
    G4double tanl = std::tan(angle_l);
    
    G4double angle_c = angle_l;
    
    for(G4int i = 0; i <= 100; i++){
        G4double sinc = std::sin(angle_c);
        G4double cosc = std::cos(angle_c);
    	G4double fc = sinc/(cosc+1/B)-tanl;
        G4double dfc = (1+(cosc/B))/((cosc+1/B)*(cosc+1/B));
        angle_c = angle_c - fc/dfc;
    }
	
   /* G4cout << "Element: " << element << G4endl;
    G4cout << "Masse: " << m_element << G4endl;
    G4cout << "Masse: " << m_particle << G4endl;*/
   // G4cout << "Material: " << material << G4endl;
   /* G4cout << "Alte Richtung: " << olddirection << G4endl;
    G4cout << "Neue Richtung: " << newdirection << G4endl;*/
   // G4cout << "Winkel l: " << angle_l << G4endl;
   // G4cout << "Winkel c: " << angle_c << G4endl;
    
    G4double cosc = std::cos(angle_c);
    
    G4double newenergy = energy * (((1+alpha)+(1-alpha)*cosc)/2);
    aParticleChange.ProposeEnergy(newenergy);
    
    return &aParticleChange;
}

