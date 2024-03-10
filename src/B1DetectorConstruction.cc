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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
// Get nist material manager
G4NistManager* nist = G4NistManager::Instance();

//Define element
G4Element* Si = nist->FindOrBuildElement("Si");

// Envelope parameters
//
G4double env_sizeXY = 45*cm, env_sizeZ = 240*cm;
// G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

// Option to switch on/off checking of volumes overlaps
//
G4bool checkOverlaps = true;

//
// World
//
G4double world_sizeXY = 50*cm;
G4double world_sizeZ  = 250*cm;
G4double layer_sizeX = 10*cm;
G4double layer_sizeY = 20*cm;
G4double layer_sizeZ = 0.1*mm;
G4double layer_dis = 30*cm;

G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");
G4Material* layer_mat = new G4Material("layer_mat",2.2*g/cm3,1);
layer_mat->AddElement(Si,1);

G4Box* solidWorld =
new G4Box("World",                       //its name
0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

G4LogicalVolume* logicWorld =
new G4LogicalVolume(solidWorld,          //its solid
world_mat,           //its material
"World");            //its name

G4VPhysicalVolume* physWorld =
new G4PVPlacement(0,                     //no rotation
G4ThreeVector(),       //at (0,0,0)
logicWorld,            //its logical volume
"World",               //its name
0,                     //its mother  volume
false,                 //no boolean operation
0,                     //copy number
checkOverlaps);        //overlaps checking

//
// Envelope
//
G4Box* solidEnv =
new G4Box("Envelope",                    //its name
0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size

G4LogicalVolume* logicEnv =
new G4LogicalVolume(solidEnv,            //its solid
env_mat,             //its material
"Envelope");         //its name

new G4PVPlacement(0,                       //no rotation
G4ThreeVector(0,0,0),         //at (0,0,0)
logicEnv,                //its logical volume
"Layers",              //its name
logicWorld,              //its mother  volume
false,                   //no boolean operation
0,                       //copy number
checkOverlaps);          //overlaps checking

//
// Layers
//
G4Box* solidLay =
new G4Box("Layers",                    //its name
0.5*layer_sizeX, 0.5*layer_sizeY, 0.5*layer_sizeZ); //its size

G4LogicalVolume* logicLay =
new G4LogicalVolume(solidLay,            //its solid
layer_mat,             //its material
"Layers");         //its name

for (int i=0;i<7;i++) {
new G4PVPlacement(0,                       //no rotation
G4ThreeVector(0,0,-90*cm+i*layer_dis),         //at (0,0,0)
logicLay,                //its logical volume
"Layers",              //its name
logicEnv,              //its mother  volume
false,                   //no boolean operation
i,                       //copy number
checkOverlaps);          //overlaps checking
}


fScoringVolume = logicLay;

//
//always return the physical World
//
return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......