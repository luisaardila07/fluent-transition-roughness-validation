
# ERCOFTAC T3A — Flat-Plate Transition and Parametric Study

This case reconstructs the ERCOFTAC T3A flat-plate transition benchmark in ANSYS Fluent using the Transition SST model. The Fluent solution was compared with NASA reference results, an independently computed SU2 solution, and experimental skin-friction measurements.

The comparisons showed mean percentage differences of **3.32% (Fluent–NASA)** and **0.913% (Fluent–SU2)**, and a mean discrepancy of **12.92% (Fluent–experiment)**.

A controlled parametric study investigated the effects of inlet turbulence intensity, inlet eddy viscosity ratio, geometric transition roughness (K), equivalent sand-grain roughness (Ks), and wall roughness constant (Cs). Each parameter was varied independently from the reference configuration.

The SU2 calculation was based on the [official SU2 T3A tutorial](https://su2code.github.io/tutorials/Transitional_Flat_Plate_T3A/) and [SU2 Tutorials repository](https://github.com/su2code/Tutorials).

Detailed methodology and results are presented in **Section 5 of the Research Internship Report**.

## Fluent Simulation Files

- [Case file](cases/final.cas.h5)
- [Data file](cases/final.dat.h5)

## Baseline Validation Results

**Fluent – NASA – Experiment**

![Fluent NASA comparison](results/cf_fluent_nasa.png)

**Fluent – SU2 – Experiment**

![Fluent SU2 comparison](results/cf_fluent_su2.png)


## Parametric Study Results

All parametric simulations were performed using the baseline Fluent configuration. Each study independently modified one input parameter while maintaining the computational mesh, freestream conditions, and remaining model settings unchanged.

The investigated parameters were inlet turbulence intensity (Tu), inlet eddy viscosity ratio (μt/μ), geometric transition roughness height (K), equivalent sand-grain roughness height (Ks), and wall roughness constant (Cs).

The following figures show the resulting skin-friction distributions. The complete parameter ranges and numerical procedures are documented in Section 5 of the Research Internship Report.

### Inlet Turbulence Intensity (Tu)

![Turbulence intensity](results/turbulence_intensity.png)

### Inlet Eddy Viscosity Ratio (μt/μ)

![Eddy viscosity ratio](results/eddy_viscosity_ratio.png)

### Geometric Transition Roughness (K)

![Geometric roughness](results/geometric_roughness_K.png)

### Equivalent Sand-Grain Roughness (Ks)

![Sand-grain roughness](results/sand_grain_roughness_Ks.png)

### Wall Roughness Constant (Cs)

![Roughness constant](results/roughness_constant_Cs.png)
