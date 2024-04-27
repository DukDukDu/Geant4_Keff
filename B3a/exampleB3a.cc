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
/// \file exampleB3a.cc
/// \brief Main program of the B3a example

#include "G4Types.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4AnalysisManager.hh"
#include "G4TScoreNtupleWriter.hh"
#include "QGSP_BIC_HP.hh"

#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned long GetSeedFromTime()
{
  auto now = std::chrono::system_clock::now();
  auto generator = CLHEP::HepRandom::getTheGenerator();
  auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() \
  %static_cast<int64_t> (1e16 * generator->flat());
  return seed;
}



int main(int argc,char** argv)
{ 
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv);}

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  //Set Seed
  unsigned long seed0 = GetSeedFromTime();
  unsigned long seed = GetSeedFromTime();
  for(int i=0; i<10; i++){

    seed = GetSeedFromTime();
    printf("###### test the randlom seed to %ldln###",seed);
  }
  if(seed0 == seed) {
    printf("stopped since the seed is same as seed0=%ld\n",seed0);
    return 0;
  }
  else{
    printf("####### set the random seed to %ld\n########",seed);
    CLHEP::HepRandom::setTheSeed(seed);
    G4Random::setTheSeed(seed);
    //gRandom->SetSeed(seed);
  }
  //
  G4RunManager *runManager = new G4RunManager;

  runManager->SetUserInitialization(new B3::DetectorConstruction);
  auto physicsList = new QGSP_BIC_HP;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // Set user action initialization
  //
  runManager->SetUserInitialization(new B3a::ActionInitialization());

  // Initialize visualization
  //
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // auto visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
  scoreNtupleWriter.SetVerboseLevel(1);
  scoreNtupleWriter.SetNtupleMerging(true);
  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
