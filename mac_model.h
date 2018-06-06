#ifndef __MAC_MODEL_H_

#include <stdlib.h>
#include <iostream>
#include <array>
#include <math.h>

using namespace std;
/*---------------- Area/Latency/Power Model -----------------------
  ------------------- 1. Adder Tree: n_channels
  ------------------- 2. Multiplier
  */

const array<float, 4> INV = { 0.4536, 0.01, 27.5065e-06, 9.9704e-06 };
const array<float, 4> XOR = { 1.134, 0.01, 68.8978e-06, 38.9723e-06};
const array<float, 4> OR = { 0.5184, 0.01, 21.9e-06, 19.6287e-06};
const array<float, 4> AND = { 0.5184, 0.01, 16.4349e-06, 14.6006e-06};

const array<float, 4> ADDF = {3.6288, 0.04, 272.6458e-06, 116.9058e-06};

void getAdderTree_AreaLatencyPower(
	int cycle_time,
	float num_input,
	float num_bit,
	float* area,
	float* latency,
	float* dynamimc_power,
	float* leakage_power
	){

	if(cycle_time != 5){
		cout<<"Error: we only support 5ns cycle_time by now"<<endl;
	}
	else{	
		float A_Cout = XOR[1] + AND[1] + OR[1];
		float Cin_Cout = AND[1] + OR[1];
		float Cin_S = XOR[1];
		float A_S = XOR[1] + XOR[1];

		*area = (2*XOR[0]+2*AND[0]+OR[0]) * (num_input-1) * num_bit;
		*latency = A_Cout + Cin_S + (num_bit-2)*Cin_Cout + ceil(log(num_input))*A_Cout;
		*dynamimc_power = (2*XOR[2]+2*AND[2]+OR[2]) * (num_input-1) * num_bit;
		*leakage_power = (2*XOR[3]+2*AND[3]+OR[3]) * (num_input-1) * num_bit;
	}
}

/* ------------------- 2. Multiplier {area, latency, dynamic power, leakage power} ----------------------*/
#define Mult_max_bits 16
#define Mult_n_paramters 5
#define Mult_array_size Mult_n_paramters*Mult_max_bits

const array<float, Mult_array_size> multiplier_2stage_model_5ns = {
	5.443200, 0.04, 9.8514e-04, 4.5459e-05, 0.1260, // 1bit
	31.978800, 0.15, 4.4779e-03, 6.3637e-04, 0.8562, // 2bit
	68.493600, 0.27, 7.5472e-03, 2.1254e-03, 1.8695, // 3bit
	112.039200, 0.32, 1.1226e-02, 4.4355e-03, 3.1043, // 4bit
	153.543598, 0.66, 1.5842e-02, 9.1972e-03, 4.0909, // 5bit
	224.078398, 0.62, 2.1394e-02, 1.1753e-02, 6.3751, // 6bit
	258.551995, 0.85, 2.6709e-02, 1.8361e-02, 6.9882, // 7bit
	328.859994, 0.97, 3.3502e-02, 2.4307e-02, 8.9566, // 8bit
	382.384792, 0.99, 4.0283e-02, 3.1363e-02, 10.4902, // 9bit
	466.527591, 1.09, 4.8996e-02, 3.9274e-02, 12.8830, // 10bit
	532.299588, 1.18, 5.7215e-02, 4.7510e-02, 14.6940, // 11bit
	631.864786, 1.28, 6.7882e-02, 5.7767e-02, 17.5312, // 12bit
	713.512783, 1.33, 7.6916e-02, 6.8046e-02, 19.7941, // 13bit
	832.582782, 1.36, 8.7952e-02, 7.8982e-02, 23.1173, // 14bit
	917.632778, 1.44, 9.8557e-02, 9.0767e-02, 25.4648, // 15bit
	1049.630375, 1.45, 0.1001, 9.3309e-02, 28.3159 // 16bit
};

void getMultiplier_AreaLatencyPower(
				int cycle_time,
				float n_bits,
				float* area,
				float* latency,
				float* dynamic_power,
				float* leakage_power
				){
	if(cycle_time!=5){
		cout<<"Error: we only support 5ns cycle time by now";
	}
	else{
		array<float, Mult_n_paramters> multiplier = {0, 0, 0, 0, 0};
		// Area index
		int area_index = int( (n_bits - 1)*Mult_n_paramters + 0 );
		// Latency index
		int latency_index = int( (n_bits - 1)*Mult_n_paramters + 1 );
		// internal power index
		int internal_power_index = int( (n_bits - 1)*Mult_n_paramters + 2 );
		// switching power index
		int switching_power_index = int( (n_bits - 1)*Mult_n_paramters + 3 );
		// leakage power index
		int leakage_power_index = int( (n_bits - 1)*Mult_n_paramters + 4 );
		
		if(cycle_time == 5){
			// Area
			multiplier[0] = multiplier_2stage_model_5ns[area_index];
			// Latency
			multiplier[1] = multiplier_2stage_model_5ns[latency_index];
			// internal power
			multiplier[2] = multiplier_2stage_model_5ns[internal_power_index];
			// switching power
			multiplier[3] = multiplier_2stage_model_5ns[switching_power_index];
			// leakage power
			multiplier[4] = multiplier_2stage_model_5ns[leakage_power_index];
		}
		
		*area = multiplier[0];
		*latency = multiplier[1];
		*dynamic_power = multiplier[2] + multiplier[3];
		*leakage_power = multiplier[4];
	}
}
#endif /* __MAC_MODEL_H_ */
