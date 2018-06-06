#ifndef __REG_ARRAY_MODEL_H_

#include <stdlib.h>
#include <iostream>
#include <array>
#include <math.h>

using namespace std;
/*---------------- Area/Latency/Power Model -----------------------
  ------------------- 1. Shift Reg
  */
/* ----------------- 7. Shift Reg {area, latency, power} ------------------- */
#define shiftreg_n_paramters 5
const array<float, shiftreg_n_paramters> DFFQ_5ns = { 3.855600, 0.05, 9.6149e-04, 1.9029e-05, 0.1023};
void getShiftReg_AreaLatencyPower(int cycle_time,
						float n_bits,
					   float* area,
					   float* latency,
                       float* dynamic_power,
					   float* leakage_power
                       ){
	if(cycle_time!=5){
		cout<<"Error: we only support 5ns cycle time by now";
	}
	else {
		if(cycle_time == 5){
			*area = DFFQ_5ns[0] * n_bits;
			*latency = DFFQ_5ns[1];
			*dynamic_power = (DFFQ_5ns[2] + DFFQ_5ns[3]) * n_bits;
			*leakage_power = DFFQ_5ns[4] * n_bits;
		}	
	}
}

void getReg_AreaLatencyPower(int cycle_time,
							float n_bits,
							float* area,
							float* latency,
							float* dynamic_power,
							float* leakage_power
							){
	if(cycle_time!=5){
		cout<<"Error: we only support 5ns cycle time by now";
	}
	else {
		if(cycle_time == 5){
			*area = DFFQ_5ns[0] * n_bits;
			*latency = DFFQ_5ns[1];
			*dynamic_power = (DFFQ_5ns[2] + DFFQ_5ns[3]) * n_bits;
			*leakage_power = DFFQ_5ns[4] * n_bits;
		}	
	}
}
#endif /* __REG_ARRAY_MODEL_H_ */
