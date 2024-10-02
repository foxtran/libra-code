#*********************************************************************************
#* Copyright (C) 2024 Alexey V. Akimov
#*
#* This file is distributed under the terms of the GNU General Public License
#* as published by the Free Software Foundation, either version 3 of
#* the License, or (at your option) any later version.
#* See the file LICENSE in the root directory of this distribution
#* or <http://www.gnu.org/licenses/>.
#*
#*********************************************************************************/

#import os
#import sys
#import math
#import time
import pytest

from liblibra_core import *


set1 = [
[0.0, 0.0, 0, 0.0, 0, 1.0, 0.0 ],
[0.0, 0.0, 0, 0.0, 1, 1.0, 0.0 ],
[0.0, 0.0, 0, 0.5, 0, 1.0, 0.0 ],
[0.0, 0.0, 0, 0.5, 1, 1.0, 0.0 ],
[0.0, 0.0, 0, 1.0, 0, 1.0, 0.0 ],
[0.0, 0.0, 0, 1.0, 1, 1.0, 0.0 ],
[0.0, 0.0, 1, 0.0, 0, 0.0, 1.0 ],
[0.0, 0.0, 1, 0.0, 1, 0.0, 1.0 ],
[0.0, 0.0, 1, 0.5, 0, 0.0, 1.0 ],
[0.0, 0.0, 1, 0.5, 1, 0.0, 1.0 ],
[0.0, 0.0, 1, 1.0, 0, 0.0, 1.0 ],
[0.0, 0.0, 1, 1.0, 1, 0.0, 1.0 ],
[0.0, 0.1, 0, 0.0, 0, 1.0, 0.0 ],
[0.0, 0.1, 0, 0.0, 1, 1.0, 0.0 ],
[0.0, 0.1, 0, 0.5, 0, 0.0, 1.0 ],
[0.0, 0.1, 0, 0.5, 1, 0.0, 1.0 ],
[0.0, 0.1, 0, 1.0, 0, 0.0, 1.0 ],
[0.0, 0.1, 0, 1.0, 1, 0.0, 1.0 ],
[0.0, 0.1, 1, 0.0, 0, 0.0, 1.0 ],
[0.0, 0.1, 1, 0.0, 1, 0.0, 1.0 ],
[0.0, 0.1, 1, 0.5, 0, 0.0, 1.0 ],
[0.0, 0.1, 1, 0.5, 1, 0.0, 1.0 ],
[0.0, 0.1, 1, 1.0, 0, 0.0, 1.0 ],
[0.0, 0.1, 1, 1.0, 1, 0.0, 1.0 ],
[0.01, 0.0, 0, 0.0, 0, 1.0, 0.0 ],
[0.01, 0.0, 0, 0.0, 1, 1.0, 0.0 ],
[0.01, 0.0, 0, 0.5, 0, 1.0, 0.0 ],
[0.01, 0.0, 0, 0.5, 1, 1.0, 0.0 ],
[0.01, 0.0, 0, 1.0, 0, 1.0, 0.0 ],
[0.01, 0.0, 0, 1.0, 1, 1.0, 0.0 ],
[0.01, 0.0, 1, 0.0, 0, 0.0, 1.0 ],
[0.01, 0.0, 1, 0.0, 1, 0.0, 1.0 ],
[0.01, 0.0, 1, 0.5, 0, 0.0, 1.0 ],
[0.01, 0.0, 1, 0.5, 1, 0.0, 1.0 ],
[0.01, 0.0, 1, 1.0, 0, 0.0, 1.0 ],
[0.01, 0.0, 1, 1.0, 1, 0.0, 1.0 ],
[0.01, 0.1, 0, 0.0, 0, 1.0, 0.0 ],
[0.01, 0.1, 0, 0.0, 1, 1.0, 0.0 ],
[0.01, 0.1, 0, 0.5, 0, 0.0, 1.0 ],
[0.01, 0.1, 0, 0.5, 1, 0.9997799598333882, 0.00022004016661186391 ],
[0.01, 0.1, 0, 1.0, 0, 0.0, 1.0 ],
[0.01, 0.1, 0, 1.0, 1, 0.999889979916694, 0.00011002008330593196 ],
[0.01, 0.1, 1, 0.0, 0, 0.0, 1.0 ],
[0.01, 0.1, 1, 0.0, 1, 0.0, 1.0 ],
[0.01, 0.1, 1, 0.5, 0, 0.0, 1.0 ],
[0.01, 0.1, 1, 0.5, 1, 0.0, 1.0 ],
[0.01, 0.1, 1, 1.0, 0, 0.0, 1.0 ],
[0.01, 0.1, 1, 1.0, 1, 0.0, 1.0 ],
[0.1, 0.0, 0, 0.0, 0, 1.0, 0.0 ],
[0.1, 0.0, 0, 0.0, 1, 1.0, 0.0 ],
[0.1, 0.0, 0, 0.5, 0, 1.0, 0.0 ],
[0.1, 0.0, 0, 0.5, 1, 1.0, 0.0 ],
[0.1, 0.0, 0, 1.0, 0, 1.0, 0.0 ],
[0.1, 0.0, 0, 1.0, 1, 1.0, 0.0 ],
[0.1, 0.0, 1, 0.0, 0, 0.0, 1.0 ],
[0.1, 0.0, 1, 0.0, 1, 0.0, 1.0 ],
[0.1, 0.0, 1, 0.5, 0, 0.0, 1.0 ],
[0.1, 0.0, 1, 0.5, 1, 0.0, 1.0 ],
[0.1, 0.0, 1, 1.0, 0, 0.0, 1.0 ],
[0.1, 0.0, 1, 1.0, 1, 0.0, 1.0 ],
[0.1, 0.1, 0, 0.0, 0, 1.0, 0.0 ],
[0.1, 0.1, 0, 0.0, 1, 1.0, 0.0 ],
[0.1, 0.1, 0, 0.5, 0, 0.0, 1.0 ],
[0.1, 0.1, 0, 0.5, 1, 1.0, 1.5874314482013768e-45 ],
[0.1, 0.1, 0, 1.0, 0, 0.0, 1.0 ],
[0.1, 0.1, 0, 1.0, 1, 1.0, 7.937157241006884e-46 ],
[0.1, 0.1, 1, 0.0, 0, 0.0, 1.0 ],
[0.1, 0.1, 1, 0.0, 1, 0.0, 1.0 ],
[0.1, 0.1, 1, 0.5, 0, 0.0, 1.0 ],
[0.1, 0.1, 1, 0.5, 1, 0.0, 1.0 ],
[0.1, 0.1, 1, 1.0, 0, 0.0, 1.0 ],
[0.1, 0.1, 1, 1.0, 1, 0.0, 1.0 ]
]

