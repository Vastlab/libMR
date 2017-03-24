#!/usr/bin/env python

import libmr
# See README.txt for help installing libmr.

import numpy as np
import matplotlib.pyplot as plt

mr = libmr.MR()
# Create some data
data = np.random.randn(100)
xs = np.linspace(-5,5, 100)

# Plot two plots
fig,(ax1,ax2) = plt.subplots(2,1)

ax1.set_title("Fitting low")
ax1.hist(data,bins=20,normed=True)
for tailsize in [10,30,50]:
    mr.fit_low(data, tailsize)
    assert mr.is_valid
    ax1.plot(xs, mr.w_score_vector(xs), label="Tailsize: %d"%tailsize)
ax1.legend()

ax2.set_title("Fitting high")
ax2.hist(data,bins=20,normed=True)
for tailsize in [10,30,50]:
    mr.fit_high(data, tailsize)
    assert mr.is_valid
    ax2.plot(xs, mr.w_score_vector(xs), label="Tailsize: %d"%tailsize)
ax2.legend()

plt.tight_layout()
plt.show()
raw_input("Press Enter to close")
