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
/// \file B3/B3a/src/DetectorConstruction.cc
/// \brief Implementation of the B3::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

namespace B3
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  
  G4double sphere_r = 7.5*cm;
  G4double sizeXYZ = 30*cm;

  G4Isotope* U5 = new G4Isotope ("U235", 92, 235, 235.01*g/mole);
  G4Isotope* U8 = new G4Isotope ("U238", 92, 238, 238.03*g/mole);

  G4Element* elU = new G4Element ("enriched Uranium", "U", 2);
  elU->AddIsotope (U5, 90. *perCent); 
  elU->AddIsotope (U8, 10. *perCent);

  G4Material* U = new G4Material("Uranium", 19.1*g/cm3, 1);
  U->AddElement(elU, 1);

  G4NistManager* nist = G4NistManager::Instance();
  //G4Material* default_mat = nist->FindOrBuildMaterial("Uranium");
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
////World
  auto solidWorld = new G4Sphere("World", 0, sphere_r, 0, twopi, 0, pi);

  auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");

  auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, fCheckOverlaps);

////Sphere
  auto solid_bomb = new G4Sphere("bomb", 0, sphere_r, 0, twopi, 0, pi);

  auto logic_bomb = new G4LogicalVolume(solid_bomb, U, "bomb");
 
  auto phys_bomb = new G4PVPlacement(nullptr, G4ThreeVector(), logic_bomb, "bomb", logicWorld, false, 0, fCheckOverlaps);

  // Visualization attributes
  //
  //logic_bomb->SetVisAttributes (G4VisAttributes::GetInvisible());
  logicWorld->SetVisAttributes (G4VisAttributes::GetInvisible());
  G4VisAttributes *spherevis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.2));
  logic_bomb->SetVisAttributes(spherevis);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


}

