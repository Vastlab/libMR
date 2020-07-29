#!/usr/bin/env python

import libmr
# See README.txt for help installing libmr.

import numpy as np

mr = libmr.MR()
# Create some data
data = np.random.randn(1000)
for tailsize in [10,30,50,100,500]:
    mr.fit_low(data, tailsize)
    assert mr.is_valid
    print("tailsize scale shape sign translate score  " , tailsize, mr.get_params());
    print("scale lb up shape lb up  " , mr.get_confidence());    

    mr.fit_high(data, tailsize)
    assert mr.is_valid
    print("scale shape sign translate score  " , mr.get_params());
    print("scale lb up shape lb up  " , mr.get_confidence());