#initstate', 'gvector', 'ksi', 'res_expt'

set2 = [
 [ 0, [0.0, 1.0], 0.0, 1 ],
 [ 0, [0.0, 1.0], 0.1, 1 ],
 [ 0, [0.2, 0.8], 0.1, 0 ],
 [ 0, [0.2, 0.8], 0.5, 1 ],
 [ 0, [1.0, 0.0], 0.2, 0 ],
 [ 1, [0.0, 1.0], 0.0, 1 ],
 [ 1, [0.0, 1.0], 0.1, 1 ],
 [ 1, [0.2, 0.8], 0.1, 0 ],
 [ 1, [0.2, 0.8], 0.5, 1 ],
 [ 1, [1.0, 0.0], 0.2, 0 ],
 [ 0, [0.1, 0.3, 0.6], 0.0, 0 ],
 [ 0, [0.1, 0.3, 0.6], 0.2, 1 ],
 [ 0, [0.1, 0.3, 0.6], 0.7, 2 ],
 [ 1, [0.1, 0.3, 0.6], 0.0, 0 ],
 [ 1, [0.1, 0.3, 0.6], 0.2, 1 ],
 [ 1, [0.1, 0.3, 0.6], 0.7, 2 ],
 [ 2, [0.1, 0.3, 0.6], 0.0, 0 ],
 [ 2, [0.1, 0.3, 0.6], 0.2, 1 ],
 [ 2, [0.1, 0.3, 0.6], 0.7, 2 ]
]


class TestSpecialFunctions: #(unittest.TestCase):


    def gen_1(self, dE, d, istate, p0, bf):
        """Test FSSH hopping probability. This function is to generate ref data"""

        Hvib = CMATRIX(2,2);
        prms = dyn_control_params();  
        prms.use_boltz_factor = bf;
        prms.Temperature = 300.0;

        Hvib = CMATRIX(2,2); 
        Hvib.set(0,0, 0.0+0.0j);    Hvib.set(0, 1, (0.0-1j)*d);
        Hvib.set(1,0, (0.0+1j)*d);  Hvib.set(1, 1, (1.0+0.0j)*dE);

        denmat = CMATRIX(2, 2); 
        denmat.set(0,0, p0*(1.0+0.0j)); denmat.set(0,1, 1.0+0.0j)
        denmat.set(1,0, 0.0+0.0j);      denmat.set(1,1, (1.0 - p0)*(1.0+0.0j))

        g = hopping_probabilities_fssh(prms, denmat, Hvib, istate);

        return g[0], g[1]  #print(g[0], g[1])
        #self.assertAlmostEqual(out[i][1], out_ref[i][1])

    @pytest.mark.parametrize(('dE', 'd', 'istate', 'p0', 'bf', 'g0', 'g1'), set1)
    def test_1(self, dE, d, istate, p0, bf, g0, g1):

        Hvib = CMATRIX(2,2);
        prms = dyn_control_params();
        prms.use_boltz_factor = bf;
        prms.Temperature = 300.0;

        Hvib = CMATRIX(2,2);
        Hvib.set(0,0, 0.0+0.0j);    Hvib.set(0, 1, (0.0-1j)*d);
        Hvib.set(1,0, (0.0+1j)*d);  Hvib.set(1, 1, (1.0+0.0j)*dE);

        denmat = CMATRIX(2, 2);
        denmat.set(0,0, p0*(1.0+0.0j)); denmat.set(0,1, 1.0+0.0j)
        denmat.set(1,0, 0.0+0.0j);      denmat.set(1,1, (1.0 - p0)*(1.0+0.0j))

        g = hopping_probabilities_fssh(prms, denmat, Hvib, istate);
        assert abs(g[0] - g0) < 1e-10
        assert abs(g[1] - g1) < 1e-10


    @pytest.mark.parametrize(('initstate', 'gvector', 'ksi', 'res_expt'), set2)
    def test_2(self, initstate, gvector, ksi, res_expt):

        res = hop(initstate, Py2Cpp_double(gvector), ksi)

        assert res == res_expt

"""
To generate the set1 run the code below:

x = TestSpecialFunctions()
for dE in [0.0, 0.01, 0.1]:
    for d in [0.0, 0.1]:
        for istate in [0, 1]:
            for p0 in [0.0, 0.5, 1.0]:
                for bf in [0, 1]:
                    #print(F"dE = {dE}  d = {d}  istate = {istate} p0 = {p0}  bf = {bf}")
                    #print(F"[{dE}, {d}, {istate}, {p0}, {bf}, ],")
                    res = x.test_1(dE, d, istate, p0, bf)
                    print(F"[{dE}, {d}, {istate}, {p0}, {bf}, {res[0]}, {res[1] } ],")
"""


