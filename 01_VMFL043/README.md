# VMFL043 — Flat-Plate Transition Benchmark

This case reproduces the ANSYS VMFL043 benchmark using the Transition SST model in ANSYS Fluent. The objective was to establish a numerical baseline for predicting laminar-to-turbulent transition over a smooth flat plate by comparing the skin-friction coefficient with the ANSYS reference solution and experimental measurements.

The `cases/` directory contains the Fluent case and data files corresponding to the two solutions compared in the report, after 1,500 iterations and an additional 2,000 iterations (3,500 cumulative iterations). The `results/` directory contains the skin-friction comparison with experimental measurements, showing a mean absolute percentage discrepancy of approximately **6.3%** across 16 experimental stations.

Detailed numerical settings, results, and discussion are presented in **Section 4 of the Research Internship Report**.
