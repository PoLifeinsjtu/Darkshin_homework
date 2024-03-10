//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

const G4double c = 300.;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
fRunAction(runAction),
fEdep(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{
fEdep = 0.;
fpos.clear();
ftime.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{
G4double x1, y1, z1, x2, y2, z2, dis, timdif, v, P, P_avr;
// accumulate statistics in run action
fRunAction->AddEdep(fEdep);
P_avr = 0.;
for (int i = 0; i<fpos.size()-1; i++) {
x1 = fpos.at(i)[0];
y1 = fpos.at(i)[1];
z1 = fpos.at(i)[2];
x2 = fpos.at(i+1)[0];
y2 = fpos.at(i+1)[1];
z2 = fpos.at(i+1)[2];
dis = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
timdif = (ftime.at(i+1)-ftime.at(i));
v = dis/timdif;
// printf("********* v: %0.2f mm/ns \n",v);
P = 938.*MeV*v/sqrt(1-v*v/(c*c))/(c*c);
P_avr += P;
// v:  "<<dis/timdif<<std::endl;
}
P_avr /= (fpos.size() - 1);
P_avr /= (MeV/c);
// printf("********* P: %0.2f MeV/c \n",P_avr);
fRunAction->PRecord(P_avr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......