/*
*	Given a Temperature Trend, outputs the MTTF due to Thermal Cycle
*	
*	The MTTF due to thermal Cycle is caclulated in Three steps [1]:
*	
*	1) Calculation of the thermal Cycles from a thermal profile using Downing simple rainbow counting algorithm[2]
*	2) Calculation, for each thermal cycle, the number of cycles to failure using Coffin-Mansion's rule:
*			Ntc[i] = Atc(dT[i] - Tth)^-b * e^(Eatc/(KTmax[i]))
*	3) Calculate MTTF using Miner's rule:
*			MTTF = (1/m)Ntc*sum,i=1 to m (ti)
*
*
*
*
*
*

*	[1] Reinforcement Learning-Based Inter- and Intra-Application Thermal Optimization for Lifetime Improvement of Multicore Systems
*	[2] S. Downing and D. Socie. Simple Rainflow Counting Algorithms. International Journal of Fatigue, 1982.
*/