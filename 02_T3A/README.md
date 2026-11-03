# ERCOFTAC T3A — Flat-Plate Transition and Parametric Studies

This case reconstructs the ERCOFTAC T3A transitional flat-plate configuration in ANSYS Fluent using the Transition SST model. The baseline solution was compared with NASA reference results, an independently computed SU2 solution, and experimental skin-friction measurements.

The comparison at 16 experimental stations showed mean differences of **3.32%** between Fluent and NASA and **0.913%** between Fluent and SU2, while the mean discrepancy between Fluent and experimental measurements was **12.92%**.

Controlled parametric studies investigated the influence of inlet turbulence intensity, inlet eddy viscosity ratio, geometric transition roughness height (K), equivalent sand-grain roughness height (Ks), and wall roughness constant (Cs).

Detailed numerical settings, results, and discussion are presented in **Section 5 of the Research Internship Report**.

## Baseline Validation Results

**Fluent, NASA reference, and experimental measurements**

![T3A Fluent NASA comparison](results/cf_fluent_nasa.png)

**Fluent, SU2, and experimental measurements**

![T3A Fluent SU2 comparison](results/cf_fluent_su2.png)
