/*********************************************************************************
* Copyright (C) 2015-2017 Alexey V. Akimov
*
* This file is distributed under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of
* the License, or (at your option) any later version.
* See the file LICENSE in the root directory of this distribution
* or <http://www.gnu.org/licenses/>.
*
*********************************************************************************/
/**
  \file libdyn.cpp
  \brief The file implements Python export function
    
*/

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "libdyn.h"


/// liblibra namespace
namespace liblibra{

using namespace boost::python;

/// libdyn namespace
namespace libdyn{

using namespace libnuclear;
using namespace libelectronic;
//using namespace librigidbody;
using namespace libthermostat;
using namespace libbarostat;
using namespace libwfcgrid;
using namespace libwfcgrid2;
using namespace libensemble;
using namespace libgwp;

using namespace libthermostat;


void export_Verlet_objects(){

  void (*expt_Verlet0_v1)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params) = &Verlet0;
  void (*expt_Verlet1_v1)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params) = &Verlet1;
  void (*expt_Verlet1_v2)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params, int ent_opt) = &Verlet1;

  def("Verlet0", expt_Verlet0_v1);
  def("Verlet1", expt_Verlet1_v1);
  def("Verlet1", expt_Verlet1_v2);

  void (*expt_Verlet0_nvt_v1)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params, Thermostat& therm)
   = &Verlet0_nvt;
  void (*expt_Verlet1_nvt_v1)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params, vector<Thermostat>& therm)
  = &Verlet1_nvt;
  void (*expt_Verlet1_nvt_v2)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params, int ent_opt, vector<Thermostat>& therm)
  = &Verlet1_nvt;
  void (*expt_Verlet1_nvt_v3)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params, Thermostat& therm)
  = &Verlet1_nvt;
  void (*expt_Verlet1_nvt_v4)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, nHamiltonian& ham, bp::object py_funct, bp::object params, int ent_opt, Thermostat& therm)
  = &Verlet1_nvt;


  def("Verlet0_nvt", expt_Verlet0_nvt_v1);
  def("Verlet1_nvt", expt_Verlet1_nvt_v1);
  def("Verlet1_nvt", expt_Verlet1_nvt_v2);
  def("Verlet1_nvt", expt_Verlet1_nvt_v3);
  def("Verlet1_nvt", expt_Verlet1_nvt_v4);

}

void export_Ehrenfest_objects(){


  void (*expt_Ehrenfest0_v1)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, nHamiltonian& ham,
  bp::object py_funct, bp::object params, int rep) = &Ehrenfest0;
  void (*expt_Ehrenfest1_v1)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, nHamiltonian& ham,
  bp::object py_funct, bp::object params, int rep) = &Ehrenfest1;

  void (*expt_Ehrenfest2_v1)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, 
  nHamiltonian& ham, bp::object py_funct, bp::object params, int rep, 
  int do_reordering, int do_phase_correction) = &Ehrenfest2; 
  void (*expt_Ehrenfest2_v2)(double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, 
  nHamiltonian& ham, bp::object py_funct, bp::object params, int rep) = &Ehrenfest2; 


  def("Ehrenfest0", expt_Ehrenfest0_v1);
  def("Ehrenfest1", expt_Ehrenfest1_v1);
  def("Ehrenfest2", expt_Ehrenfest2_v1);
  def("Ehrenfest2", expt_Ehrenfest2_v2);

}


void export_LZ_hopping_probabilities_objects(){


  MATRIX (*expt_compute_hopping_probabilities_lz_v1)
  (nHamiltonian& ham, int rep, MATRIX& p, const MATRIX& invM, MATRIX& prev_ham_dia) = &compute_hopping_probabilities_lz;

  def("compute_hopping_probabilities_lz",expt_compute_hopping_probabilities_lz_v1);

}

