# Assessment of Transition and Surface Roughness Modelling in ANSYS Fluent

This repository contains the numerical simulation cases and selected results developed during a research internship conducted from July to September 2026 at École de technologie supérieure (ÉTS), Montréal, Canada, under the supervision of Professor François Morency.

## Research Objective

The broader research objective is to predict the aerodynamic performance degradation of rotating blades under icing conditions, particularly changes in thrust and torque relative to clean configurations.

As a preliminary step toward this objective, the investigation focused on assessing laminar-to-turbulent transition and surface roughness modelling in ANSYS Fluent. Particular attention was given to the influence of localized leading-edge roughness on boundary-layer development and its downstream aerodynamic effects.


## Numerical Investigations

The investigation was organized into three stages:

1. **[VMFL043 Flat-Plate Transition Benchmark](01_VMFL043/README.md)**  
   Validation of the Transition SST model against an ANSYS reference benchmark and experimental skin-friction measurements.

2. **[ERCOFTAC T3A Transition and Parametric Study](02_T3A/README.md)**  
   Comparison with NASA, SU2, and experimental data, followed by sensitivity analyses of turbulence and roughness parameters.

3. **[NACA 0012 Transition and Surface Roughness Assessment](03_NACA0012/README.md)**  
   Smooth and rough-wall simulations, turbulence sustaining, global geometric-roughness diagnostics, exploratory roughness transport, and standard SST comparisons.

Each directory contains ANSYS Fluent simulation files and selected results. The NACA 0012 directory also includes the available UDF source codes.

Detailed numerical methodology, results, and limitations are documented in the separately provided Research Internship Report. Ice accretion and rotor-performance predictions remain objectives of subsequent research.
