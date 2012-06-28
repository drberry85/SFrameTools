// Dear emacs, this is -*- c++ -*-
#ifndef EventCalc_H
#define EventCalc_H

#include "ObjectHandler.h"
#include "SLogger.h"
#include "TVector3.h"
#include "Utils.h"

/**
 *  @short class for the calculation of basic event variables
 *
 * 
 *
 *  @author Roman Kogler
 */

class EventCalc
{
 private:
  static EventCalc* m_instance;

 public:
  static EventCalc* Instance();

  void Reset();
  
  BaseCycleContainer* GetBaseCycleContainer();
  LuminosityHandler* GetLumiHandler();

  int GetRunNum() {return (m_bcc ? m_bcc->run : -1);}
  int GetLumiBlock() {return (m_bcc ? m_bcc->luminosityBlock : -1);}
  int GetEventNum() {return (m_bcc ? m_bcc->event : -1);}
  bool IsRealData() {return (m_bcc ? m_bcc->isRealData : false);}
  //bool GetHBHENoiseFilterResult() {return (m_bcc ? m_bcc->HBHENoiseFilterResult : false);}

  float GetBeamSpotX0() {return (m_bcc ? m_bcc->beamspot_x0 : -999.);}
  float GetBeamSpotY0() {return (m_bcc ? m_bcc->beamspot_y0 : -999.);}
  float GetBeamSpotZ0() {return (m_bcc ? m_bcc->beamspot_z0 : -999.);}
  
  std::vector< Electron >* GetElectrons() {return (m_bcc ? m_bcc->electrons : NULL);}
  std::vector< Muon >* GetMuons() {return (m_bcc ? m_bcc->muons : NULL);}
  std::vector< Tau >* GetTaus() {return (m_bcc ? m_bcc->taus : NULL);}
  std::vector< Photon >* GetPhotons() {return (m_bcc ? m_bcc->photons : NULL);}
  std::vector< PrimaryVertex >* GetPrimaryVertices() {return (m_bcc ? m_bcc->pvs : NULL);}

  std::vector< Jet >* GetJets() { return (m_bcc ? m_bcc->jets : NULL);}
  std::vector< TopJet >* GetCAJets() {return (m_bcc ? m_bcc->topjets : NULL);}
  std::vector< TopJet >* GetPrunedCAJets() {return (m_bcc ? m_bcc->prunedjets : NULL);}
  std::vector< GenParticle >* GetGenParticles() {return (m_bcc ? m_bcc->genparticles : NULL);}
  MET* GetMET() {return (m_bcc ? m_bcc->met : NULL);}

  GenInfo* GetGenInfo() {return (m_bcc ? m_bcc->genInfo : NULL);}
 
  std::vector<std::string>* GetTrigNames() {return (m_bcc ? m_bcc->triggerNames : NULL);}
  std::vector<bool>* GetTrigResults() {return (m_bcc ? m_bcc->triggerResults : NULL);}

  std::vector<std::string> GetTrigNamesActualRun() {return (m_bcc ? m_bcc->triggerNames_actualrun : std::vector<std::string>());}

  std::vector< ReconstructionHypothesis >* GetRecoHyps(){ return (m_bcc ? m_bcc->recoHyps : NULL);}
  
  /// scalar sum of the pt of all jets, leptons and missing transverse energy
  double GetHT();

  /// scalar sum of missing transverse energy and the pt of all leptons in the actual BaseCycleContainer
  double GetHTlep();

  /// electron or muon in the actual BaseCycleContainer with largest transverse momentum
  Particle* GetPrimaryLepton();

  /**
   * @short function to calculate the neutrino four-momentum from MET and charged lepton momenta
   *
   * Given the Decay:
   *
   * A -> B + Neutrino
   *
   * reconstruct the Neutrino pZ component of the Lorentz Vector given
   * it's Energy, px, and py are known.
   *
   * Calculation is carried with formula:
   *
   * P4A^2 = (P4B + P4Neutrino)^2
   *
   * assuming:
   *
   * P4Neutrino^2 = 0
   * P4B^2 = 0
   *
   * within the SM: m_neutrino = 0, mass of the second decay product
   * is neglected due to expected small mass, e.g. in case of the
   * electron: m_B = 0.5 MeV, for the muon: m_mu = 105 MeV and mass
   * of the W-boson: m_W = 80 GeV. m_B is used in formula in form:
   *
   * m_A^2 - m_B^2
   *
   * and therefore m_B can be neglected.
   *
   * The final equation is:
   *
   * (-pTlep^2) * x^2 + 2 * (mu * pZlep) * x + (mu^2 - Elep^2 * pTnu^2) = 0
   *
   * where
   * x is pz_nu
   * mu = mW^2 / 2 + pTlep * pTnu * cos(phi)
   * phi is angle between p_lepton and p_neutrino in transverse plane
   *
   */
  std::vector<LorentzVector> NeutrinoReconstruction(const LorentzVector lepton, const LorentzVector met);

  void FillHighMassTTbarHypotheses();

  /// print a list of all objects in the actual BaseCycleContainer
  void PrintEventContent();

private:
  mutable SLogger m_logger;
  EventCalc();
  ~EventCalc();

  // same as for ObjectHandler, to make things easier
  BaseCycleContainer* m_bcc;
  LuminosityHandler* m_lumi;

  // booleans to tell weather quantities have already been derived in an event
  bool b_HT;
  bool b_HTlep;

  // data members to store calculated results
  double m_HT;
  double m_HTlep;

  Particle* m_primlep;

};


#endif // EventCalc_H