void export_FSSH_hopping_probabilities_objects(){


  MATRIX (*expt_compute_hopping_probabilities_fssh_v1)
  (CMATRIX& Coeff, CMATRIX& Hvib, double dt, int use_boltz_factor, double T) = &compute_hopping_probabilities_fssh;

  MATRIX (*expt_compute_hopping_probabilities_fssh_v2)
  (CMATRIX& Coeff, CMATRIX& Hvib, double dt) = &compute_hopping_probabilities_fssh;

  MATRIX (*expt_compute_hopping_probabilities_fssh_v3)
  (CMATRIX& Coeff, nHamiltonian& ham, int rep, double dt, int use_boltz_factor, double T) = &compute_hopping_probabilities_fssh;

  MATRIX (*expt_compute_hopping_probabilities_fssh_v4)
  (CMATRIX& Coeff, nHamiltonian& ham, int rep, double dt) = &compute_hopping_probabilities_fssh;

  void (*expt_compute_hopping_probabilities_fssh_v5)
  (Nuclear& mol, Electronic& el, Hamiltonian& ham, MATRIX& g,
   double dt, int use_boltz_factor,double T) = &compute_hopping_probabilities_fssh;

  void (*expt_compute_hopping_probabilities_fssh_v6)
  (Ensemble& ens, int i, MATRIX& g,
   double dt, int use_boltz_factor,double T) = &compute_hopping_probabilities_fssh;

  def("compute_hopping_probabilities_fssh",expt_compute_hopping_probabilities_fssh_v1);
  def("compute_hopping_probabilities_fssh",expt_compute_hopping_probabilities_fssh_v2);
  def("compute_hopping_probabilities_fssh",expt_compute_hopping_probabilities_fssh_v3);
  def("compute_hopping_probabilities_fssh",expt_compute_hopping_probabilities_fssh_v4);
  def("compute_hopping_probabilities_fssh",expt_compute_hopping_probabilities_fssh_v5);
  def("compute_hopping_probabilities_fssh",expt_compute_hopping_probabilities_fssh_v6);


}


void export_GFSH_hopping_probabilities_objects(){

  MATRIX (*expt_compute_hopping_probabilities_gfsh_v1)
  (CMATRIX& Coeff, CMATRIX& Hvib, double dt, int use_boltz_factor, double T) = &compute_hopping_probabilities_gfsh;

  MATRIX (*expt_compute_hopping_probabilities_gfsh_v2)
  (CMATRIX& Coeff, CMATRIX& Hvib, double dt) = &compute_hopping_probabilities_gfsh;

  MATRIX (*expt_compute_hopping_probabilities_gfsh_v3)
  (CMATRIX& Coeff, nHamiltonian& ham, int rep, double dt, int use_boltz_factor, double T) = &compute_hopping_probabilities_gfsh;

  MATRIX (*expt_compute_hopping_probabilities_gfsh_v4)
  (CMATRIX& Coeff, nHamiltonian& ham, int rep, double dt) = &compute_hopping_probabilities_gfsh;

  void (*expt_compute_hopping_probabilities_gfsh_v5)
  (Nuclear& mol, Electronic& el, Hamiltonian& ham, MATRIX& g,
   double dt, int use_boltz_factor,double T) = &compute_hopping_probabilities_gfsh;

  void (*expt_compute_hopping_probabilities_gfsh_v6)
  (Ensemble& ens, int i, MATRIX& g,
   double dt, int use_boltz_factor,double T) = &compute_hopping_probabilities_gfsh;

  def("compute_hopping_probabilities_gfsh",expt_compute_hopping_probabilities_gfsh_v1);
  def("compute_hopping_probabilities_gfsh",expt_compute_hopping_probabilities_gfsh_v2);
  def("compute_hopping_probabilities_gfsh",expt_compute_hopping_probabilities_gfsh_v3);
  def("compute_hopping_probabilities_gfsh",expt_compute_hopping_probabilities_gfsh_v4);
  def("compute_hopping_probabilities_gfsh",expt_compute_hopping_probabilities_gfsh_v5);
  def("compute_hopping_probabilities_gfsh",expt_compute_hopping_probabilities_gfsh_v6);

}


