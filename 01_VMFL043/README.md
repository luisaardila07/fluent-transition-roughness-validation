# VMFL043 — Flat-plate transition benchmark

This case reproduces the ANSYS VMFL043 benchmark using the Transition SST model in ANSYS Fluent. It was used to assess the predicted laminar-to-turbulent transition over a smooth flat plate by comparing the skin-friction coefficient with experimental measurements and checking the sensitivity of the solution to additional iterations.

The `cases/` folder contains the matching Fluent case and data files for the saved states at **1,500** and **3,500 cumulative iterations**. The `results/` folder contains the skin-friction comparison figure and the values interpolated at the 16 experimental stations. The mean absolute percentage discrepancy of the 3,500-state solution relative to the experimental data is **6.33%**. For the setup, numerical methods and detailed discussion, see **Section 4 of the research internship report**.

**Iteration note:** The report describes a comparison between 1,500 and 2,000 iterations, whereas the supplied Fluent files record 1,500 and 3,500 cumulative iterations. This difference in iteration labeling should be clarified before citing the saved states as an exact reproduction of that comparison.
