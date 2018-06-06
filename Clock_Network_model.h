#ifndef __CLOCK_NETWORK_MODEL_H_

#include <stdlib.h>
#include <iostream>
#include <array>
#include <math.h>

using namespace std;

void get_CLK_AreaLatencyPower(int cycle_time,
					float bandwidth_inbit,
					float n_input,
					float n_output,
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
			float broadcast_ratio = n_output/n_input;
			
			/*
			*area = bandwidth_inbit * broadcast_ratio * (0.53948*broadcast_ratio - 0.66);
			*latency = 0.6;
			*dynamic_power = bandwidth_inbit * broadcast_ratio * (0.0032374*broadcast_ratio + 0.0051);
			*leakage_power = bandwidth_inbit * broadcast_ratio * (0.013987*broadcast_ratio + 0.074);
			*/


			*area = bandwidth_inbit * broadcast_ratio * (0.49552*broadcast_ratio - 0.34);
			*latency = 0.6;
			//*dynamic_power = bandwidth_inbit * broadcast_ratio * (0.0013978*broadcast_ratio + 0.0223);
			*dynamic_power = bandwidth_inbit * broadcast_ratio * (0.069582/500*broadcast_ratio + 0.0031) + 22;
			*leakage_power = bandwidth_inbit * broadcast_ratio * (0.0114664*broadcast_ratio - 0.0028);
		}
	}
}


#endif /* __CLOCK_NETWORK_MODEL_H_ */