void export_MSSH_hopping_probabilities_objects(){

  MATRIX (*expt_compute_hopping_probabilities_mssh_v1)
  (CMATRIX& Coeff)  = &compute_hopping_probabilities_mssh;

  void (*expt_compute_hopping_probabilities_mssh_v2)
  (Nuclear& mol, Electronic& el, Hamiltonian& ham, MATRIX& g,
   double dt, int use_boltz_factor,double T) = &compute_hopping_probabilities_mssh;

  void (*expt_compute_hopping_probabilities_mssh_v3)
  (Ensemble& ens, int i, MATRIX& g,
   double dt, int use_boltz_factor,double T) = &compute_hopping_probabilities_mssh;

  MATRIX (*expt_compute_hopping_probabilities_mssh_v4)
    (CMATRIX& Coeff, CMATRIX* Hvib, int use_boltz_factor,double T)  = &compute_hopping_probabilities_mssh;

  def("compute_hopping_probabilities_mssh",expt_compute_hopping_probabilities_mssh_v1);
  def("compute_hopping_probabilities_mssh",expt_compute_hopping_probabilities_mssh_v2);
  def("compute_hopping_probabilities_mssh",expt_compute_hopping_probabilities_mssh_v3);
  def("compute_hopping_probabilities_mssh",expt_compute_hopping_probabilities_mssh_v4);

}

void export_ESH_hopping_probabilities_objects(){


  void (*expt_compute_hopping_probabilities_esh_v1)
  (Ensemble& ens, MATRIX* g, double dt, int use_boltz_factor,double T) = compute_hopping_probabilities_esh;

  def("compute_hopping_probabilities_esh", expt_compute_hopping_probabilities_esh_v1);

}


void export_tsh_aux_rescale_objects(){


  int (*expt_apply_transition0_v1)
  (MATRIX& p, MATRIX& invM, nHamiltonian& ham, int istate, int fstate, 
  int vel_rescale_opt, int do_reverse,int do_rescale) = &apply_transition0;

  def("apply_transition0", expt_apply_transition0_v1);

  vector<int> (*expt_apply_transition1_v1)
  (MATRIX& p, MATRIX& invM, nHamiltonian& ham, vector<int>& istate, 
  vector<int>& fstate, int vel_rescale_opt, int do_reverse, int do_rescale) = &apply_transition1;

  def("apply_transition1", expt_apply_transition1_v1);




  int (*expt_rescale_velocities_adiabatic_v1)
  (MATRIX& p, MATRIX& invM, CMATRIX& ham_adi, vector<CMATRIX>& dc1_adi, 
  int new_st,int old_st, int do_reverse, int do_rescale) = &rescale_velocities_adiabatic;

  int (*expt_rescale_velocities_adiabatic_v2)
  (MATRIX& p, MATRIX& invM, CMATRIX& ham_adi, vector<CMATRIX>& dc1_adi, 
  int new_st,int old_st, int do_reverse) = &rescale_velocities_adiabatic;


  int (*expt_rescale_velocities_adiabatic_v3)
  (MATRIX& p, MATRIX& invM, nHamiltonian& ham, int new_st,int old_st,
  int do_reverse, int do_rescale) = &rescale_velocities_adiabatic;

  int (*expt_rescale_velocities_adiabatic_v4)
  (MATRIX& p, MATRIX& invM, nHamiltonian& ham, int new_st,int old_st,
  int do_reverse) = &rescale_velocities_adiabatic;


  int (*expt_rescale_velocities_adiabatic_v5)
  (vector<double>& p, vector<double>& masses, CMATRIX& ham_adi,
   vector<CMATRIX>& dc1_adi, int new_st,int old_st, int do_reverse) = &rescale_velocities_adiabatic;

  int (*expt_rescale_velocities_adiabatic_v6)
  (vector<double>& p, vector<double>& masses, CMATRIX& ham_adi,
   vector<CMATRIX>& dc1_adi, int new_st,int old_st, int do_reverse) = &rescale_velocities_adiabatic;

  int (*expt_rescale_velocities_adiabatic_v7)
  (Nuclear& mol, Hamiltonian& ham, int old_st, int do_reverse) = &rescale_velocities_adiabatic;

  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v1);
  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v2);
  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v3);
  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v4);
  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v5);
  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v6);
  def("rescale_velocities_adiabatic", expt_rescale_velocities_adiabatic_v7);




  int (*expt_rescale_velocities_diabatic_v1)
  (MATRIX& p, MATRIX& invM, CMATRIX& ham_adi, int new_st,int old_st, int do_rescale) = &rescale_velocities_diabatic;

  int (*expt_rescale_velocities_diabatic_v2)
  (MATRIX& p, MATRIX& invM, CMATRIX& ham_adi, int new_st,int old_st) = &rescale_velocities_diabatic;

  int (*expt_rescale_velocities_diabatic_v3)
  (MATRIX& p, MATRIX& invM, nHamiltonian& ham, int new_st,int old_st, int do_rescale) = &rescale_velocities_diabatic;

  int (*expt_rescale_velocities_diabatic_v4)
  (MATRIX& p, MATRIX& invM, nHamiltonian& ham, int new_st,int old_st) = &rescale_velocities_diabatic;

  int (*expt_rescale_velocities_diabatic_v5)
  (Nuclear& mol, Hamiltonian& ham, int old_st) = &rescale_velocities_diabatic;

  def("rescale_velocities_diabatic", expt_rescale_velocities_diabatic_v1);
  def("rescale_velocities_diabatic", expt_rescale_velocities_diabatic_v2);
  def("rescale_velocities_diabatic", expt_rescale_velocities_diabatic_v3);
  def("rescale_velocities_diabatic", expt_rescale_velocities_diabatic_v4);
  def("rescale_velocities_diabatic", expt_rescale_velocities_diabatic_v5);



}



