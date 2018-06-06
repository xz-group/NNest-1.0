#ifndef __NOC_MODEL_H_

#include <stdlib.h>
#include <iostream>
#include <array>
#include <math.h>

using namespace std;

#define NoC_n_paramters 4

const array<float, NoC_n_paramters> INV_5ns = {0.453600, 0.01, 50e-06, 10e-03};
void getNoC_AreaLatencyPower(int cycle_time,
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
			if(n_output == n_input){
				*area = 0;
				*latency = 0;
				*dynamic_power = 0;
				*leakage_power = 0;
			}
			else{
				float num_inv = bandwidth_inbit * (n_input + n_output);
				*area = num_inv * INV_5ns[0];

				*leakage_power = num_inv * INV_5ns[3];

				float broadcast_copy = n_output / n_input;
				if(broadcast_copy >= 24){
					*latency = 0.16;
				}
				else{
					*latency = (5.0099*(broadcast_copy - 1) + 36.403) / 1000;
				}


				float input_inv_delay = n_output/n_input * INV_5ns[1];
				float output_inv_delay = INV_5ns[1];
				*latency = input_inv_delay + output_inv_delay;

				*dynamic_power = (INV_5ns[2]*n_output) * bandwidth_inbit;
			}
			
		}
	}
}


#endif /* __NOC_MODEL_H_ */