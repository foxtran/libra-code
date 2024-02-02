/*********************************************************************************
* Copyright (C) 2018-2022 Alexey V. Akimov
*
* This file is distributed under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
* See the file LICENSE in the root directory of this distribution
* or <http://www.gnu.org/licenses/>.
*
*********************************************************************************/
/**
  \file dyn_decoherence_methods.cpp
  \brief The file implements various decoherence correction methods
    
*/

#include "../nhamiltonian/libnhamiltonian.h"
#include "Surface_Hopping.h"
#include "Energy_and_Forces.h"
#include "dyn_hop_proposal.h"
#include "dyn_hop_acceptance.h"
#include "../math_specialfunctions/libspecialfunctions.h"
#include "../math_meigen/libmeigen.h"

/// liblibra namespace
namespace liblibra{

/// libdyn namespace
namespace libdyn{


CMATRIX sdm(CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates, double tol){
    /**
    \brief Generic framework of Simplified Decay of Mixing (SDM) method of 
    Granucci, G.; Persico, M. J. Chem. Phys. 2007, 126, 134114
    
    \param[in]       Coeff [ CMATRIX(nadi, 1) ] An object containig electronic DOFs. 
    \param[in]          dt [ float ] The integration timestep. Units = a.u. of time
    \param[in]      act_st [ integer ] The active state index
    \param[in]      decoh_rates [ MATRIX ] The matrix of decoherence (pure dephasing) rates between all pairs of states
    \param[in]         tol [double] The maximal acceptable deviation of the p_aa_old from 1. If the p_aa_old < 1.0 + tol, then renormalize it to 1.0 
    C [ CMATRIX ] - the updated state of the electronic DOF, in the same data type as the input

    */

    double sclf;


    CMATRIX C(Coeff);


    // Population of the active state
    double p_aa_old = (std::conj(C.get(act_st)) * C.get(act_st)).real();

    if(p_aa_old>1.0 + tol){
      // Comment this place if you want to allow inprecise integration
      // (where the total norm may exceeed 1.0), as is the case for too large dt
      // for some algorithms
      cout<<"=== Place 1 =====\n";
      cout<<"WARNING in CMATRIX sdm(CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates):\n";
      cout<<"The population of the active state is larger than 1: p_aa_old = "<< p_aa_old << endl;
      cout<<"C = \n"; C.show_matrix();
      cout<<"act_st = "<<act_st<<endl;
      cout<<"Coeff = \n"; Coeff.show_matrix();
      cout<<"decoh_rates = \n"; decoh_rates.show_matrix();
      cout<<"initial total pop = "<<(Coeff.H() * Coeff).get(0,0).real();

      C *= (1.0/sqrt(p_aa_old));
      p_aa_old = 1.0;
      //exit(0);
    }


    if(p_aa_old>0.0){

      // First - update all the coefficients for the non-active states        
      int N = Coeff.n_elts;

      double inact_st_pop = 0.0; // population of the inactive states after rescaling

      for(int i=0; i<N; i++){
        if(i != act_st){
          double itau = decoh_rates.get(i, act_st); 
          sclf = exp(-dt*itau);
          C.scale(i, 0, sclf);

          inact_st_pop += (std::conj(C.get(i)) * C.get(i)).real();
        }
      }

      if(inact_st_pop>1.0){
        cout<<"=== Place 2 =====\n";
        cout<<"Error in CMATRIX sdm(CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates):\n";
        cout<<"Total population of inactive states after rescaling is larger than 1: inact_st_pop = "<<inact_st_pop<<endl;
        cout<<"C = \n"; C.show_matrix();
        cout<<"act_st = "<<act_st<<endl;
        cout<<"Coeff = \n"; Coeff.show_matrix();
        cout<<"decoh_rates = \n"; decoh_rates.show_matrix();
        cout<<"initial total pop = "<<(Coeff.H() * Coeff).get(0,0).real();
        exit(0);
      }

      double p_aa_new = 1.0 - inact_st_pop;

     
      if(p_aa_new<0.0){
        cout<<"=== Place 3 =====\n";
        cout<<"Error in CMATRIX sdm(CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates):\n";
        cout<<"new population of the active state "<< p_aa_new <<" is negative...\n";
        cout<<"inact_st_pop = "<<inact_st_pop<<endl;
        cout<<"p_aa_old = "<<p_aa_old<<endl;
        cout<<"C = \n"; C.show_matrix();
        cout<<"act_st = "<<act_st<<endl;
        cout<<"Coeff = \n"; Coeff.show_matrix();
        cout<<"decoh_rates = \n"; decoh_rates.show_matrix();     
        cout<<"initial total pop = "<<(Coeff.H() * Coeff).get(0,0).real();
        exit(0);
      }

      sclf = sqrt( p_aa_new / p_aa_old );  // scaling factor for the active state
        
      // Rescale the active state
      C.scale(act_st, 0, sclf);

    }// if p_aa_old > 0.0

    double new_norm = (C.H() * C).get(0,0).real();
    //cout<<"new_norm = "<<new_norm<<endl;


    if(fabs(new_norm-1.0)>0.1){
      cout<<"=== Place 4 =====\n";
      cout<<"Error in CMATRIX sdm(CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates):\n";
    //  cout<<"new population of the active state "<< p_aa_new <<" is negative...\n";
    //  cout<<"inact_st_pop = "<<inact_st_pop<<endl;
      cout<<"p_aa_old = "<<p_aa_old<<endl;
      cout<<"C = \n"; C.show_matrix();
      cout<<"act_st = "<<act_st<<endl;
      cout<<"Coeff = \n"; Coeff.show_matrix();
      cout<<"decoh_rates = \n"; decoh_rates.show_matrix();     
      cout<<"initial total pop = "<<(Coeff.H() * Coeff).get(0,0).real();
      exit(0);
    }


    return C;

}

CMATRIX sdm(CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates){

  double tol = 0.0;

  return sdm(Coeff, dt, act_st, decoh_rates, tol);
}



CMATRIX sdm(CMATRIX& Coeff, double dt, vector<int>& act_st, vector<MATRIX>& decoh_rates, double tol, int isNBRA){
    /**
    \brief The generic framework of the Simplified Decay of Mixing (SDM) method of
    Granucci, G.; Persico, M. J. Chem. Phys. 2007, 126, 134114)

    This is a version for multiple trajectories
    
    \param[in]       Coeff [ CMATRIX(nadi, ntraj) ] An object containig electronic DOFs. 
    \param[in]          dt [ float ] The integration timestep. Units = a.u. of time
    \param[in]      act_st [ integer ] The active state index
    \param[in]      decoh_rates [ MATRIX ] The matrix of decoherence (pure dephasing) rates between all pairs of states
    \param[in]         tol [double] The maximal acceptable deviation of the p_aa_old from 1. If the p_aa_old < 1.0 + tol, then renormalize it to 1.0 
    \param[in]      isNBRA [integer] If this flag is set to 1, then the Hamiltonian related properties are only computed for one of the trajectories.

    The function returns:
    # C [ CMATRIX ] - the updated state of the electronic DOF, in the same data type as the input

    */

  int nadi = Coeff.n_rows;
  int ntraj = Coeff.n_cols;
  int i, traj;


  vector<int> stenc_x(nadi, 0); for(i=0;i<nadi;i++){  stenc_x[i] = i; }
  vector<int> stenc_y(1, 0); 

  CMATRIX coeff(nadi, 1);
  CMATRIX res(nadi, ntraj);

  for(traj=0; traj<ntraj; traj++){
    int indx = traj;
    if(isNBRA==1){ indx = 0; }
    
    stenc_y[0] = traj;
    pop_submatrix(Coeff, coeff, stenc_x, stenc_y);
    coeff = sdm(coeff, dt, act_st[traj], decoh_rates[indx], tol);
    push_submatrix(res, coeff, stenc_x, stenc_y);

  }// for traj

  return res;

}

CMATRIX sdm(CMATRIX& Coeff, double dt, vector<int>& act_st, vector<MATRIX>& decoh_rates, double tol){
  int is_nbra = 0;
  return sdm(Coeff, dt, act_st, decoh_rates, tol, is_nbra);

}

CMATRIX sdm(CMATRIX& Coeff, double dt, vector<int>& act_st, vector<MATRIX>& decoh_rates){
  double tol = 0.0; 
  int is_nbra = 0;
  return sdm(Coeff, dt, act_st, decoh_rates, tol, is_nbra);

}



void project_out(CMATRIX& Coeff, int traj, int i){
    /** Projects the state `i` out of a coherent superposition of states

    Args:
        Coeff ( CMATRIX(nstates, ntraj) ): amplitudes of the electronic states in
            a coherent superposition from which we will project a state out
        traj ( int ): The index of the trajectory (column) on which we will operate
        i ( int ): The index of the state to be projected out

    Returns:
        None: but changes the input variable `Coeff`

   */ 

    int nstates = Coeff.n_rows;
    
    complex<double> ci; ci = Coeff.get(i,traj);
    double pi; pi = (std::conj(ci) * ci).real();
    double nrm = 1.0 - pi;

    if(nrm<=0.0){ nrm = 0.0; }
    if(nrm>0.0){  nrm = 1.0/sqrt(nrm); }

    Coeff.scale(-1, traj, nrm);
    Coeff.set(i, traj, complex<double>(0.0, 0.0));
}


void collapse(CMATRIX& Coeff, int traj, int i, int collapse_option){
    /** Collapse the wfc but such that to preserve the phase!

    Args:
        Coeff ( CMATRIX(nstates, ntraj) ): amplitudes of the electronic states in
            a coherent superposition which we are going to collapse
        traj ( int ): The index of the trajectory (column) on which we will operate
        i ( int ): The index of the state onto which the supeposition will be collapsed
        collapse_option ( int ): the way to collapse onto given state:
        
          - 0: by rescaling the magnitude of the amplitude vector elements, but preserving "phase"
          - 1: by resetting the amplitudes to 1.0+0.0j. This option changes phase

    Returns:
        None: but changes the input variable `Coeff`

    */ 

    complex<double> ci; ci = Coeff.get(i,traj);
    double pi; pi = (std::conj(ci) * ci).real();

    Coeff.scale(-1, traj, 0.0);

    if(collapse_option==0){
      if(pi>0.0){   Coeff.set(i, traj, ci/sqrt(pi)); }     
      else{  Coeff.set(i, traj, complex<double>(1.0, 0.0) ); }
    }
    else if(collapse_option==1){
      Coeff.set(i, traj, complex<double>(1.0, 0.0) );
    }

}


void collapse_dm(CMATRIX* dm, int i){
    /** Collapse the density matrix to a given state 

    Args:
        dm ( CMATRIX(nstates, nstates) ): density matrix to be modified
        i ( int ): The index of the state onto which the density matrix will be collapsed

    Returns:
        None: but changes the input variable `dm`

    */

    dm->set(-1, -1, complex<double>(0.0, 0.0));
    dm->set(i,i, complex<double>(1.0, 0.0));

}


void instantaneous_decoherence(CMATRIX& Coeff, 
   vector<int>& accepted_states, vector<int>& proposed_states, vector<int>& initial_states,
   int instantaneous_decoherence_variant, int collapse_option){

/**
  This function implements the instantaneous decoherence approach of Nelson et al.:

  Nelson, T.; Fernandez-Alberti, S.; Roitberg, A. E.; Tretiak, S. J. Chem. Phys. 2013, 138, 224111

  Two options of the algorithm are available:

  ID-S: wavefunction amplitudes are collapsed only during the successful hops (onto the new state)

  ID-A: wavefunction amplitudes are collapsed at every attempted hop
      to the new state, if successful
      to the old state, if now

  There collapsing options are controlled by the parameter instantaneous_decoherence_variant

   0 - ID-S
   1 - ID-A
   2 - ID-C - consistent ID - an experimental algo

   In the "consistent" version, we lift the condition that the accepted/proposed states must be different from
   the starting state - this option addresses a philosophical question - what if we say that no hops 
   is equivalent to the hop onto the current state? why should the "hopping" into the original state 
   by treated differently from the "actual hopping" to another state? So, in this version we 
   collapse onto the accpeted states, no matter if they are the result of a successfull or frustrated hop.

  The mechanism of the collapse event itself is controlled by the collapse_option parameter
*/
  int traj;
  int ntraj = Coeff.n_cols;

  if(accepted_states.size()!=ntraj){
    cout<<"ERROR in ids: the sizes of the input variables Coeff and accepted_states are inconsistent\n";
    cout<<"Coeff.num_of_cols = = "<<ntraj<<"\n";
    cout<<"accepted_states.size() = "<<accepted_states.size()<<"\n";
    cout<<"exiting...\n";
    exit(0);
  }
  if(proposed_states.size()!=ntraj){
    cout<<"ERROR in ids: the sizes of the input variables Coeff and proposed_states are inconsistent\n";
    cout<<"Coeff.num_of_cols = = "<<ntraj<<"\n";
    cout<<"proposed_states.size() = "<<proposed_states.size()<<"\n";
    cout<<"exiting...\n";
    exit(0);
  }
  if(initial_states.size()!=ntraj){
    cout<<"ERROR in ids: the sizes of the input variables Coeff and initial_states are inconsistent\n";
    cout<<"Coeff.num_of_cols = = "<<ntraj<<"\n";
    cout<<"initial_states.size() = "<<initial_states.size()<<"\n";
    cout<<"exiting...\n";
    exit(0);
  }


  // ID-S
  if(instantaneous_decoherence_variant==0){

    for(traj = 0; traj < ntraj; traj++){

      if(accepted_states[traj] != initial_states[traj]){
        collapse(Coeff, traj, accepted_states[traj], collapse_option);
      }
    }// traj

  }// ID-S

  // ID-A
  else if(instantaneous_decoherence_variant==1){
    for(traj = 0; traj < ntraj; traj++){
      if(proposed_states[traj] != initial_states[traj]){
        // Only apply ID-A, if the proposed states are different from the original ones

        if(accepted_states[traj] == proposed_states[traj]){
          // Proposed hop is successful - collapse onto newly accepted state
          collapse(Coeff, traj, accepted_states[traj], collapse_option);
        }
        else{
          // Proposed hop is not successful - collapse onto the original state
          collapse(Coeff, traj, initial_states[traj], collapse_option);
        }
      }
    }// traj
  }// ID-A

  // ID-C
  else if(instantaneous_decoherence_variant==2){

    for(traj = 0; traj < ntraj; traj++){
      collapse(Coeff, traj, accepted_states[traj], collapse_option);
    }// traj

  }// ID-C

  // ID-A, new version = IDN
  else if(instantaneous_decoherence_variant==3){
    for(traj = 0; traj < ntraj; traj++){
      if(proposed_states[traj] != initial_states[traj]){
        // Only apply ID-A, if the proposed states are different from the original ones

        if(accepted_states[traj] == proposed_states[traj]){
          // Proposed hop is successful - collapse onto newly accepted state
          collapse(Coeff, traj, accepted_states[traj], collapse_option);
        }
        else{
          // Proposed hop is not successful - project out the proposed state
          project_out(Coeff, traj, proposed_states[traj]);
        }
      }
    }// traj
  }// IDN

  // ID-F, ID on the failed transition, experimental
  else if(instantaneous_decoherence_variant==4){
    for(traj = 0; traj < ntraj; traj++){
      if(proposed_states[traj] != initial_states[traj]){
        // Only apply ID-F, if the proposed states are different from the original ones

        if(accepted_states[traj] != proposed_states[traj]){
          // Proposed hop is rejected - project out the proposed states, but don't collapse
          // onto the accepted hop
          project_out(Coeff, traj, proposed_states[traj]);
        }
      }
    }// traj
  }// IDF

}


void instantaneous_decoherence_dia(CMATRIX& Coeff, nHamiltonian& ham,
   vector<int>& accepted_states, vector<int>& proposed_states, vector<int>& initial_states,
   int instantaneous_decoherence_variant, int collapse_option){

/**
  This is an experimental ID function which does all the same as the normal one does,
  but in the diabatic basis:

  Args: 
    Coeff - the adiabatic amplitudes
    ham - nHamiltonian object
 
  Two options of the algorithm are available:
  ID-S: wavefunction amplitudes are collapsed only during the successful hops (onto the new state)
  ID-A: wavefunction amplitudes are collapsed at every attempted hop
      to the new state, if successful
      to the old state, if now

  There collapsing options are controlled by the parameter instantaneous_decoherence_variant

   0 - ID-S
   1 - ID-A
   2 - ID-C - consistent ID - an experimental algo

   In the "consistent" version, we lift the condition that the accepted/proposed states must be different from
   the starting state - this option addresses a philosophical question - what if we say that no hops
   is equivalent to the hop onto the current state? why should the "hopping" into the original state
   by treated differently from the "actual hopping" to another state? So, in this version we
   collapse onto the accpeted states, no matter if they are the result of a successfull or frustrated hop.

  The mechanism of the collapse event itself is controlled by the collapse_option parameter
*/
  int traj;
  int ntraj = Coeff.n_cols;

  if(accepted_states.size()!=ntraj){
    cout<<"ERROR in ids: the sizes of the input variables Coeff and accepted_states are inconsistent\n";
    cout<<"Coeff.num_of_cols = = "<<ntraj<<"\n";
    cout<<"accepted_states.size() = "<<accepted_states.size()<<"\n";
    cout<<"exiting...\n";
    exit(0);
  }
  if(proposed_states.size()!=ntraj){
    cout<<"ERROR in ids: the sizes of the input variables Coeff and proposed_states are inconsistent\n";
    cout<<"Coeff.num_of_cols = = "<<ntraj<<"\n";
    cout<<"proposed_states.size() = "<<proposed_states.size()<<"\n";
    cout<<"exiting...\n";
    exit(0);
  }
  if(initial_states.size()!=ntraj){
    cout<<"ERROR in ids: the sizes of the input variables Coeff and initial_states are inconsistent\n";
    cout<<"Coeff.num_of_cols = = "<<ntraj<<"\n";
    cout<<"initial_states.size() = "<<initial_states.size()<<"\n";
    cout<<"exiting...\n";
    exit(0);
  }

  // ID-S
  if(instantaneous_decoherence_variant==0){

    for(traj = 0; traj < ntraj; traj++){

      if(accepted_states[traj] != initial_states[traj]){
        collapse(Coeff, traj, accepted_states[traj], collapse_option);
      }
    }// traj

  }// ID-S

  // ID-A
  else if(instantaneous_decoherence_variant==1){

    for(traj = 0; traj < ntraj; traj++){

      if(proposed_states[traj] != initial_states[traj]){
        // Only apply ID-A, if the proposed states are different from the original ones

        if(accepted_states[traj] == proposed_states[traj]){
          // Proposed hop is successful - collapse onto newly accepted state
          collapse(Coeff, traj, accepted_states[traj], collapse_option);
        }
        else{
          // Proposed hop is not successful - collapse onto the original state
          collapse(Coeff, traj, initial_states[traj], collapse_option);
        }
      }
    }// traj

  }// ID-A

  // ID-C
  else if(instantaneous_decoherence_variant==2){

    for(traj = 0; traj < ntraj; traj++){
      collapse(Coeff, traj, accepted_states[traj], collapse_option);
    }// traj

  }// ID-C


}


CMATRIX afssh_dzdt(CMATRIX& dz, CMATRIX& Hvib, CMATRIX& F, CMATRIX& C, double mass, int act_state){
/**
  Right-hand side of Eqs. 15-16 in  J. Chem. Theory Comput. 2016, 12, 5256

       (  dR    0  )
  dz = (           )
       (   0   dP  )


    Args:
        dz is a block-diagonal matrix of dR and dP, which are:
          dR ( CMATRIX(nstates, nstates) ): moment matrix for position, for just 1 DOF and 1 trajectory
          dP ( CMATRIX(nstates, nstates) ): moment matrix for momentum, for just 1 DOF and 1 trajectory
        Hvib ( CMATRIX(nstates, nstates) ): vibronic Hamiltonian matrix, for 1 trajectory
        F ( CMATRIX(nstates, nstates) ): diagonal matrix of forces for all states, for a given DOF and for 1 trajectory
        C ( CMATRIX(nstates, 1) ): amplitudes of electronic states, for 1 trajectory
        mass ( double ): mass of the given DOF
        act_state ( int ): index of the currently active state for given trajectory

    Returns:
        None: dz/dt  matrix of the same dimension and structure as dz

*/

  int i;  

  complex<double> one(0.0, 1.0);
  int nst = dz.n_rows/2;

  vector<int> t1(nst); for(i=0; i<nst; i++){ t1[i] = i; }
  vector<int> t2(nst); for(i=0; i<nst; i++){ t2[i] = nst + i; }
  

  
  CMATRIX res(2*nst, 2*nst);
  CMATRIX tmp(nst, nst);
  CMATRIX dR(nst, nst);
  CMATRIX dP(nst, nst);

  // Unpack dz
  pop_submatrix(dz, dR, t1);
  pop_submatrix(dz, dP, t2);


  // dR/dt
  tmp = -one*(Hvib * dR - dR * Hvib) - dP/mass; 
  push_submatrix(res, tmp, t1);


  // dP/dt
  CMATRIX dF(nst, nst);
  CMATRIX id(nst, nst); id.identity();
  dF = F - id * F.get(act_state, act_state);

  CMATRIX sigma(nst, nst);
  sigma = C * C.H();
  
  tmp = -one*(Hvib * dP - dP * Hvib) - 0.5*(dF * sigma + sigma * dF); 
  push_submatrix(res, tmp, t2);

  
  return res;

}

void integrate_afssh_moments(CMATRIX& dR, CMATRIX& dP, CMATRIX& Hvib, CMATRIX& F, CMATRIX& C, double mass, int act_state, double dt, int nsteps){
/**

   RK4 integration for 1 DOF and 1 trajectory but the matrices are for all states

    Args:
        dR ( CMATRIX(nstates, nstates) ): moment matrix for position, for just 1 DOF and 1 trajectory
        dP ( CMATRIX(nstates, nstates) ): moment matrix for momentum, for just 1 DOF and 1 trajectory
        Hvib ( CMATRIX(nstates, nstates) ): vibronic Hamiltonian matrix, for 1 trajectory
        F ( CMATRIX(nstates, nstates) ): diagonal matrix of forces for all states, for a given DOF and for 1 trajectory
        C ( CMATRIX(nstates, 1) ): amplitudes of electronic states, for 1 trajectory
        mass ( double ): mass of the given DOF
        act_state ( int ): active electronic state index
        dt ( double ): integration timestep
        nsteps ( int ): how many integration steps to take

    Returns:
        None: but changes the input variables `dR` and `dP`

*/

  int i;  

  int nst = dR.n_rows;

  vector<int> t1(nst); for(i=0; i<nst; i++){ t1[i] = i; }
  vector<int> t2(nst); for(i=0; i<nst; i++){ t2[i] = nst + i; }

  CMATRIX der1(2*nst, 2*nst);
  CMATRIX der2(2*nst, 2*nst);
  CMATRIX der3(2*nst, 2*nst);
  CMATRIX der4(2*nst, 2*nst);
  CMATRIX dz(2*nst, 2*nst);
  CMATRIX tmp(2*nst, 2*nst);

  push_submatrix(dz, dR, t1);  
  push_submatrix(dz, dP, t2);  
  
  for(int istep=0; istep<nsteps; istep++){

    // Call the Python function with such arguments
    der1 = afssh_dzdt(dz, Hvib, F, C, mass, act_state);

    tmp = dz + 0.5*dt*der1;
    der2 = afssh_dzdt(tmp, Hvib, F, C, mass, act_state);

    tmp = dz + 0.5*dt*der2;
    der3 = afssh_dzdt(tmp, Hvib, F, C, mass, act_state);

    tmp = dz + dt*der3;
    der4 = afssh_dzdt(tmp, Hvib, F, C, mass, act_state);

    dz = dz + (dt/6.0)*(der1 + 2.0*der2 + 2.0*der3 + der4);
 
  }// for istep

  pop_submatrix(dz, dR, t1);
  pop_submatrix(dz, dP, t2);


}



//MATRIX wp_reversal_events(MATRIX& p, MATRIX& invM, vector<int>& act_states, 
//                          nHamiltonian& ham, vector<CMATRIX>& projectors, double dt){
void wp_reversal_events(dyn_variables& dyn_var, nHamiltonian& ham, double dt){
/**
   This function computes the flags to decide on whether WP on any given surface of every trajectory
   reflects (1) or not (0).

   This is according to:
   Xu, J.; Wang, L. "Branching corrected surface hopping: Resetting wavefunction coefficients based
   on judgement of wave packet reflection" J. Chem. Phys. 2019,  150,  164101
  
*/


  int ntraj = dyn_var.ntraj;  //p.n_cols;
  int nadi = dyn_var.nadi;
  int ndof = dyn_var.ndof; 

  CMATRIX E(nadi, nadi);
  CMATRIX dE(nadi, nadi);
  MATRIX pa(ndof, 1);
  MATRIX pi(ndof, 1);
  MATRIX pi_adv(ndof, 1);
  MATRIX F(ndof, nadi);
  MATRIX fi(ndof, 1);

  dyn_var.reversal_events->set(-1, -1, 0.0);

//  MATRIX res(nadi, ntraj);  res = 0.0;

  for(int itraj=0; itraj<ntraj; itraj++){
    int a = dyn_var.act_states[itraj]; // active state index    
    pa = dyn_var.p->col(itraj); // this is momentum on the active state

    double T_a = compute_kinetic_energy(pa, *dyn_var.iM); // kinetic energy on the active state

    // Energy
    E = ham.children[itraj]->get_ham_adi();
    double E_a = E.get(a, a).real();  // potential energy on the active state

    // Forces on all states
    for(int idof=0; idof<ndof; idof++){
      dE = -1.0 * ham.children[itraj]->get_d1ham_adi(idof);
      for(int i=0; i<nadi; i++){  F.set(idof, i,  dE.get(i,i).real() );   }
    }


    for(int i=0; i<nadi; i++){
      if(i!=a){
        double E_i = E.get(i, i).real();  // potential energy on all other states
        double T_i = T_a + E_a - E_i;      // predicted kinetic energy on other state i

        // WP momenta on all surfaces
        if(T_i<0){  pi = 0.0000001 * pa; }   // infinitesimally small along pa
        else{   pi = sqrt(T_i/T_a) * pa; }   // just along pa, conserving energy
        
      }// i != a
      else{ pi = pa; }

      // Compute the advanced momentum
      fi = F.col(i);
      pi_adv = pi + fi * dt; 

      // Compute the old and new dot products:
      double dp_old = (pi.T() * fi).get(0,0);
      double dp_new = (pi_adv.T() * fi).get(0,0);

      // Different signs: reflection on the i-th surface takes place
      if(dp_old * dp_new < 0.0){
         dyn_var.reversal_events->set(i, itraj, 1.0);
      }
    }// for i
  }// for itraj

}


CMATRIX bcsh(CMATRIX& Coeff, double dt, vector<int>& act_states, MATRIX& reversal_events){
    /**
    \brief Branching corrected surface hopping decoherence algorithm
  
    This function performs the wavefunction amplitudes collapses according to:

    Xu, J.; Wang, L. "Branching corrected surface hopping: Resetting wavefunction coefficients based
    on judgement of wave packet reflection" J. Chem. Phys. 2019,  150,  164101
    
    \param[in]       Coeff [ CMATRIX(nadi, ntraj) ] An object containig electronic DOFs. 
    \param[in]          dt [ float ] The integration timestep. Units = a.u. of time
    \param[in]      act_st [ list of integer ] The active state indices for all trajectories
    \param[in]      reversal_events [ MATRIX(nadi, ntraj) ]  > 0 - wp on that state reverses

    The function returns:
    C [ CMATRIX ] - the updated state of the electronic DOF, in the same data type as the input

    */

    int nadi = Coeff.n_rows;
    int ntraj = Coeff.n_cols;

    CMATRIX C(Coeff);

    for(int itraj=0; itraj<ntraj; itraj++){    

      // First handle the active state:
      int a = act_states[itraj];

      // If it reverses, we collapse wfc on that state
      if(reversal_events.get(a, itraj) > 0 ){    collapse(C, itraj, a, 0);    }

      else{ // Otherwise, let's project out the all the nonactive states that reflect

        for(int i=0; i<nadi; i++){
          if(i!=a){
            if(reversal_events.get(i, itraj) > 0){ project_out(C, itraj, i);  }
          }// if
        }// for i
 
      }// else

    }// for itraj

  return C;

}



CMATRIX mfsd(MATRIX& p, CMATRIX& Coeff, MATRIX& invM, double dt, vector<MATRIX>& decoherence_rates, nHamiltonian& ham, Random& rnd, int isNBRA){
    /**
    \brief Mean field with stochastic decoherence
  
    This function performs the wavefunction amplitudes collapses according to:

    Bedard-Hearn, M. J.; Larsen, R. E.; Schwartz, B. J. "Mean-field dynamics with
    stochastic decoherence (MF-SD): A new algorithm for nonadiabatic mixed quantum/classical
    molecular-dynamics simulations with nuclear-induced decoherence" 
    J. Chem. Phys. 123, 234106, 2005.
    
    \param[in]       Coeff [ CMATRIX(nadi, ntraj) ] An object containig electronic DOFs. 
    \param[in]          dt [ float ] The integration timestep. Units = a.u. of time
    \param[in]      act_st [ list of integer ] The active state indices for all trajectories
    \param[in]      decoherence_rates [ MATRIX(nadi, nadi) x ntraj ]  - decoherence rates (1/tau_i)

    The function returns:
    C [ CMATRIX ] - the updated state of the electronic DOF, in the same data type as the input

    */

    int i, idof;
    int nadi = Coeff.n_rows;
    int ntraj = Coeff.n_cols;
    int ndof = ham.nnucl;

    CMATRIX C(Coeff);
    //CMATRIX Ctmp(Coeff);
    CMATRIX coeff(nadi, 1);
    CMATRIX coeff_tmp(nadi, 1);
    
    for(int itraj=0; itraj<ntraj; itraj++){    

      //================ Determine states onto which we could potentially collapse ===========
      double ksi = rnd.uniform(0.0, 1.0);
      vector<int> proposed_states;
      vector<double> hopping_prob;
      double summ_prob = 0.0;

      coeff = C.col(itraj);
      coeff_tmp = CMATRIX(coeff);

      //cout<<"*** trajectory "<<itraj<<" ****\n";
 
      for(i=0; i<nadi; i++){
           
        complex<double> c_i = C.get(i, itraj); 
  
        // Probability of decoherence on state i
        // Here, we assume that the state-only decoherence rates are on the diagonal
        double P_i;
        if(isNBRA==1){
        P_i = (std::conj(c_i) * c_i).real() * decoherence_rates[0].get(i, i) * dt;
        }
        else
        {
        P_i = (std::conj(c_i) * c_i).real() * decoherence_rates[itraj].get(i, i) * dt;
        }
        if(ksi<P_i){ 
          proposed_states.push_back(i);
          hopping_prob.push_back(P_i);
          summ_prob += P_i;
        }

      }// for i


      // There is a decoherence for at least 1 state
      if(summ_prob>0.0){ 
        // Renormalize relative probabilities of the collapse on any of non-unique states
        for(i=0; i<proposed_states.size(); i++){ hopping_prob[i] /= summ_prob;  }

        // Select one of the possible states
        int indx_i = hop(hopping_prob, rnd.uniform(0.0, 1.0) );
        int decohered_state = proposed_states[indx_i];

        // So, now we collapse the MF wfc to a pure state `decohered_state`
        vector<int> _id(2, 0);  _id[1] = itraj;
        double E_old = ham.Ehrenfest_energy_adi(coeff, _id).real();

        collapse(coeff_tmp, 0, decohered_state, 0);        
        double E_new = ham.Ehrenfest_energy_adi(coeff_tmp, _id).real();
        //double E_new = ham.get_ham_adi(_id).get(decohered_state, decohered_state).real();

        //cout<<"Decohered state = "<<decohered_state<<" E_old = "<<E_old<<"  E_new = "<<E_new<<endl;

        // Compute the MF NAC direction
        MATRIX nac_eff(ndof, 1);
        MATRIX p_i(ndof, 1);

        for(i=0; i<nadi; i++){
          complex<double> c_i = C.get(i, itraj);   

          // Weighting factor for NACs
          double P_i = (std::conj(c_i) * c_i).real();
          
          //p_i = p.col(itraj); /// momentum
          for(idof=0; idof<ndof; idof++){
            if(i!=decohered_state){
              nac_eff.add(idof, 0, P_i * ham.get_dc1_adi(idof, _id).get(i, decohered_state).real() );
            }
          }// idof        
        }// i

        p_i = p.col(itraj);

        double nac_mag = (nac_eff.T() * nac_eff).get(0,0);
        
        if(  can_rescale_along_vector(E_old, E_new, p_i, invM, nac_eff) && nac_mag > 0.0 ){

          //cout<<"Decohered state = "<<decohered_state<<" E_old = "<<E_old<<"  E_new = "<<E_new<<endl;
          //cout<<"Ampl = \n"; C.show_matrix();
          //cout<<"momentum = \n"; p_i.show_matrix();
//          cout<<"  nac_eff = \n"; nac_eff.show_matrix();

          // Adjust velocities 
          int do_reverse = 0;
          rescale_along_vector(E_old, E_new, p_i, invM, nac_eff, do_reverse);

          for(idof=0; idof<ndof; idof++){ p.set(idof, itraj, p_i.get(idof, 0)); }

          //cout<<"new momentum = \n";
          //p.show_matrix();

          // And collapse the coherent superposition on the decohered state 
          collapse(C, itraj, decohered_state, 0);

          //cout<<"Ampl (after) = \n"; C.show_matrix();
        }// can rescale

      }// possible collapse: summ_prob > 0.0
        
    }// for itraj

    return C;

}



CMATRIX mfsd(MATRIX& p, CMATRIX& Coeff, MATRIX& invM, double dt, vector<MATRIX>& decoherence_rates, nHamiltonian& ham, Random& rnd){

 int is_nbra = 0;
 return mfsd(p, Coeff, invM, dt, decoherence_rates, ham, rnd, is_nbra);

}

void xf_init_AT(dyn_variables& dyn_var, int traj, int ist){
    /**
    \brief Initialize the ist-th auxiliary trajectory. When ist==-1, initialize all auxiliary trajectories 

    */
    int nadi = dyn_var.nadi;

    dyn_var.q_aux[traj]->set(ist, -1, 0.0);
    dyn_var.p_aux[traj]->set(ist, -1, 0.0);
    dyn_var.p_aux_old[traj]->set(ist, -1, 0.0);
    dyn_var.nab_phase[traj]->set(ist, -1, 0.0);

    if( ist == -1){
      dyn_var.is_mixed[traj].assign(nadi, 0);
      dyn_var.is_first[traj].assign(nadi, 0);
      dyn_var.is_fixed[traj].assign(nadi, 0);
      dyn_var.is_keep[traj].assign(nadi, 0);
      dyn_var.p_quant->set(-1, traj, 0.0);
      dyn_var.VP->set(-1, traj, 0.0);
    }
    else{
      dyn_var.is_mixed[traj][ist] = 0;
      dyn_var.is_first[traj][ist] = 0;
      dyn_var.is_fixed[traj][ist] = 0;
      dyn_var.is_keep[traj][ist] = 0;
    }
}

void xf_destroy_AT(dyn_variables& dyn_var, double threshold){
    /**
    \brief When the electronic state recovers to an adiabatic state, destroy auxiliary trajectories

    */
    int traj;
    int ntraj = dyn_var.ntraj;
    int nadi = dyn_var.nadi;

    double upper_lim = 1.0 - threshold;
    double lower_lim = threshold;
    
    for(int traj=0; traj<ntraj; traj++){
      vector<int>& is_mixed = dyn_var.is_mixed[traj];
      vector<int>& is_first = dyn_var.is_first[traj];
      CMATRIX& dm = *dyn_var.dm_adi[traj];

      int is_recovered = 0;

      for(int i=0; i<nadi; i++){
        double a_ii = dm.get(i,i).real(); 
        if(is_mixed[i]==1){
          if(a_ii>upper_lim){
            is_recovered = 1;
            collapse(*dyn_var.ampl_adi, traj, i, 0);
            break;
          }
        }
      } //i

      if(is_recovered==1){xf_init_AT(dyn_var, traj, -1);}
    } //traj
}

void xf_destroy_AT(dyn_variables& dyn_var, nHamiltonian& ham, double threshold){
    /**
    \brief When the electronic state recovers to an adiabatic state, destroy auxiliary trajectories
           Here, momentum rescaling is also performed.
    */
    int traj;
    int ntraj = dyn_var.ntraj;
    int nadi = dyn_var.nadi;

    double upper_lim = 1.0 - threshold;
    double lower_lim = threshold;
    
    for(int traj=0; traj<ntraj; traj++){
      vector<int>& is_mixed = dyn_var.is_mixed[traj];
      vector<int>& is_first = dyn_var.is_first[traj];
      CMATRIX& dm = *dyn_var.dm_adi[traj];

      int is_recovered = 0;

      for(int i=0; i<nadi; i++){
        double a_ii = dm.get(i,i).real(); 
        if(is_mixed[i]==1){
          if(a_ii>upper_lim){
            is_recovered = 1;

            // Before the collapse
            vector<int> _id(2, 0);  _id[1] = traj;
            CMATRIX coeff(nadi, 1);
            coeff = dyn_var.ampl_adi->col(traj);
            double Epot_old = ham.Ehrenfest_energy_adi(coeff, _id).real();

            collapse(*dyn_var.ampl_adi, traj, i, 0);
            
            // After the collapse
            coeff = dyn_var.ampl_adi->col(traj);
            double Epot = ham.Ehrenfest_energy_adi(coeff, _id).real();

            // Rescaling momenta for the energy conservation
            MATRIX p_real(dyn_var.ndof, 1); p_real = dyn_var.p->col(traj); 
            double alpha = compute_kinetic_energy(p_real, *dyn_var.iM) + Epot_old - Epot;

            if(alpha > 0.0){alpha /= compute_kinetic_energy(p_real, *dyn_var.iM);}
            else{alpha = 0.0;
              cout << "Total energy is drifted due to adiabatic recovery to a classically forbidden state!" << endl;
            }

            for(int idof=0; idof<dyn_var.ndof; idof++){
              dyn_var.p->set(idof, traj, dyn_var.p->get(idof, traj) * sqrt(alpha));
            }

            break;
          }
        }
      } //i

      if(is_recovered==1){xf_init_AT(dyn_var, traj, -1);}
    } //traj
}

void xf_create_AT(dyn_variables& dyn_var, double threshold){
    /**
    \brief When the electronic state is in a superposition between adiabatic states, create auxiliary trajectories

    */
    int traj;
    int ntraj = dyn_var.ntraj;
    int nadi = dyn_var.nadi;
    
    double upper_lim = 1.0 - threshold;
    double lower_lim = threshold;
    
    for(int traj=0; traj<ntraj; traj++){
      vector<int>& is_mixed = dyn_var.is_mixed[traj];
      vector<int>& is_first = dyn_var.is_first[traj];
      CMATRIX& dm = *dyn_var.dm_adi[traj];

      int nr_mixed = 0; 
      for(int i=0; i<nadi; i++){
        double a_ii = dm.get(i,i).real(); 
        if(a_ii<=lower_lim || a_ii>upper_lim){
          is_mixed[i]=0;
          xf_init_AT(dyn_var, traj, i);
        }
        else{
          is_mixed[i]==1 ? is_first[i]=0:is_first[i]=1;
          is_mixed[i]=1;
          nr_mixed += 1;
        }
      } //i

      // Prevent spurious decoherence
      if (nr_mixed < 2){
         is_mixed.assign(nadi, 0);
         is_first.assign(nadi, 0);
      }
    } //traj 

}

void xf_hop_reset(dyn_variables& dyn_var, vector<int>& accepted_states, vector<int>& initial_states){
  int traj;
  int ntraj = dyn_var.ntraj;

for(traj = 0; traj < ntraj; traj++){
    // When a hop occurs, destroy auxiliary trajectories 
    if(accepted_states[traj] != initial_states[traj]){
      xf_init_AT(dyn_var, traj, -1);
      cout << "destroy auxiliary trajectories of traj " << traj << " due to a hop" << endl;
    }
  }// traj
}

void td_width_aux(dyn_variables& dyn_var){
  int ntraj = dyn_var.ntraj;
  int nadi = dyn_var.nadi;
  int ndof = dyn_var.ndof; 

  double width_temp;

  dyn_var.wp_width->set(-1, -1, 0.0);
  for(int traj=0; traj<ntraj; traj++){
    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    vector<int>& is_first = dyn_var.is_first[traj];

    // wp_width is computed by pairwise widths based on auxiliary trajectories
    MATRIX sum_inv_w2(ndof, 1);
    MATRIX w2_temp(ndof, 1);
    
    int check_mixing = 0;
    for(int i=0; i<nadi; i++){
      for(int j=0; j<nadi; j++){
        if(i>=j){continue;}

        if(is_mixed[i] == 0 or is_mixed[j] == 0){continue; }
        check_mixing = 1;

        if(is_first[i] == 1 or is_first[j] == 1){
          w2_temp.set(-1, 1.0e+10); // At initial, an auxiliary pair does not contribute to wp_width
        }
        else{
          for(int idof=0; idof<ndof; idof++){
            w2_temp.set(idof, 0, fabs(dyn_var.q_aux[traj]->get(i, idof) - dyn_var.q_aux[traj]->get(j, idof))/
              fabs(dyn_var.p_aux[traj]->get(i, idof) - dyn_var.p_aux[traj]->get(j, idof)) );
          }
        }

        for(int idof=0; idof<ndof; idof++){
          sum_inv_w2.add(idof, 0, 1.0/w2_temp.get(idof));
        }

      } //j
    } //i
    
    if(check_mixing == 1){
      for(int idof=0; idof<ndof; idof++){
        dyn_var.wp_width->set(idof, traj, sqrt((nadi - 1)* 1.0/sum_inv_w2.get(idof, 0)) );
      }
    }

  } // traj
}

void shxf(dyn_variables& dyn_var, nHamiltonian& ham, nHamiltonian& ham_prev, dyn_control_params& prms){
    /**
    \brief The generic framework of the SHXF (Surface Hopping based on eXact Factorization) method of
    Ha, J.-K.; Lee, I. S.; Min, S. K. J. Phys. Chem. Lett. 2018, 9, 1097

    */
  int ntraj = dyn_var.ntraj;
  int nadi = dyn_var.nadi;
  int ndof = dyn_var.ndof; 

  xf_destroy_AT(dyn_var, prms.coherence_threshold);

  xf_create_AT(dyn_var, prms.coherence_threshold);

  MATRIX& invM = *dyn_var.iM;
  for(int traj=0; traj<ntraj; traj++){

    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    vector<int>& is_first = dyn_var.is_first[traj];
    vector<int>& is_fixed = dyn_var.is_fixed[traj];
    MATRIX& q_aux = *dyn_var.q_aux[traj];
    MATRIX& p_aux = *dyn_var.p_aux[traj];

    // Propagate auxiliary positions
    int a = dyn_var.act_states[traj];

    for(int i=0; i<nadi; i++){
      if(is_mixed[i]==1){
        if(is_fixed[i]==1){continue;}

        if(is_first[i]==1){
          // Initially, the auxiliary position is set to the real position
          for(int idof=0; idof<ndof; idof++){q_aux.set(i, idof, dyn_var.q->get(idof, traj));}
        }
        else{
          if(i==a){
            for(int idof=0; idof<ndof; idof++){q_aux.set(i, idof, dyn_var.q->get(idof, traj));}
          }
          else{
            for(int idof=0; idof<ndof; idof++){  
              q_aux.add(i, idof, invM.get(idof,0) * p_aux.get(i, idof) * prms.dt); 
            }
          }
        }
      }
    } //i
  } //traj

  // Propagate auxiliary momenta
  for(int traj=0; traj<ntraj; traj++){
    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    vector<int>& is_first = dyn_var.is_first[traj];
    vector<int>& is_fixed = dyn_var.is_fixed[traj];
    vector<int>& is_keep = dyn_var.is_keep[traj];
    MATRIX& p_aux = *dyn_var.p_aux[traj];
    MATRIX& p_aux_old = *dyn_var.p_aux_old[traj];
    
    int a = dyn_var.act_states[traj];
    
    MATRIX p_real(ndof, 1); 
    MATRIX p_aux_temp(ndof, 1);
    
    CMATRIX ham_adi(nadi, nadi);
    CMATRIX ham_adi_prev(nadi, nadi);
    ham_adi = ham.children[traj]->get_ham_adi();
    ham_adi_prev = ham_prev.children[traj]->get_ham_adi();

    for(int i=0; i<nadi; i++){
      for(int idof=0; idof<ndof; idof++){
        p_aux_old.set(i, idof, p_aux.get(i, idof));
      }
    }

    int is_tp = 0;
    double alpha; 
    for(int i=0; i<nadi; i++){
      if(is_mixed[i]==1){
        if(is_fixed[i]==1 or is_keep[i]==1){continue;}

        p_real = dyn_var.p->col(traj); 
        
        if(i==a){
          alpha = compute_kinetic_energy(p_real, invM);
        }
        else{
          if(is_first[i]==1){
            alpha = compute_kinetic_energy(p_real, invM) + ham_adi.get(a,a).real() - ham_adi.get(i,i).real();
          }
          else{
            p_aux_temp = p_aux_old.row(i).T(); 
            alpha = compute_kinetic_energy(p_aux_temp, invM) + ham_adi_prev.get(i,i).real() - ham_adi.get(i,i).real();
          }
        }

        if (alpha < 0.0){ alpha = 0.0;
          if (prms.project_out_aux == 1){
            project_out(*dyn_var.ampl_adi, traj, i);
            xf_init_AT(dyn_var, traj, -1);
            cout << "Project out a classically forbidden state " << i << " on traj " << traj <<endl;
            break;
          }
        }
        
        alpha /= compute_kinetic_energy(p_real, invM);
        for(int idof=0; idof<ndof; idof++){
          p_aux.set(i, idof, dyn_var.p->get(idof, traj) * sqrt(alpha));
        }

        // Check the turning point
        if (is_first[i] == 0 and prms.tp_algo != 0){
          MATRIX Fa(ndof, 1);
          for(int idof=0; idof<ndof; idof++){
            Fa.set(idof, 0, dyn_var.f->get(idof,0) );
          }
          double dp_old = 0.0;
          double dp_new = 0.0;

          for(int idof=0; idof<ndof; idof++){
            dp_old += Fa.get(idof, 0) * p_aux_old.get(i, idof);
            dp_new += Fa.get(idof, 0) * p_aux.get(i, idof);
          }

          if(dp_old*dp_new < 0.0){
            is_tp = 1;
            break;
          }
        }

      }
    }//i

    if(is_tp == 1){
      if(prms.tp_algo == 1){
        collapse(*dyn_var.ampl_adi, traj, a, 0);
        xf_init_AT(dyn_var, traj, -1);
        cout << "Collapse to the active state " << a << " at a classical turning point on traj " << traj <<endl;
      }
      else if(prms.tp_algo == 2){
        for(int i=0; i<nadi; i++){
          if(i == a){continue;}
          
          for(int idof=0; idof<ndof; idof++){
            p_aux.set(i, idof, 0.0);
            p_aux_old.set(i, idof, 0.0);
          }
          is_fixed[i] = 1;
        }//i
        cout << "Fix auxiliary trajectory except for the active state " << a << " at a classical turning point on traj " << traj <<endl;
      }
      else if(prms.tp_algo == 3){
        for(int i=0; i<nadi; i++){
          if(i == a){continue;}

          for(int idof=0; idof<ndof; idof++){
            p_aux.set(i, idof, p_aux_old.get(i, idof));
          }
          is_keep[i] = 1;
        } 
        cout << "Keep auxiliary momenta except for the active state " << a << " at a classical turning point on traj " << traj <<endl;
      }

      is_tp = 0;
    }
  }//traj

  // Compute the td width based on auxiliary trajectories
  if(prms.use_td_width == 3){ td_width_aux(dyn_var);}

  //// Propagate the spatial derivative of phases
  //for(int traj=0; traj<ntraj; traj++){
  //  vector<int>& is_mixed = dyn_var.is_mixed[traj];
  //  vector<int>& is_first = dyn_var.is_first[traj];
  //  MATRIX& p_aux = *dyn_var.p_aux[traj];
  //  MATRIX& p_aux_old = *dyn_var.p_aux_old[traj];
  //  MATRIX& nab_phase = *dyn_var.nab_phase[traj];

  //  for(int i=0; i<nadi; i++){
  //    if(is_mixed[i]==1){
  //      if(is_first[i]==1){
  //        nab_phase.set(i, -1, 0.0);
  //      }
  //      else{
  //        for(int idof=0; idof<ndof; idof++){
  //          nab_phase.add(i, idof, p_aux.get(i, idof) - p_aux_old.get(i, idof));
  //        }//idof
  //      }
  //    }
  //  }//i
  //} // traj

  //cout << "SHXF " << dyn_var.p_quant->get(0,0) << " " << dyn_var.q_aux[0]->get(0,0) << " " << dyn_var.q_aux[0]->get(1,0)
  //     << " " << dyn_var.p_aux[0]->get(0,0) << " " << dyn_var.p_aux[0]->get(1,0) <<endl; // Debug

}


void mqcxf(dyn_variables& dyn_var, nHamiltonian& ham, nHamiltonian& ham_prev, dyn_control_params& prms){
    /**
    \brief The generic framework of the MQCXF (Mixed Quantum-Classical based on eXact Factorization) method of
    Ha, J.-K.; Min, S. K. J. Chem. Phys. 2022, 156, 174109

    */
  int ntraj = dyn_var.ntraj;
  int nadi = dyn_var.nadi;
  int ndof = dyn_var.ndof; 

  xf_destroy_AT(dyn_var, ham, prms.coherence_threshold);

  xf_create_AT(dyn_var, prms.coherence_threshold);

  MATRIX& invM = *dyn_var.iM;
  for(int traj=0; traj<ntraj; traj++){

    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    vector<int>& is_first = dyn_var.is_first[traj];
    vector<int>& is_fixed = dyn_var.is_fixed[traj];
    MATRIX& q_aux = *dyn_var.q_aux[traj];
    MATRIX& p_aux = *dyn_var.p_aux[traj];

    for(int i=0; i<nadi; i++){
      if(is_mixed[i]==1){
        if(is_fixed[i]==1){continue;}

        if(is_first[i]==1){
          // Initially, the auxiliary position is set to the real position
          for(int idof=0; idof<ndof; idof++){q_aux.set(i, idof, dyn_var.q->get(idof, traj));}
        }
        else{
          for(int idof=0; idof<ndof; idof++){  
            q_aux.add(i, idof, invM.get(idof,0) * p_aux.get(i, idof) * prms.dt); 
          }
        }
      }
    } //i
  } //traj

  // Propagate auxiliary momenta
  CMATRIX coeff(nadi, 1);

  for(int traj=0; traj<ntraj; traj++){
    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    vector<int>& is_first = dyn_var.is_first[traj];
    vector<int>& is_fixed = dyn_var.is_fixed[traj];
    vector<int>& is_keep = dyn_var.is_keep[traj];
    MATRIX& p_aux = *dyn_var.p_aux[traj];
    MATRIX& p_aux_old = *dyn_var.p_aux_old[traj];
    
    int a = dyn_var.act_states[traj];

    MATRIX p_real(ndof, 1); 
    MATRIX p_aux_temp(ndof, 1);
    
    CMATRIX ham_adi(nadi, nadi);
    CMATRIX ham_adi_prev(nadi, nadi);
    ham_adi = ham.children[traj]->get_ham_adi();
    ham_adi_prev = ham_prev.children[traj]->get_ham_adi();

    for(int i=0; i<nadi; i++){
      for(int idof=0; idof<ndof; idof++){
        p_aux_old.set(i, idof, p_aux.get(i, idof));
      }
    }
   
    vector<int> _id(2, 0);  _id[1] = traj;
    coeff = dyn_var.ampl_adi->col(traj);
    double Epot = ham.Ehrenfest_energy_adi(coeff, _id).real();
    
    int is_tp = 0;
    double alpha; 
    for(int i=0; i<nadi; i++){
      if(is_mixed[i]==1){
        if(is_fixed[i]==1 or is_keep[i]==1){continue;}

        p_real = dyn_var.p->col(traj); 
        
        if(is_first[i]==1){
          alpha = compute_kinetic_energy(p_real, invM) + Epot - ham_adi.get(i,i).real();
        }
        else{
          //p_aux_temp = p_aux_old.row(i).T(); 
          //alpha = compute_kinetic_energy(p_aux_temp, invM) + ham_adi_prev.get(i,i).real() - ham_adi.get(i,i).real();
          alpha = compute_kinetic_energy(p_real, invM) + Epot - ham_adi.get(i,i).real();
        }

        if (alpha < 0.0){ alpha = 0.0;
          if (prms.project_out_aux == 1 and i!=a){
            project_out(*dyn_var.ampl_adi, traj, i);
            xf_init_AT(dyn_var, traj, -1);
            cout << "Project out a classically forbidden state " << i << " on traj " << traj <<endl;

            // rescaling velocity
            double Epot_old = Epot;
            coeff = dyn_var.ampl_adi->col(traj);
            Epot = ham.Ehrenfest_energy_adi(coeff, _id).real();

            alpha = compute_kinetic_energy(p_real, invM) + Epot_old - Epot;
            alpha /= compute_kinetic_energy(p_real, invM);
            for(int idof=0; idof<ndof; idof++){
              dyn_var.p->set(idof, traj, dyn_var.p->get(idof, traj) * sqrt(alpha));
            }

            break;
          }
        }

        alpha /= compute_kinetic_energy(p_real, invM);
        for(int idof=0; idof<ndof; idof++){
          p_aux.set(i, idof, dyn_var.p->get(idof, traj) * sqrt(alpha));
        }
        
        // Check the turning point
        if (is_first[i] == 0 and prms.tp_algo != 0){
          MATRIX Fa(ndof, 1);
          for(int idof=0; idof<ndof; idof++){
            Fa.set(idof, 0, dyn_var.f->get(idof,0) );
          }
          double dp_old = 0.0;
          double dp_new = 0.0;

          for(int idof=0; idof<ndof; idof++){
            dp_old += Fa.get(idof, 0) * p_aux_old.get(i, idof);
            dp_new += Fa.get(idof, 0) * p_aux.get(i, idof);
          }

          if(dp_old*dp_new < 0.0){
            is_tp = 1;
            break;
          }
        }

      }
    }//i

    if(is_tp == 1){
      if(prms.tp_algo == 1){
        double Epot_old = Epot;

        int a = dyn_var.act_states[traj];
        collapse(*dyn_var.ampl_adi, traj, a, 0);
        
        // After the collapse
        coeff = dyn_var.ampl_adi->col(traj);
        double Epot = ham.Ehrenfest_energy_adi(coeff, _id).real();
        
        // Rescaling momenta for the energy conservation
        p_real = dyn_var.p->col(traj); 
        double alpha = compute_kinetic_energy(p_real, invM) + Epot_old - Epot;

        if(alpha > 0.0){alpha /= compute_kinetic_energy(p_real, invM);}
        else{
          alpha = 0.0;
          cout << "Total energy is drifted due to the dynamics initialization at a classical turning point" << endl;
        }

        for(int idof=0; idof<dyn_var.ndof; idof++){
          dyn_var.p->set(idof, traj, dyn_var.p->get(idof, traj) * sqrt(alpha));
        }

        xf_init_AT(dyn_var, traj, -1);
        cout << "Collapse to the active state " << a << " at a classical turning point on traj " << traj <<endl;
      }
      else if(prms.tp_algo == 2){
        int a = dyn_var.act_states[traj];
        for(int i=0; i<nadi; i++){
          if(i == a){continue;}
          
          for(int idof=0; idof<ndof; idof++){
            p_aux.set(i, idof, 0.0);
            p_aux_old.set(i, idof, 0.0);
          }
          is_fixed[i] = 1;
        }//i
        cout << "Fix auxiliary trajectory except for the active state " << a << " at a classical turning point on traj " << traj <<endl;
      }
      else if(prms.tp_algo == 3){
        int a = dyn_var.act_states[traj];
        for(int i=0; i<nadi; i++){
          if(i == a){continue;}

          for(int idof=0; idof<ndof; idof++){
            p_aux.set(i, idof, p_aux_old.get(i, idof));
          }
          is_keep[i] = 1;
        } 
        cout << "Keep auxiliary momenta except for the active state " << a << " at a classical turning point on traj " << traj <<endl;
      }
      is_tp = 0;
    }

  }//traj

  // Compute the td width based on auxiliary trajectories
  if(prms.use_td_width == 3){ td_width_aux(dyn_var);}

  //// Propagate the spatial derivative of phases; the E-based approximation is used
  //for(int traj=0; traj<ntraj; traj++){
  //  vector<int>& is_mixed = dyn_var.is_mixed[traj];
  //  vector<int>& is_first = dyn_var.is_first[traj];
  //  MATRIX& nab_phase = *dyn_var.nab_phase[traj];
  //
  //  CMATRIX E(nadi, nadi);
  //  E = ham.children[traj]->get_ham_adi();

  //  MATRIX p_real(ndof, 1);
  //  p_real = dyn_var.p->col(traj); 
  //  double Ekin = compute_kinetic_energy(p_real, invM);

  //  for(int i=0; i<nadi; i++){
  //    if(is_mixed[i]==1){
  //      for(int idof=0; idof<ndof; idof++){
  //        nab_phase.set(i, idof, -0.5*E.get(i,i).real()*dyn_var.p->get(idof,traj)/(Ekin + prms.e_mask));
  //      }//idof
  //    }
  //  }//i
  //} // traj
}

void update_nab_phase(dyn_variables& dyn_var, nHamiltonian& ham, dyn_control_params& prms){
  int ntraj = dyn_var.ntraj;
  int nadi = dyn_var.nadi;
  int ndof = dyn_var.ndof; 
  
  for(int traj=0; traj<ntraj; traj++){
    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    vector<int>& is_first = dyn_var.is_first[traj];
    MATRIX& p_aux = *dyn_var.p_aux[traj];
    MATRIX& p_aux_old = *dyn_var.p_aux_old[traj];
    MATRIX& nab_phase = *dyn_var.nab_phase[traj];
    MATRIX& nab_phase_old = *dyn_var.nab_phase_old[traj];
    
    CMATRIX T_new(*dyn_var.proj_adi[traj]);
    if(prms.assume_always_consistent){ T_new.identity(); }
          
    vector<MATRIX> P_AUX; vector<MATRIX> P_AUX_OLD;
    for(int idof=0; idof<ndof; idof++){
      P_AUX.push_back(MATRIX(nadi, nadi));
      P_AUX_OLD.push_back(MATRIX(nadi, nadi));
    }

    if (prms.decoherence_algo==5){
      for(int i=0; i<nadi; i++){
        if(is_mixed[i]==1){
          if(is_first[i]==1){
            nab_phase.set(i, -1, 0.0);
            nab_phase_old.set(i, -1, 0.0);
          }
          else{ 
            for(int idof=0; idof<ndof; idof++){
              for(int i=0; i<nadi; i++){
                if(is_mixed[i]==1){
                  P_AUX[idof].set(i,i, p_aux.get(i, idof) );
                  P_AUX_OLD[idof].set(i,i, p_aux_old.get(i, idof) );
                }
              }
              P_AUX[idof] = (T_new.H() * P_AUX[idof] * T_new).real();
              P_AUX_OLD[idof] = (T_new.H() * P_AUX_OLD[idof] * T_new).real();
            }

            for(int idof=0; idof<ndof; idof++){
              nab_phase.set(i, idof, nab_phase_old.get(i, idof) + P_AUX[idof].get(i, i) - P_AUX_OLD[idof].get(i, i));
            }
            
            for(int idof=0; idof<ndof; idof++){
              nab_phase_old.set(i, idof, nab_phase.get(i, idof));
            }
          }
        }
      }//i
    }
    else{
      MATRIX& invM = *dyn_var.iM;
      CMATRIX E(nadi, nadi);
      E = ham.children[traj]->get_ham_adi();
      E = T_new.H() * E * T_new;

      MATRIX p_real(ndof, 1);
      p_real = dyn_var.p->col(traj); 
      double Ekin = compute_kinetic_energy(p_real, invM);

      for(int i=0; i<nadi; i++){
        if(is_mixed[i]==1){
          for(int idof=0; idof<ndof; idof++){
            nab_phase.set(i, idof, -0.5*E.get(i,i).real()*dyn_var.p->get(idof,traj)/(Ekin + prms.e_mask));
          }//idof
        }
      }//i

    }
  } // traj

}

void XF_correction(CMATRIX& Ham, dyn_variables& dyn_var, CMATRIX& C, CMATRIX& T, int traj){

  int ndof = dyn_var.ndof;
  int nst = dyn_var.nadi;
  MATRIX& invM = *dyn_var.iM;
  
  vector<int>& is_mixed = dyn_var.is_mixed[traj];

  // Construct and transform the density matrix
  CMATRIX RHO(nst, nst);
  RHO = T * C * C.H() * T.H();

  // Compute quantum momenta
  int a = dyn_var.act_states[traj];

  dyn_var.p_quant->set(-1, traj, 0.0);
  for(int i=0; i<nst; i++){
    if(is_mixed[i]==1){
      for(int idof=0; idof<ndof; idof++){
        dyn_var.p_quant->add(idof, traj, 0.5 / pow(dyn_var.wp_width->get(idof, traj), 2.0) * RHO.get(i,i).real()
          *(dyn_var.q_aux[traj]->get(a, idof) - dyn_var.q_aux[traj]->get(i, idof)));
         // *(dyn_var.q->get(idof, traj) - dyn_var.q_aux[traj]->get(i, idof)));
      }
    }
  }

  // Add the XF-based decoherence correction
  for(int idof=0; idof<ndof; idof++){

    // Set a diagonal matrix of nabla_phase for each dof
    CMATRIX F(nst, nst);
    for(int i=0; i<nst; i++){
      if(is_mixed[i]==1){
        F.set(i,i, complex<double>(0.0, dyn_var.nab_phase[traj]->get(i, idof)) );
      }
    }
    F = T * F * T.H();

    Ham += -invM.get(idof,0) * dyn_var.p_quant->get(idof, traj)*(RHO * F - F * RHO);
  }
}

void update_forces_xf(dyn_variables& dyn_var, nHamiltonian& ham, nHamiltonian& ham_prev){
  /**
    Add the decoherence force in XFMQC
  */

  int ntraj = dyn_var.ntraj;
  int ndof = dyn_var.ndof;
  int nst = dyn_var.nadi;
  MATRIX& invM = *dyn_var.iM;
  
  CMATRIX C(nst, 1);
  CMATRIX Coeff(nst, ntraj);

  // termporaries for nabla_phase and adiabatic force
  vector<CMATRIX> F;
  for(int idof=0; idof<ndof; idof++){
    F.push_back(CMATRIX(nst, nst));
  }
  
  Coeff = *dyn_var.ampl_adi;

  dyn_var.f_xf->set(-1, -1, 0.0);

  for(int traj=0; traj<ntraj; traj++){
    C = Coeff.col(traj);

    CMATRIX E(nst, nst);
    E = ham.children[traj]->get_ham_adi();

    double Epot = (C.H()*E*C).get(0,0).real();

    MATRIX p_real(ndof, 1);
    p_real = dyn_var.p->col(traj); 
    double Ekin = compute_kinetic_energy(p_real, invM);

    // Compute F for each dof
    vector<int>& is_mixed = dyn_var.is_mixed[traj];
    for(int idof=0; idof<ndof; idof++){
      F[idof].set(-1,-1, complex<double> (0.0, 0.0));
      for(int i=0; i<nst; i++){
        if(is_mixed[i]==1){
          F[idof].set(i,i,complex<double> (dyn_var.nab_phase[traj]->get(i, idof), 0.0) );
        }
      }
    }

    // Save vector potential (contribution from NAC is neglected)
    for(int idof=0; idof<ndof; idof++){
      CMATRIX temp = C.H()*F[idof]*C;
      dyn_var.VP->set(idof, traj, temp.get(0,0).real());
    }

    // Original form of the decoherence force
    for(int idof=0; idof<ndof; idof++){
      for(int jdof=0; jdof<ndof; jdof++){
        CMATRIX temp = (F[jdof]*C).H() * (F[idof]*C);
        dyn_var.f_xf->add(idof, traj, -2.0*invM.get(jdof,0)*dyn_var.p_quant->get(jdof, traj)*
          (dyn_var.VP->get(jdof, traj)*dyn_var.VP->get(idof, traj) - temp.get(0,0).real() ) );
      }
    }

    //// Energy-conserving treatment
    //for(int idof=0; idof<ndof; idof++){
    //  double temp = 0.0;
    //  for(int jdof=0; jdof<ndof; jdof++){
    //    temp +=invM.get(jdof,0)*dyn_var.p_quant->get(jdof, traj)* 
    //    ( dyn_var.VP->get(jdof, traj)*Epot - (C.H()*F[jdof]*E*C).get(0,0).real() );
    //  }
    //  dyn_var.f_xf->set(idof, traj, temp / Ekin * dyn_var.p->get(idof, traj) );
    //}
  } //traj

  // Add the XF contribution
  *dyn_var.f += *dyn_var.f_xf;
}

void propagate_half_xf(dyn_variables& dyn_var, nHamiltonian& Ham, dyn_control_params& prms, int rotation){
  int itraj, i, j;

  int num_el = prms.num_electronic_substeps;
  double dt = prms.dt / num_el;
  int method = prms.electronic_integrator;
  
  //======= Parameters of the dyn variables ==========
  int ndof = dyn_var.ndof;
  int ntraj = dyn_var.ntraj;
  int nadi = dyn_var.nadi;
  int ndia = dyn_var.ndia;
  MATRIX& invM = *dyn_var.iM;

  int nst = nadi;

  CMATRIX C(nst, 1);
  CMATRIX Coeff(nst, ntraj);
  
  Coeff = *dyn_var.ampl_adi;

  for(itraj=0; itraj<ntraj; itraj++){

    C = Coeff.col(itraj);

    int traj1 = itraj;  if(method >=100 && method <200){ traj1 = 0; }
    
    nHamiltonian* ham = Ham.children[traj1];

    CMATRIX T(*dyn_var.proj_adi[itraj]);  T.load_identity();
    CMATRIX T_new(*dyn_var.proj_adi[itraj]);
    if(prms.assume_always_consistent){ T_new.identity(); }

    // Generate the half-time exponential operator 
    CMATRIX Hxf(nadi, nadi);
    CMATRIX D(nadi, nadi); /// this is \exp[-idt/4\hbar * ( T_new.H()*Hxf(t+dt)*T_new + Hxf(t) )]

    XF_correction(Hxf, dyn_var, C, T, itraj);
    //XF_correction(Hxf, dyn_var, C, T, itraj);

    //Hxf = T_new.H() * Hxf * T_new;      
    //Hxf += Hxf_old;

    D = libspecialfunctions::exp_(Hxf, complex<double>(0.0, -dt) );
    if(rotation == 1){
      D = T_new * D * T_new.H();      
    }

    C = D * C;

//  *dyn_var.proj_adi[itraj] = T_new;

    // Insert the propagated result back
    for(int st=0; st<nst; st++){  Coeff.set(st, itraj, C.get(st, 0));  }

  } //itraj
  *dyn_var.ampl_adi = Coeff;
}

}// namespace libdyn
}// liblibra