void export_tsh_aux_hop_objects(){

  vector<int> (*expt_tsh_vec2indx_v1)(CMATRIX& states) = &tsh_vec2indx;
  def("tsh_vec2indx", expt_tsh_vec2indx_v1);

  void (*expt_tsh_indx2vec_v1)(nHamiltonian& ham, CMATRIX& states, vector<int>& res) = &tsh_indx2vec;
  def("tsh_indx2vec", expt_tsh_indx2vec_v1);

  void (*expt_tsh_internal2physical_v1)
  (nHamiltonian& ham, vector<int>& internal, vector<int>& physical) = &tsh_internal2physical;
  def("tsh_internal2physical", expt_tsh_internal2physical_v1);

  void (*expt_tsh_physical2internal_v1)
  (nHamiltonian& ham, vector<int>& internal, vector<int>& physical) = &tsh_physical2internal;
  def("tsh_physical2internal", expt_tsh_physical2internal_v1);



//  CMATRIX (*expt_compute_phases_v1)(CMATRIX& U, CMATRIX& U_prev) = &compute_phases;
//  def("compute_phases", expt_compute_phases_v1);

  void (*expt_phase_correct_ampl_v1)
  (CMATRIX& C, CMATRIX& cum_phases, CMATRIX& cum_phases_prev) = &phase_correct_ampl;
  void (*expt_phase_correct_ampl_v2)
  (CMATRIX& C, CMATRIX& phase) = &phase_correct_ampl;

  def("phase_correct_ampl", expt_phase_correct_ampl_v1);
  def("phase_correct_ampl", expt_phase_correct_ampl_v2);
  

  int (*expt_hop_v1)
  (int initstate, Nuclear& mol, Hamiltonian& ham, double ksi, MATRIX& g, int do_rescaling, int rep, int do_reverse) = &hop;
  int (*expt_hop_v2)
  (int initstate, Ensemble& ens, int i, double ksi, MATRIX& g, int do_rescaling, int rep, int do_reverse) = &hop;
  vector<int> (*expt_hop_v3)
  (int ntraj, vector<int> initstate, vector<Nuclear>& mol, vector<Hamiltonian>& ham, 
   vector<double> ksi, vector<MATRIX>& g, int do_rescaling, int rep, int do_reverse) = &hop;
  int (*expt_hop_v4)(int initstate, MATRIX& g, double ksi) = &hop;

  def("hop", expt_hop_v1);
  def("hop", expt_hop_v2);
  def("hop", expt_hop_v3);
  def("hop", expt_hop_v4);

}


