# Supplementary Materials for: Anomalous strain-rate enhancement of grain boundary sliding triggered by phonon-drag-induced dislocation deficit

## 1. Settings

The input file (**SimulationBox.dat**) and the applied potential (**Potential.alloy**) are presented in this folder. To help track the essential elements discussed in the manuscript, an illustration of the model (**SimulationBox.dat**) is also attached.

It is noteworthy that under periodic boundary conditions, the input file describes two crystals forming symmetric tilt GBs, with a single edge dislocation introduced at the centre of the box.

The crystal orientations are orchestrated such that, under periodic boundary conditions, the dislocation slips along a closed loop within its original crystal, ensuring that all accommodated strain is parallel to the applied shear stress. Specifically, in Crystal A, $[110]$ aligns with the global X-axis, $[1\bar{1}1]$ aligns with the Y-axis, and $[1\bar{1}\bar{2}]$ aligns with the Z-axis, while in Crystal B, $[110]$ aligns with the X-axis,  $[\bar{1}11]$  aligns with the Y-axis, and $[1\bar{1}2]$ aligns with the Z-axis.

## 2. Simulation

The simulations were performed on LAMMPS. **Code.lammps** shows the key LAMMPS commands reorganized for better readability, where the strain rate and the temperature need specifying. The deformation was subjected to ultrahigh strain rates ranging from $2\times 10^8$ to $1\times 10^9 \text{ s}^{−1}$. To ensure numerical fidelity, the present simulations were performed at $750\text{ K}$. Please check the manuscript for more information.

While the whole ensemble of raw data is too large to upload, we uploaded a representative case (**RepresentativeRawData.lammpstrj**) and a video demonstration (**DeformationDemo.mp4**) to help understand the precedure.

## 3. Stress Extraction

The calculated stress tensors are presented here in a group of files **Stress_Strain_SRxxxx_T750.xls**.

Please note that the shear stress $p_{xy}$ is of our interest.

## 4. ASD Analysis

The accumulated shear displacement (ASD) is defined as the integrated displacement along the shear direction $x$ and expressed as a function of the sample height $y$. This is the major variable throughout our analysis. 

In our deliberately minimal configuration, differentiation of the ASD directly yields the strain components: $ε_{GB}+ε_{DL}+ε_e=ε_total$, where $ε_{GB}$, $ε_{DL}$, $ε_e$ and $ε_{total}$ denote the strain accommodated by the two grain boundaries, the sole dislocation, elastic deformation, and the total strain, respectively.

The ASD function across various strain rates is presented in a group of files **ASD_SRxxxx_T750**. The code that implements the extraction (**Code.cpp**) involves a series of displacement sampling techniques and corner case discussions addressing issues arising from periodic boundary conditions.

## 5. Dislocation Tracking

Recall that the crystal orientations are orchestrated such that, under periodic boundary conditions, the dislocation slips along a closed loop within its original crystal. Such motion is demonstrated and explained in **DislocationInMotion.mp4** and **DislocationInMotionExplained.png**, respectively.

Please note that the inserted full dislocation sometimes splits into two partial dislocations. Since these partial dislocations remain very close to each other throughout the deformation, the average $x$ coordinate of them is extracted and used for velocity calculation (**DislocationVelocity_SRxxxx_T750**).

Please also note that Ovito DXA occasionally misidentifies dislocations, e.g. the same dislocation appears multiple times. Nonetheless, it does not interfere with our data.
