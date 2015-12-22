#*********************************************************************************
#* Copyright (C) 2015 Alexey V. Akimov
#*
#* This file is distributed under the terms of the GNU General Public License
#* as published by the Free Software Foundation, either version 2 of
#* the License, or (at your option) any later version.
#* See the file LICENSE in the root directory of this distribution
#* or <http://www.gnu.org/licenses/>.
#*
#*********************************************************************************/
###################################################################
# Tutorial: SCF computations are hidden - use built-in function
###################################################################

import os
import sys
import math

# Fisrt, we add the location of the library to test to the PYTHON path
cwd = os.getcwd()
print "Current working directory", cwd
sys.path.insert(1,cwd+"/../../_build/src/mmath")
sys.path.insert(1,cwd+"/../../_build/src/chemobjects")
sys.path.insert(1,cwd+"/../../_build/src/hamiltonian")
sys.path.insert(1,cwd+"/../../_build/src/hamiltonian/Hamiltonian_Atomistic")
sys.path.insert(1,cwd+"/../../_build/src/hamiltonian/Hamiltonian_Atomistic/Hamiltonian_QM")
sys.path.insert(1,cwd+"/../../_build/src/hamiltonian/Hamiltonian_Atomistic/Hamiltonian_QM/Control_Parameters")
sys.path.insert(1,cwd+"/../../_build/src/hamiltonian/Hamiltonian_Atomistic/Hamiltonian_QM/Model_Parameters")
sys.path.insert(1,cwd+"/../../_build/src/hamiltonian/Hamiltonian_Atomistic/Hamiltonian_QM/Basis_Setups")
sys.path.insert(1,cwd+"/../../_build/src/dyn")
sys.path.insert(1,cwd+"/../../_build/src/qchem/qobjects")
sys.path.insert(1,cwd+"/../../_build/src/qchem/basis")
sys.path.insert(1,cwd+"/../../_build/src/converters")
sys.path.insert(1,cwd+"/../../_build/src/calculators")

print "\nTest 1: Importing the library and its content"
from cygmmath import *
from cygchemobjects import *
from cyghamiltonian import *
from cyghamiltonian_qm import *
from cygcontrol_parameters import *
from cygmodel_parameters import *
from cygbasis_setups import *
from cygdyn import *
from cygqobjects import *
from cygbasis import *
from cygconverters import *
from cygcalculators import *

from LoadPT import * # Load_PT
from LoadMolecule import * # Load_Molecule



#=========== STEP 1:  Create Universe and populate it ================
U = Universe()
Load_PT(U, "elements.dat", 1)


#=========== STEP 2:  Create system and load a molecule ================
syst = System()
#Load_Molecule(U, syst, os.getcwd()+"/c.pdb", "pdb_1")
#Load_Molecule(U, syst, os.getcwd()+"/c2.pdb", "pdb_1")
Load_Molecule(U, syst, os.getcwd()+"/bh.pdb", "pdb_1")
#Load_Molecule(U, syst, os.getcwd()+"/co.pdb", "pdb_1")
#Load_Molecule(U, syst, os.getcwd()+"/ch4.pdb", "pdb_1")


print "Number of atoms in the system = ", syst.Number_of_atoms
atlst1 = range(0,syst.Number_of_atoms)


#=========== STEP 3: Create control parameters (setting computation options) ================


# Creating Hamiltonian
ham = Hamiltonian_Atomistic(2, 3*syst.Number_of_atoms)
ham.set_Hamiltonian_type("QM")
ham.set_system(syst)
ham.init_qm_Hamiltonian("control_parameters_indo.dat")
#ham.init_qm_Hamiltonian("control_parameters_eht.dat")
ham.set_rep(1)
ham.add_excitation(0,1,1,1)


ham.compute()



for i in xrange(2):
    print "Energy = ", ham.H(i,i), " a.u."
print "Force 1 = ", -ham.dHdq(0,0,0), " a.u."
print "Force 3 = ", -ham.dHdq(0,0,3), " a.u."





#degen = 1.0
#kT = 0.025
#etol = 0.0001
#pop_opt = 0  #  0 -  integer populations,  1 - Fermi distribution              

#res_alp = Fock_to_P(el.get_Fao_alp(), el.get_Sao(), Nelec_alp, degen, kT, etol, pop_opt)
#res_bet = Fock_to_P(el.get_Fao_alp(), el.get_Sao(), Nelec_bet, degen, kT, etol, pop_opt)


#print "Bands(alp)    Occupations(alp)       Bands(bet)    Occupations(bet)"
#for j in xrange(Norb):
#     print "%12.8f   %12.8f  %12.8f   %12.8f" %(res_alp[3][j][1], res_alp[4][j][1], res_bet[3][j][1], res_bet[4][j][1])

    