void export_decoherence_objects(){
 
  //================== ID-A =======================

  int (*expt_ida_v1)(CMATRIX& Coeff, int old_st, int new_st, double E_old, double E_new, double T, double ksi) = &ida;
  def("ida", expt_ida_v1);



  //=============== SDM & MSDM ====================

  CMATRIX (*expt_sdm_v1)
  (CMATRIX& Coeff, double dt, int act_st, vector<double>& En, double Ekin, double C_param, double eps_param) = &sdm;

  Electronic (*expt_sdm_v2)
  (Electronic& Coeff, double dt, int act_st, vector<double>& En, double Ekin, double C_param, double eps_param) = &sdm;

  def("sdm", expt_sdm_v1);
  def("sdm", expt_sdm_v2);



  CMATRIX (*expt_msdm_v1)
  (CMATRIX& Coeff, double dt, int act_st, MATRIX& decoh_rates) = &msdm;

  CMATRIX (*expt_msdm_v2)
  (CMATRIX& Coeff, double dt, vector<int>& act_st, MATRIX& decoh_rates) = &msdm;

  Electronic (*expt_msdm_v3)
  (Electronic& Coeff, double dt, int act_st, MATRIX& decoh_rates) = &msdm;

  def("msdm", expt_msdm_v1);
  def("msdm", expt_msdm_v2);
  def("msdm", expt_msdm_v3);



  //================== DISH =======================

  void (*expt_project_out_v1)(CMATRIX& Coeff, int i) = &project_out;
  void (*expt_collapse_v1)(CMATRIX& Coeff, int i) = &collapse;

  MATRIX (*expt_coherence_intervals_v1)(CMATRIX& Coeff, MATRIX& rates) = &coherence_intervals;

  int (*expt_dish_v1)(Electronic& el, MATRIX& t_m, const MATRIX& tau_m, const CMATRIX& Hvib,
          int use_boltz_flag, double Ekin, double T, double ksi1, double ksi2) = &dish;

  int (*expt_dish_v2)(Electronic& el, Nuclear& mol, Hamiltonian& ham, 
          MATRIX& t_m, const MATRIX& tau_m, int use_boltz_flag, double T, double ksi1, double ksi2) = &dish;


  def("project_out", expt_project_out_v1);
  def("collapse", expt_collapse_v1);
  def("coherence_intervals", expt_coherence_intervals_v1);
  def("dish", expt_dish_v1);
  def("dish", expt_dish_v2);


}

void export_permutation_objects(){
 
  vector<int> (*expt_get_permutation_v1)(vector<vector<int> >& inp) = &get_permutation;
  vector<int> (*expt_Munkres_Kuhn_minimize_v1)(MATRIX& _X, int verbosity) = &Munkres_Kuhn_minimize;
  vector<int> (*expt_Munkres_Kuhn_maximize_v1)(MATRIX& _X, int verbosity) = &Munkres_Kuhn_maximize;

  def("get_permutation", expt_get_permutation_v1);  
  def("Munkres_Kuhn_minimize", expt_Munkres_Kuhn_minimize_v1);  
  def("Munkres_Kuhn_maximize", expt_Munkres_Kuhn_maximize_v1);  


  vector<int> (*expt_get_reordering_v1)(CMATRIX& time_overlap) = &get_reordering;
  MATRIX (*expt_make_cost_mat_v1)(CMATRIX& orb_mat_inp, CMATRIX& en_mat_inp, double alpha) = &make_cost_mat;
  vector<int> (*expt_Munkres_Kuhn_v1)(CMATRIX& orb_mat_inp, CMATRIX& en_mat_inp, double alpha, int verbosity) = &Munkres_Kuhn;

  def("get_reordering", expt_get_reordering_v1);  
  def("make_cost_mat", expt_make_cost_mat_v1);  
  def("Munkres_Kuhn", expt_Munkres_Kuhn_v1);  

}

