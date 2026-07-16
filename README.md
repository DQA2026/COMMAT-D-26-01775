# Supplementary Materials for *Anomalous strain-rate enhancement of grain boundary sliding triggered by phonon-drag-induced dislocation deficit*

---

# 1. Simulation Setup

The simulation input file (`SimulationBox.dat`) and the employed interatomic potential (`Potential.alloy`) are provided in this repository. An illustration of the simulation model based on `SimulationBox.dat` is also included for reference.

Under periodic boundary conditions, the simulation cell consists of two crystals forming symmetric tilt grain boundaries (GBs), with a single edge dislocation introduced at the center of the simulation box.

The crystal orientations were specifically designed such that the dislocation moves along a closed loop within its original crystal under periodic boundary conditions. This construction ensures that all accommodated plastic strain remains parallel to the applied shear stress.

The crystallographic orientations are defined as follows:

## Crystal A

- $[110]$ parallel to the global $x$-axis
- $[1\bar{1}1]$ parallel to the global $y$-axis
- $[1\bar{1}\bar{2}]$ parallel to the global $z$-axis

## Crystal B

- $[110]$ parallel to the global $x$-axis
- $[\bar{1}11]$ parallel to the global $y$-axis
- $[1\bar{1}2]$ parallel to the global $z$-axis

---

# 2. Molecular Dynamics Simulations

All simulations were performed using LAMMPS.

The file `Code.lammps` summarizes the key simulation commands reorganized for clarity and readability. The strain rate and temperature should be specified by the user before execution.

The simulations were conducted under ultrahigh strain rates ranging from $2 \times 10^8 \text{ s}^{-1}$ to $1\times 10^9 \text{ s}^{-1}$.

To maintain numerical stability and fidelity, all simulations presented in this work were performed at $750\text{ K}$.

Please refer to the manuscript for additional methodological details.

Due to the large size of the complete raw dataset, only representative examples are included in this repository `RepresentativeRawData.lammpstrj` and `DeformationDemo.mp4`.

These files are intended to demonstrate the deformation process and data structure.

---

# 3. Stress Extraction

The calculated stress tensors are provided in the files `Stress_Strain_SRxxxx_T750.xls`.

The primary quantity analyzed in this work is the shear stress component $p_{xy}$.

---

# 4. ASD Analysis

The accumulated shear displacement (ASD) is defined as the integrated displacement along the shear direction ($x$) expressed as a function of the sample height ($y$).

ASD serves as the primary variable throughout the present analysis.

Under the deliberately simplified simulation configuration employed here, differentiation of the ASD directly yields the strain partitioning relation:

```math
\varepsilon_{GB} + \varepsilon_{DL} + \varepsilon_e = \varepsilon_{total}
```

where:

- $ε_{GB}$ denotes strain accommodated by the two grain boundaries
- $ε_{DL}$ denotes strain accommodated by the dislocation
- $ε_e$ denotes elastic strain
- $ε_{total}$ denotes the total applied strain

The ASD data corresponding to different strain rates are provided in `ASD_SRxxxx_T750`.

The extraction code (`Code.cpp`) implements the ASD analysis together with displacement sampling procedures and treatments for corner cases associated with periodic boundary conditions.

---

# 5. Dislocation Tracking

As discussed above, the crystallographic orientations were intentionally designed such that the dislocation propagates along a closed loop within its original crystal under periodic boundary conditions.

This motion is illustrated in: `DislocationInMotion.mp4` and `DislocationInMotionExplained.png`.

During deformation, the inserted full dislocation occasionally dissociates into two partial dislocations. Since the two partials remain spatially close throughout the simulation, their average $x$ coordinate is used to evaluate the dislocation velocity.

The extracted velocity data are provided in `DislocationVelocity_SRxxxx_T750`.

Dislocation identification was performed using the OVITO DXA algorithm. Occasional duplicate identification of the same dislocation may occur under certain configurations; however, this does not affect the extracted velocity trends or the conclusions of the present work.

---

# 6. Plateau Interpretation

The datasets supporting the interpretation of the dislocation velocity plateau are organized into three categories:

- `A-Geometry&BulkControl`
- `B1-TemperatureDependence`
- `B2-PhononDragCriterion`

Together, these datasets correspond to Fig. 5(a–h) of the manuscript.

## A. Geometry & Bulk Control

This directory contains the control simulations used to examine whether the observed velocity plateau depends on the simulation geometry.

The datasets are organized as follows:

- `Fig5a-GB–DislocationDistance`
- `Fig5b-AspectRatio`
- `Fig5c-BoxSize`
- `Fig5d-BulkControl`

Each directory contains the source data used to construct the corresponding panel in Fig. 5.

For every simulation, the evolution of the shear stress and dislocation velocity, together with the resulting velocity–stress relation, is provided.

These datasets were used to evaluate the effects of:

- dislocation–GB distance,
- simulation cell aspect ratio,
- simulation cell size,
- and the presence or absence of grain boundaries.

## B1. Temperature Dependence

The directory

`Fig5e-TemperatureDependence`

contains the datasets corresponding to Fig. 5(e).

The simulations were performed under identical loading conditions at different temperatures.

For each temperature, the complete evolution of the shear stress and dislocation velocity, together with the resulting velocity–stress relation, is provided.

These datasets support the temperature dependence discussed in the manuscript.

## B2. Phonon-Drag Criterion

The directory

`Fig5f-h-PhononDragCriterion`

contains the datasets supporting Fig. 5(f–h).

The representative simulations shown in Fig. 5(f) and Fig. 5(g) are included together with the complete collection of simulations performed under different combinations of dislocation density and strain rate.

For every simulation, the following information is provided:

- strain rate,
- dislocation density,
- shear stress,
- dislocation velocity,
- identified deformation regime.

The deformation regime is classified as either

- **Classical Forest-Hardening**, or
- **Phonon-Drag-Controlled**,

according to the criterion proposed by Fan *et al.* [22].

These datasets allow direct inspection of the complete parameter space beyond the representative cases presented in the manuscript.

---