void export_dyn_control_params_objects(){

  // Arbitrary wavefunction
  void (dyn_control_params::*expt_sanity_check_v1)(boost::python::dict params) = &dyn_control_params::set_parameters;
  void (dyn_control_params::*expt_set_parameters_v1)(boost::python::dict params) = &dyn_control_params::set_parameters;


  class_<dyn_control_params>("dyn_control_params",init<>())
      .def("__copy__", &generic__copy__<dyn_control_params>)
      .def("__deepcopy__", &generic__deepcopy__<dyn_control_params>)

      .def_readwrite("rep_tdse", &dyn_control_params::rep_tdse)
      .def_readwrite("rep_ham", &dyn_control_params::rep_ham)
      .def_readwrite("rep_sh", &dyn_control_params::rep_sh)
      .def_readwrite("rep_lz", &dyn_control_params::rep_lz)
      .def_readwrite("tsh_method", &dyn_control_params::tsh_method)
      .def_readwrite("force_method", &dyn_control_params::force_method)
      .def_readwrite("nac_update_method", &dyn_control_params::nac_update_method)
      .def_readwrite("rep_force", &dyn_control_params::rep_force)
      .def_readwrite("use_boltz_factor", &dyn_control_params::use_boltz_factor)
      .def_readwrite("Temperature", &dyn_control_params::Temperature)
      .def_readwrite("do_reverse", &dyn_control_params::do_reverse)
      .def_readwrite("vel_rescale_opt", &dyn_control_params::vel_rescale_opt)
      .def_readwrite("dt", &dyn_control_params::dt)
      .def_readwrite("do_phase_correction", &dyn_control_params::do_phase_correction)
      .def_readwrite("state_tracking_algo", &dyn_control_params::state_tracking_algo)
      .def_readwrite("MK_alpha", &dyn_control_params::MK_alpha)
      .def_readwrite("MK_verbosity", &dyn_control_params::MK_verbosity)
      .def_readwrite("entanglement_opt", &dyn_control_params::entanglement_opt)
      .def_readwrite("ETHD3_alpha", &dyn_control_params::ETHD3_alpha)
      .def_readwrite("ETHD3_beta", &dyn_control_params::ETHD3_beta)
      .def_readwrite("decoherence_algo", &dyn_control_params::decoherence_algo)


      .def("sanity_check", expt_sanity_check_v1)
      .def("set_parameters", expt_set_parameters_v1)
  ;
}


void export_Dyn_objects(){
/** 
  \brief Exporter of libdyn classes and functions

*/


  export_Nuclear_objects();
//  export_RigidBody_objects();
  export_Electronic_objects();
  export_Thermostat_objects();
  export_Barostat_objects();
  export_Wfcgrid_objects();
  export_Wfcgrid2_objects();
  export_Ensemble_objects();
  export_gwp_objects();

  
  export_Verlet_objects();
  export_Ehrenfest_objects();

  export_LZ_hopping_probabilities_objects();
  export_FSSH_hopping_probabilities_objects();
  export_GFSH_hopping_probabilities_objects();
  export_MSSH_hopping_probabilities_objects();
  export_ESH_hopping_probabilities_objects();

  export_tsh_aux_hop_objects();
  export_tsh_aux_rescale_objects();

  export_decoherence_objects();

  export_permutation_objects();

  export_dyn_control_params_objects();


  double (*expt_compute_kinetic_energy_v1)(MATRIX& p, MATRIX& invM) = &compute_kinetic_energy;
  double (*expt_compute_kinetic_energy_v2)(Nuclear& mol) = &compute_kinetic_energy;
  double (*expt_compute_kinetic_energy_v3)(Ensemble& ens) = &compute_kinetic_energy;

  def("compute_kinetic_energy",expt_compute_kinetic_energy_v1);
  def("compute_kinetic_energy",expt_compute_kinetic_energy_v2);
  def("compute_kinetic_energy",expt_compute_kinetic_energy_v3);



  double (*expt_compute_potential_energy_v1)(Nuclear& mol, Electronic& el, Hamiltonian& ham, int opt) = &compute_potential_energy;
  double (*expt_compute_potential_energy_v2)(Ensemble& ens, int opt) = &compute_potential_energy;
  double (*expt_compute_forces_v1)(Nuclear& mol, Electronic& el, Hamiltonian& ham, int opt) = &compute_forces;
  double (*expt_compute_forces_v2)(Ensemble& ens, int opt) = &compute_forces;

  def("compute_potential_energy",expt_compute_potential_energy_v1);
  def("compute_potential_energy",expt_compute_potential_energy_v2);
  def("compute_forces",expt_compute_forces_v1);
  def("compute_forces",expt_compute_forces_v2);


  void (*expt_propagate_ensemble_v1)(double dt,Ensemble& ens,int opt) = &propagate_ensemble;
  def("propagate_ensemble", expt_propagate_ensemble_v1);

/*
  int (*expt_tsh0_v1)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, int state, nHamiltonian& ham, 
   bp::object py_funct, bp::object params,  boost::python::dict params1, Random& rnd, 
   int do_reordering, int do_phase_correction) = &tsh0;
  int (*expt_tsh0_v2)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, int state, nHamiltonian& ham,
   bp::object py_funct, bp::object params,  boost::python::dict params1, Random& rnd) = &tsh0;
  def("tsh0", expt_tsh0_v1);
  def("tsh0", expt_tsh0_v2);
*/

  void (*expt_tsh1_v1)
  (MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, vector<int>& states, nHamiltonian& ham,
   bp::object py_funct, bp::object params,  boost::python::dict params1, Random& rnd) = &tsh1;
  def("tsh1", expt_tsh1_v1);


  void (*expt_tsh1b_v1)
  (MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, vector<int>& states, nHamiltonian& ham,
   bp::object py_funct, bp::object params,  boost::python::dict params1, Random& rnd) = &tsh1b;
  def("tsh1b", expt_tsh1b_v1);


  void (*expt_tsh2_v1)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, vector<int>& act_states, nHamiltonian& ham,
   bp::object py_funct, bp::object params, boost::python::dict params1, Random& rnd) = &tsh2;
  def("tsh2", expt_tsh2_v1);

  void (*expt_tsh2a_v1)
  (double dt, MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, vector<int>& act_states,
   nHamiltonian& ham, bp::object py_funct, bp::object params, boost::python::dict params1, Random& rnd) = &tsh2a;
  def("tsh2a", expt_tsh2a_v1);


  //============= Dynamics.cpp ======================

  void (*expt_update_Hamiltonian_q_v1)
  (dyn_control_params& prms, MATRIX& q, nHamiltonian& ham, 
   bp::object py_funct, bp::object model_params) = &update_Hamiltonian_q;

  void (*expt_update_Hamiltonian_q_v2)
  (bp::dict prms, MATRIX& q, nHamiltonian& ham, 
   bp::object py_funct, bp::object model_params) = &update_Hamiltonian_q;

  def("update_Hamiltonian_q", expt_update_Hamiltonian_q_v1);
  def("update_Hamiltonian_q", expt_update_Hamiltonian_q_v2);


  void (*expt_update_Hamiltonian_p_v1)
  (dyn_control_params& prms, nHamiltonian& ham, MATRIX& p, MATRIX& invM) = &update_Hamiltonian_p;

  void (*expt_update_Hamiltonian_p_v2)
  (bp::dict prms, nHamiltonian& ham, MATRIX& p, MATRIX& invM) = &update_Hamiltonian_p;

  def("update_Hamiltonian_p", expt_update_Hamiltonian_p_v1);
  def("update_Hamiltonian_p", expt_update_Hamiltonian_p_v2);


  CMATRIX (*expt_transform_amplitudes_v1)
  (int rep_in, int rep_out, CMATRIX& C, nHamiltonian& ham) = &transform_amplitudes;

  def("transform_amplitudes", expt_transform_amplitudes_v1);


  void (*expt_do_surface_hopping_v1)
  (dyn_control_params& prms, MATRIX& q, MATRIX& p, MATRIX& invM,
   CMATRIX& C, vector<int>& act_states, nHamiltonian& ham, 
   vector<MATRIX>& prev_ham_dia, Random& rnd) = &do_surface_hopping;

  void (*expt_do_surface_hopping_v2)
  (bp::dict prms, MATRIX& q, MATRIX& p, MATRIX& invM, 
   CMATRIX& C, vector<int>& act_states, nHamiltonian& ham,
   vector<MATRIX>& prev_ham_dia, Random& rnd) = &do_surface_hopping;

  def("do_surface_hopping", expt_do_surface_hopping_v1);
  def("do_surface_hopping", expt_do_surface_hopping_v2);


  void (*expt_compute_dynamics_v1)
  (MATRIX& q, MATRIX& p, MATRIX& invM, CMATRIX& C, vector<int>& act_states,
   nHamiltonian& ham, bp::object py_funct, bp::dict model_params, 
   bp::dict dyn_params, Random& rnd) = &compute_dynamics;
  def("compute_dynamics", expt_compute_dynamics_v1);


}// export_Dyn_objects()




#ifdef CYGWIN
BOOST_PYTHON_MODULE(cygdyn){
#else
BOOST_PYTHON_MODULE(libdyn){
#endif

  // Register converters:
  // See here: https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/
  //to_python_converter<std::vector<DATA>, VecToList<DATA> >();

  export_Dyn_objects();

}


}// libdyn
}// liblibra

