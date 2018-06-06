#include <fstream>
#include "mac_model.h"
#include "reg_array_model.h"
#include "SRAM_model.h"
#include "NoC_model.h"
#include "Clock_Network_model.h"

int main(){

	ofstream dram_data_file("./MH_IWR_result/MH_IWR_dram.txt");
	ofstream energy_data_file("./MH_IWR_result/MH_IWR_energy.txt");

	ofstream out_data_file("./MH_IWR_result/MH_IWR_design_space.txt");

	ofstream GLB_data_file("./MH_IWR_result/MH_IWR_design_space_GLB.txt");
	ofstream LB_data_file("./MH_IWR_result/MH_IWR_design_space_LB.txt");
	ofstream NoC_data_file("./MH_IWR_result/MH_IWR_design_space_NoC.txt");
	ofstream PE_data_file("./MH_IWR_result/MH_IWR_design_space_PE.txt");

	ofstream PE_sum_data_file("./MH_IWR_result/MH_IWR_design_space_PE_sum.txt");

	ofstream PE_area_division_data_file("./MH_IWR_result/MH_IWR_design_space_PE_area_division.txt");
	ofstream PE_power_division_data_file("./MH_IWR_result/MH_IWR_design_space_PE_area_division.txt");
	ofstream PE_latency_division_data_file("./MH_IWR_result/MH_IWR_design_space_PE_area_division.txt");

	ofstream CLK_data_file("./MH_IWR_result/MH_IWR_design_space_CLK.txt");

	ofstream area_division_data_file("./MH_IWR_result/MH_IWR_area_division.txt");
	ofstream latency_division_data_file("./MH_IWR_result/MH_IWR_latency_division.txt");
	ofstream dynamic_power_division_data_file("./MH_IWR_result/MH_IWR_dynamic_power_division.txt");
	ofstream leakage_power_division_data_file("./MH_IWR_result/MH_IWR_leakage_power_division.txt");

	// Alexnet Conv3 parameter
	float N = 1, M = 384;
	float R = 3, S = 3, C = 256;
	float E = 13, F = 13;
	float U = 1;

	// Alexnet Conv2 parameter
	M = 256; C = 48; R = 5; S = 5;
	E = 27; F = 27;

	// Alexnet Conv3 parameter
	M = 384; C = 256; R = 3; S = 3;
	E = 13; F = 13;

	// Alexnet Conv3 parameter
	M = 96; C = 48; R = 3; S = 3;
	E = 55; F = 55;


	float H = U * E;
	float W = U * F;

	float input_bitwidth = 16;
	float weight_bitwidth = 16;
	float output_bitwidth = 16;

	float cycle_time = 5;

	// 1GB DRAM
	float dram_block_size = 8; //byte
	float dram_read_energy = 1.48; //4.88984; //nJ
	float dram_write_energy = 1.48; //4.91799; //nJ

	// microarchitecture paramters
	float Tm = 32, Te = 13;

	float ttm, tte, ttc;
	float ttm_max = 4;
	float tte_max = 4;
	float ttc_max = 16;

	ttm = 8;
	tte = 2;
	ttc = 4;

	//for(ttm = 1; ttm <= ttm_max; ttm++){
		//for(tte = 1; tte <= tte_max; tte++){
			//for(ttc = 2; ttc <= ttc_max; ttc+=2){

				// GB IS
				float dram_input_per_access_energy = (input_bitwidth/8)/dram_block_size * dram_read_energy;
				float dram_input_access_times = C*H*W;

				float dram_weight_per_access_energy = (weight_bitwidth/8)/dram_block_size * dram_read_energy;
				float dram_weight_access_times = M*C*R*S;

				float dram_input_read_energy = dram_input_per_access_energy * dram_input_access_times;
				float dram_weight_read_energy = dram_weight_per_access_energy * dram_weight_access_times;


				float dram_output_per_access_energy = (output_bitwidth/8)/dram_block_size * dram_write_energy;
				float dram_output_access_times = M*E*F;

				float dram_output_write_energy = dram_output_per_access_energy * dram_output_access_times;


				dram_data_file<<ttm<<", "<<tte<<", "<<ttc<<", ";
				dram_data_file<<dram_input_read_energy<<", "<<dram_weight_read_energy<<", "<<dram_output_write_energy<<", ";

				float dram_energy = dram_input_read_energy + dram_weight_read_energy + dram_output_write_energy;
				dram_data_file<<dram_energy<<endl;















				/* ---------------- Configuration ---------------------*/
				/* ------------------ 1. GLB --------------------------*/
				float input_num_access_per_cycle, weight_num_access_per_cycle, psum_num_access_per_cycle;

				input_num_access_per_cycle = 1; 
				weight_num_access_per_cycle = 1/F; 
				psum_num_access_per_cycle = 1;

				// C * (R-U+U*TTe) * (U*Tf)*(S-U+U*F/U*Tf)
				int num_input_GLB_SRAM = R-U+U*Te;
				int input_block_size_inByte = U*ttc * (input_bitwidth/8); 
				int input_num_blocks = int( F * ceil(C/ttc) );
				float input_GB_read_activity_factor = float(R-U+U*tte) / float(R-U+U*Te);
				float input_GB_write_activity_factor = float(tte) / float(R-U+U*Te);

				// R*S * TTm * C
				int num_weight_GLB_SRAM = R*S;
				int weight_block_size_inByte = ttm * ttc * (weight_bitwidth/8);
				int weight_num_blocks = int( ceil(Tm/ttm) * ceil(C/ttc) ); 

				// TTm*TTe*F
				int num_psum_GLB_SRAM = 1;
				int psum_block_size_inByte = ttm*tte * (output_bitwidth/8);
				int psum_num_blocks = int( ceil(Tm/ttm) * ceil(Te/tte) * F);





				/* ------------------ 2. LB --------------------------*/
				float input_LB_num_bits, weight_LB_num_bits, psum_LB_num_bits;
				
				input_LB_num_bits =  tte*ttc*R*S *input_bitwidth;
				weight_LB_num_bits = ttm*ttc*R*S *weight_bitwidth;
				psum_LB_num_bits = ttm*tte*1;




				/* ----------------- 3. PE ---------------------------------*/
				float num_PE = ttm * tte;
				/* -------------------- 1.1 Multiplier -----------------------*/
				float n_multiplier =  ttc*R*S;
				float mult_bitwidth = input_bitwidth;
				/* -------------------- 1.2 Adder Tree -----------------------*/
				float num_input = ttc*R*S + 1;
				float num_bit = input_bitwidth;



				/* ------------------ 4. NoC --------------------------*/
				/* --------------------- 2.1 Ifmap NoC --------------------------*/
				// input GLB out = Tn*Tc * (R-U+U*Te)*(S-U+U*Tf)  --> PE = Tn*Tc *Tm*Te*Tf*R*S
				float ifmap_bandwidth_inbit = input_bitwidth;
				float ifmap_num_input = R-U+U*tte;
				float ifmap_num_output = tte*R;
				float ifmap_num_NoC = ttm*ttc*S;


				/* --------------------- 2.2 weight NoC --------------------------*/
				// weight GLB out = (Tm * Tc*R*S) * 1  --> PE = (Tm * Tc*R*S) * Tn*Te*Tf
				float weight_bandwidth_inbit = weight_bitwidth;
				float weight_num_input = 1;
				float weight_num_output = tte;
				float weight_num_NoC = ttm*ttc*R*S;



				/* ------------------ 3. CLK Network --------------------------*/
				float clk_bitwidth = 1;
				float clk_fan_in = 1;
				float clk_fan_out = 3;

























				/* ------------------ 1. GLB --------------------------*/

				/* --------------------- 2.1 Input SRAM ------------------*/
				// # of SRAMs = Tn*Tc, 1-SRAM size = Tc*(R-U+U*E)*(S-U+U*F), block size = (R-U+U*Te)*(S-U+U*Tf), # of blocks = C/Tc *E/Te * F/Tf


				// minimum line size in cacti is 32-bit/4-byte
				if (input_block_size_inByte < 4)         // minimum line size in cacti is 32-bit/4-byte
					input_block_size_inByte = 4;
				 // minimum number of blocks in cacti is 64
				if (input_num_blocks < 64)         // minimum number of blocks in cacti is 64
					input_num_blocks = 64;

				int input_num_ports = 1;

				int input_num_of_bytes = input_block_size_inByte * input_num_blocks; 

				uca_org_t input_GLB_cacti = cactiWrapper(input_num_of_bytes, input_block_size_inByte, input_num_ports);

				// Cacti area unit in um2
				float input_GLB_area = num_input_GLB_SRAM * float(input_GLB_cacti.area);
				// Cacti access time unit in S, so access_time*1e+9 -> ns
				float input_GLB_access_time = float(input_GLB_cacti.access_time * 1e+9);

				// Cacti energy unit mJ
				float input_GLB_read_energy = input_num_access_per_cycle * input_GB_read_activity_factor * num_input_GLB_SRAM * float(input_GLB_cacti.power.readOp.dynamic) * 1e+3;
				float input_GLB_write_energy = input_num_access_per_cycle * input_GB_write_activity_factor * num_input_GLB_SRAM * float(input_GLB_cacti.power.writeOp.dynamic) * 1000;
				
				float input_GLB_dynamic_power = (input_GLB_read_energy + input_GLB_write_energy) / (cycle_time*1e-09);
				// Cacti leakage power unit uW
				float input_GLB_leakage_power = num_input_GLB_SRAM * float(input_GLB_cacti.power.readOp.leakage) * 1e+6;

				cout<<"num_input_GLB_SRAM = "<<num_input_GLB_SRAM<<endl;
				cout<<"input_block_size_inByte = "<<input_block_size_inByte<<endl;
				cout<<"input_num_blocks = "<<input_num_blocks<<endl;
				cout<<"input_num_of_bytes = "<<input_num_of_bytes<<endl;
				cout<<"input_GLB_area = "<<input_GLB_area<<endl;
				cout<<"input_GLB_access_time = "<<input_GLB_access_time<<endl;
				cout<<"input_GLB_read_energy = "<<input_GLB_read_energy<<endl;
				cout<<"input_GLB_write_energy = "<<input_GLB_write_energy<<endl;
				cout<<"input_GLB_dynamic_power = "<<input_GLB_dynamic_power<<endl;
				cout<<"input_GLB_leakage_power = "<<input_GLB_leakage_power<<endl; 

				GLB_data_file<<input_GLB_area<<", "<<input_GLB_access_time<<", "<<input_GLB_dynamic_power<<", "<<input_GLB_leakage_power<<", ";


			
				/* ---------------------2.2 Weight GLB ------------------*/
				// # of SRAMs = Tm, 1-SRAM size = C*R*S, block size = TC*R*S, # of blocks = C/Tc


				 // minimum line size in cacti is 32-bit/4-byte
				if (weight_block_size_inByte < 4)         // minimum line size in cacti is 32-bit/4-byte
					weight_block_size_inByte = 4;
				// minimum number of blocks in cacti is 64
				if (weight_num_blocks < 64)         // minimum number of blocks in cacti is 64
					weight_num_blocks = 64;

				int weight_num_ports = 1;
				
				int weight_num_of_bytes = weight_block_size_inByte * weight_num_blocks; 

				uca_org_t weight_GLB_cacti = cactiWrapper(weight_num_of_bytes, weight_block_size_inByte, weight_num_ports);

				// Cacti area unit in um2
				float weight_GLB_area = num_weight_GLB_SRAM * float(weight_GLB_cacti.area);
				// Cacti access time unit in S, so access_time*1e+9 -> ns
				float weight_GLB_access_time = float(weight_GLB_cacti.access_time * 1e+9);

				// Cacti energy unit mJ
				float weight_GLB_read_energy = weight_num_access_per_cycle * num_weight_GLB_SRAM * float(weight_GLB_cacti.power.readOp.dynamic) * 1e+3;
				float weight_GLB_write_energy = weight_num_access_per_cycle * num_weight_GLB_SRAM * float(weight_GLB_cacti.power.writeOp.dynamic) * 1000;
				
				float weight_GLB_dynamic_power = (weight_GLB_read_energy + weight_GLB_write_energy) / (cycle_time*1e-09);
				// Cacti leakage power unit uW
				float weight_GLB_leakage_power = num_weight_GLB_SRAM * float(weight_GLB_cacti.power.readOp.leakage) * 1e+6;

				//weight_GLB_area = 0;
				//weight_GLB_access_time = 0;
				//weight_GLB_dynamic_power = 0;
				//weight_GLB_leakage_power = 0;

				cout<<"num_weight_GLB_SRAM = "<<num_weight_GLB_SRAM<<endl;
				cout<<"weight_num_blocks = "<<weight_num_blocks<<endl;
				cout<<"weight_block_size_inByte = "<<weight_block_size_inByte<<endl;
				cout<<"weight_num_of_bytes = "<<weight_num_of_bytes<<endl;
				cout<<"weight_GLB_area = "<<weight_GLB_area<<endl;
				cout<<"weight_GLB_access_time = "<<weight_GLB_access_time<<endl;
				cout<<"weight_GLB_read_energy = "<<weight_GLB_read_energy<<endl;
				cout<<"weight_GLB_write_energy = "<<weight_GLB_write_energy<<endl;
				cout<<"weight_GLB_dynamic_power = "<<weight_GLB_dynamic_power<<endl;
				cout<<"weight_GLB_leakage_power = "<<weight_GLB_leakage_power<<endl; 

				/*
				float weight_GLB_num_bits = Tm*Tc*R*S*weight_bitwidth;

				getReg_AreaLatencyPower(cycle_time,
										weight_GLB_num_bits,
										&weight_GLB_area,
										&weight_GLB_access_time,
										&weight_GLB_dynamic_power,
										&weight_GLB_leakage_power
										);
										*/

				GLB_data_file<<weight_GLB_area<<", "<<weight_GLB_access_time<<", "<<weight_GLB_dynamic_power<<", "<<weight_GLB_leakage_power<<", ";


				/* ---------------------2.3 Psum GLB ------------------*/
				// # of SRAMs = Tn*Tm, total size = E*F, block size = Te*Tf, # of blocks = E/Te * F/Tf
				 // minimum line size in cacti is 32-bit/4-byte
				if (psum_block_size_inByte < 4)         // minimum line size in cacti is 32-bit/4-byte
					psum_block_size_inByte = 4;
				 // minimum number of blocks in cacti is 64
				if (psum_num_blocks < 64)         // minimum number of blocks in cacti is 64
					psum_num_blocks = 64;

				int psum_num_ports = 1;
				
				int psum_num_of_bytes = psum_block_size_inByte * psum_num_blocks; 

				uca_org_t psum_GLB_cacti = cactiWrapper(psum_num_of_bytes, psum_block_size_inByte, psum_num_ports);

				// Cacti area unit in um2
				float psum_GLB_area = num_psum_GLB_SRAM * float(psum_GLB_cacti.area);
				// Cacti access time unit in S, so access_time*1e+9 -> ns
				float psum_GLB_access_time = float(psum_GLB_cacti.access_time * 1e+9);

				// Cacti energy unit mJ
				float psum_GLB_read_energy = psum_num_access_per_cycle * num_psum_GLB_SRAM * float(psum_GLB_cacti.power.readOp.dynamic) * 1e+3;
				float psum_GLB_write_energy = psum_num_access_per_cycle * num_psum_GLB_SRAM * float(psum_GLB_cacti.power.writeOp.dynamic) * 1000;
				
				float psum_GLB_dynamic_power = (psum_GLB_read_energy + psum_GLB_write_energy) / (cycle_time*1e-09);

				// Cacti power unit uW
				float psum_GLB_leakage_power = num_psum_GLB_SRAM * float(psum_GLB_cacti.power.readOp.leakage) * 1e+6;
				cout<<"psum num_blocks = "<<psum_num_blocks<<endl;
				cout<<"psum block_size_inByte = "<<psum_block_size_inByte<<endl;
				cout<<"psum num_of_bytes = "<<psum_num_of_bytes<<endl;
				cout<<"psum_GLB_area = "<<psum_GLB_area<<endl;
				cout<<"psum_GLB_access_time = "<<psum_GLB_access_time<<endl;
				cout<<"psum_GLB_read_energy = "<<psum_GLB_read_energy<<endl;
				cout<<"psum_GLB_write_energy = "<<psum_GLB_write_energy<<endl;
				cout<<"psum_GLB_dynamic_power = "<<psum_GLB_dynamic_power<<endl;
				cout<<"psum_GLB_leakage_power = "<<psum_GLB_leakage_power<<endl;

				GLB_data_file<<psum_GLB_area<<", "<<psum_GLB_access_time<<", "<<psum_GLB_dynamic_power<<", "<<psum_GLB_leakage_power<<", ";

				float GLB_area = input_GLB_area + weight_GLB_area + psum_GLB_area;
				float GLB_latency = max( max(input_GLB_access_time, weight_GLB_access_time), psum_GLB_access_time );
				float GLB_dynamic_power = input_GLB_dynamic_power + weight_GLB_dynamic_power + psum_GLB_dynamic_power;
				float GLB_leakage_power = input_GLB_leakage_power + weight_GLB_leakage_power + psum_GLB_leakage_power;

				cout<<"GLB_area = "<<GLB_area<<endl;
				cout<<"GLB_latency = "<<GLB_latency<<endl;
				cout<<"GLB_dynamic_power = "<<GLB_dynamic_power<<endl;
				cout<<"GLB_leakage_power = "<<GLB_leakage_power<<endl;
				
				GLB_data_file<<GLB_area<<", "<<GLB_latency<<", "<<GLB_dynamic_power<<", "<<GLB_leakage_power<<endl;



				latency_division_data_file<<input_GLB_access_time<<", "<<weight_GLB_access_time<<", "<<psum_GLB_access_time<<", ";




















				/* ------------------ 4. LB --------------------------*/
				float input_LB_area;
				float input_LB_latency;
				float input_LB_dynamic_power;
				float input_LB_leakage_power;
				getShiftReg_AreaLatencyPower(cycle_time,
										input_LB_num_bits,
										&input_LB_area,
										&input_LB_latency,
										&input_LB_dynamic_power,
										&input_LB_leakage_power
										);
				LB_data_file<<input_LB_area<<", "<<input_LB_latency<<", "<<input_LB_dynamic_power<<", "<<input_LB_leakage_power<<", ";

				float weight_LB_area;
				float weight_LB_latency;
				float weight_LB_dynamic_power;
				float weight_LB_leakage_power;
				getReg_AreaLatencyPower(cycle_time,
										weight_LB_num_bits,
										&weight_LB_area,
										&weight_LB_latency,
										&weight_LB_dynamic_power,
										&weight_LB_leakage_power
										);
				
				LB_data_file<<weight_LB_area<<", "<<weight_LB_latency<<", "<<weight_LB_dynamic_power<<", "<<weight_LB_leakage_power<<", ";


				float psum_LB_area;
				float psum_LB_latency;
				float psum_LB_dynamic_power;
				float psum_LB_leakage_power;
				getReg_AreaLatencyPower(cycle_time,
										psum_LB_num_bits,
										&psum_LB_area,
										&psum_LB_latency,
										&psum_LB_dynamic_power,
										&psum_LB_leakage_power
										);
				
				LB_data_file<<psum_LB_area<<", "<<psum_LB_latency<<", "<<psum_LB_dynamic_power<<", "<<psum_LB_leakage_power<<", ";


				float LB_area = input_LB_area + weight_LB_area + psum_LB_area;
				float LB_latency = max(max(input_LB_latency, weight_LB_latency), psum_LB_latency);
				float LB_dynamic_power = input_LB_dynamic_power + weight_LB_dynamic_power + psum_LB_dynamic_power;
				float LB_leakage_power = input_LB_leakage_power + weight_LB_leakage_power + psum_LB_leakage_power;

				cout<<"LB_area = "<<LB_area<<endl;
				cout<<"LB_latency = "<<LB_latency<<endl;
				cout<<"LB_dynamic_power = "<<LB_dynamic_power<<endl;
				cout<<"LB_leakage_power = "<<LB_leakage_power<<endl;

				LB_data_file<<LB_area<<", "<<LB_latency<<", "<<LB_dynamic_power<<", "<<LB_leakage_power<<endl;

				latency_division_data_file<<input_LB_latency<<", "<<weight_LB_latency<<", "<<psum_LB_latency<<", ";





















				/* ----------------- 1. PE ---------------------------------*/
				/* -------------------- 1.1 Multiplier -----------------------*/
			
				float multiplier_area;
				float multiplier_latency;
				float multiplier_dynamic_power;
				float multiplier_leakage_power;

				getMultiplier_AreaLatencyPower(
					cycle_time,
					mult_bitwidth,
					&multiplier_area,
					&multiplier_latency,
					&multiplier_dynamic_power,
					&multiplier_leakage_power
				);
				
				multiplier_area = multiplier_area * n_multiplier;
				multiplier_latency = multiplier_latency;
				multiplier_dynamic_power = multiplier_dynamic_power * n_multiplier;
				multiplier_leakage_power = multiplier_leakage_power * n_multiplier;

				cout<<"# of multipliers in each PE = "<<n_multiplier<<endl;
				cout<<"multiplier_area = "<<multiplier_area<<endl;
				cout<<"multiplier_latency = "<<multiplier_latency<<endl;
				cout<<"multiplier_dynamic_power = "<<multiplier_dynamic_power<<endl;
				cout<<"multiplier_leakage_power = "<<multiplier_leakage_power<<endl;

				PE_data_file<<multiplier_area<<", "<<multiplier_latency<<", "<<multiplier_dynamic_power<<", "<<multiplier_leakage_power<<", ";

				/* -------------------- 1.2 Adder Tree -----------------------*/

				float adder_tree_area;
				float adder_tree_latency;
				float adder_tree_dynamic_power;
				float adder_tree_leakage_power;

				getAdderTree_AreaLatencyPower(
					cycle_time,
					num_input,
					num_bit,
					&adder_tree_area,
					&adder_tree_latency,
					&adder_tree_dynamic_power,
					&adder_tree_leakage_power
				);

				cout<<"# of adder tree's input = "<<num_input<<endl;
				cout<<"adder_tree_area = "<<adder_tree_area<<endl;
				cout<<"adder_tree_latency = "<<adder_tree_latency<<endl;
				cout<<"adder_tree_dynamic_power = "<<adder_tree_dynamic_power<<endl;
				cout<<"adder_tree_leakage_power = "<<adder_tree_leakage_power<<endl;

				PE_data_file<<adder_tree_area<<", "<<adder_tree_latency<<", "<<adder_tree_dynamic_power<<", "<<adder_tree_leakage_power<<", ";



				float single_PE_area = multiplier_area + adder_tree_area;
				float single_PE_latency = multiplier_latency + adder_tree_latency;
				float single_PE_dynamic_power = multiplier_dynamic_power + adder_tree_dynamic_power;
				float single_PE_leakage_power = multiplier_leakage_power + adder_tree_leakage_power;
				
				cout<<"single_PE_area = "<<single_PE_area<<endl;
				cout<<"single_PE_latency = "<<single_PE_latency<<endl;
				cout<<"single_PE_dynamic_power = "<<single_PE_dynamic_power<<endl;
				cout<<"single_PE_leakage_power = "<<single_PE_leakage_power<<endl;

				PE_data_file<<single_PE_area<<", "<<single_PE_latency<<", "<<single_PE_dynamic_power<<", "<<single_PE_leakage_power<<endl;

				float multiplier_area_div = multiplier_area / single_PE_area;
				float multiplier_dynamic_power_div = multiplier_dynamic_power / single_PE_dynamic_power;
				float multiplier_latency_div = multiplier_latency / single_PE_latency;

				float adder_tree_area_div = adder_tree_area / single_PE_area;
				float adder_tree_dynamic_power_div = adder_tree_dynamic_power / single_PE_dynamic_power;
				float adder_tree_latency_div = adder_tree_latency / single_PE_latency;

				PE_area_division_data_file<<multiplier_area_div<<", "<<adder_tree_area_div<<endl;
				PE_power_division_data_file<<multiplier_dynamic_power_div<<", "<<adder_tree_dynamic_power_div<<endl;
				PE_latency_division_data_file<<multiplier_latency_div<<", "<<adder_tree_latency_div<<endl;


				float PE_area = num_PE * single_PE_area;
				float PE_latency = single_PE_latency;
				float PE_dynamic_power = num_PE * single_PE_dynamic_power;
				float PE_leakage_power = num_PE * single_PE_leakage_power;

				cout<<"PE_area = "<<PE_area<<endl;
				cout<<"PE_latency = "<<PE_latency<<endl;
				cout<<"PE_dynamic_power = "<<PE_dynamic_power<<endl;
				cout<<"PE_leakage_power = "<<PE_leakage_power<<endl;

				PE_sum_data_file<<PE_area<<", "<<PE_latency<<", "<<PE_dynamic_power<<", "<<PE_leakage_power<<endl;

				latency_division_data_file<<multiplier_latency<<", "<<adder_tree_latency<<", ";




























				/* ------------------ 2. NoC --------------------------*/
				/* --------------------- 2.1 Ifmap NoC --------------------------*/
				// input GLB out = Tn*Tc * (R-U+U*Te)*(S-U+U*Tf)  --> PE = Tn*Tc *Tm*Te*Tf*R*S

				float ifmap_NoC_area;
				float ifmap_NoC_latency;
				float ifmap_NoC_dynamic_power;
				float ifmap_NoC_leakage_power;

				getNoC_AreaLatencyPower(
					cycle_time,
					ifmap_bandwidth_inbit,
					ifmap_num_input,
					ifmap_num_output,
					&ifmap_NoC_area,
					&ifmap_NoC_latency,
					&ifmap_NoC_dynamic_power,
					&ifmap_NoC_leakage_power
			        );
				ifmap_NoC_area = ifmap_NoC_area * ifmap_num_NoC;
				ifmap_NoC_latency = ifmap_NoC_latency;
				ifmap_NoC_dynamic_power = ifmap_NoC_dynamic_power * ifmap_num_NoC;
				ifmap_NoC_leakage_power = ifmap_NoC_leakage_power * ifmap_num_NoC;

				cout<<"ifmap_NoC_area = "<<ifmap_NoC_area<<endl;
				cout<<"ifmap_NoC_latency = "<<ifmap_NoC_latency<<endl;
				cout<<"ifmap_NoC_dynamic_power = "<<ifmap_NoC_dynamic_power<<endl;
				cout<<"ifmap_NoC_leakage_power = "<<ifmap_NoC_leakage_power<<endl;

				NoC_data_file<<ifmap_NoC_area<<", "<<ifmap_NoC_latency<<", "<<ifmap_NoC_dynamic_power<<", "<<ifmap_NoC_leakage_power<<", ";

				/* --------------------- 2.2 weight NoC --------------------------*/
				// weight GLB out = (Tm * Tc*R*S) * 1  --> PE = (Tm * Tc*R*S) * Tn*Te*Tf

				float weight_NoC_area;
				float weight_NoC_latency;
				float weight_NoC_dynamic_power;
				float weight_NoC_leakage_power;

				getNoC_AreaLatencyPower(
					cycle_time,
					weight_bandwidth_inbit,
					weight_num_input,
					weight_num_output,
					&weight_NoC_area,
					&weight_NoC_latency,
					&weight_NoC_dynamic_power,
					&weight_NoC_leakage_power
			        );
				weight_NoC_area = weight_NoC_area * weight_num_NoC;
				weight_NoC_latency = weight_NoC_latency;
				weight_NoC_dynamic_power = weight_NoC_dynamic_power * weight_num_NoC;
				weight_NoC_leakage_power = weight_NoC_leakage_power * weight_num_NoC;

				cout<<"weight_NoC_area = "<<weight_NoC_area<<endl;
				cout<<"weight_NoC_latency = "<<weight_NoC_latency<<endl;
				cout<<"weight_NoC_dynamic_power = "<<weight_NoC_dynamic_power<<endl;
				cout<<"weight_NoC_leakage_power = "<<weight_NoC_leakage_power<<endl;

				NoC_data_file<<weight_NoC_area<<", "<<weight_NoC_latency<<", "<<weight_NoC_dynamic_power<<", "<<weight_NoC_leakage_power<<", ";

				float NoC_area = weight_NoC_area + ifmap_NoC_area;
				float NoC_latency = max(weight_NoC_latency, ifmap_NoC_latency);
				float NoC_dynamic_power = weight_NoC_dynamic_power + ifmap_NoC_dynamic_power;
				float NoC_leakage_power = weight_NoC_leakage_power + ifmap_NoC_leakage_power;
				cout<<"NoC_area = "<<NoC_area<<endl;
				cout<<"NoC_latency = "<<NoC_latency<<endl;
				cout<<"NoC_dynamic_power = "<<NoC_dynamic_power<<endl;
				cout<<"NoC_leakage_power = "<<NoC_leakage_power<<endl;
				cout<<endl;

				NoC_data_file<<NoC_area<<", "<<NoC_latency<<", "<<NoC_dynamic_power<<", "<<NoC_leakage_power<<endl;

				
				/* ------------------ 3. CLK Network --------------------------*/
				float CLK_area;
				float CLK_latency;
				float CLK_dynamic_power;
				float CLK_leakage_power;

				get_CLK_AreaLatencyPower(
					cycle_time,
					clk_bitwidth,
					clk_fan_in,
					clk_fan_out,
					&CLK_area,
					&CLK_latency,
					&CLK_dynamic_power,
					&CLK_leakage_power
			        );

				CLK_area = 0;
				CLK_latency = 0;
				CLK_dynamic_power = 0;
				CLK_leakage_power = 0;

				cout<<"CLK_area = "<<CLK_area<<endl;
				cout<<"CLK_latency = "<<CLK_latency<<endl;
				cout<<"CLK_dynamic_power = "<<CLK_dynamic_power<<endl;
				cout<<"CLK_leakage_power = "<<CLK_leakage_power<<endl;

				CLK_data_file<<CLK_area<<", "<<CLK_latency<<", "<<CLK_dynamic_power<<", "<<CLK_leakage_power<<endl;


				latency_division_data_file<<ifmap_NoC_latency<<", "<<weight_NoC_latency<<", ";





























				













				out_data_file<<ttm<<", "<<tte<<", "<<ttc<<", ";
				float exe_time = ceil(M/ttm) * ceil(E/tte) * F * ceil(C/ttc) * cycle_time;
				out_data_file<<exe_time<<", ";



				float total_area = PE_area + LB_area + GLB_area + NoC_area + CLK_area;
				float PE_area_div = PE_area / total_area;
				float LB_area_div = LB_area / total_area;
				float GLB_area_div = GLB_area / total_area;
				float NoC_area_div = NoC_area / total_area;
				float CLK_area_div = CLK_area / total_area;
				area_division_data_file<<GLB_area_div<<", "<<LB_area_div<<", "<<NoC_area_div<<", "<<PE_area_div<<", "<<CLK_area_div<<endl;


				float total_latency = GLB_latency + LB_latency + NoC_latency + PE_latency;
				float PE_latency_div = PE_latency / total_latency;
				float LB_latency_div = LB_latency / total_latency;
				float GLB_latency_div = GLB_latency / total_latency;
				float NoC_latency_div = NoC_latency / total_latency;
				float CLK_latency_div = CLK_latency / total_latency;
				latency_division_data_file<<GLB_latency_div<<", "<<LB_latency_div<<", "<<NoC_latency_div<<", "<<PE_latency_div<<", "<<CLK_latency_div<<endl;



				float total_dynamic_power = GLB_dynamic_power + LB_dynamic_power + NoC_dynamic_power + PE_dynamic_power + CLK_dynamic_power;
				float PE_dynamic_power_div = PE_dynamic_power / total_dynamic_power;
				float LB_dynamic_power_div = LB_dynamic_power / total_dynamic_power;
				float GLB_dynamic_power_div = GLB_dynamic_power / total_dynamic_power;
				float NoC_dynamic_power_div = NoC_dynamic_power / total_dynamic_power;
				float CLK_dynamic_power_div = CLK_dynamic_power / total_dynamic_power;
				dynamic_power_division_data_file<<GLB_dynamic_power_div<<", "<<LB_dynamic_power_div<<", "<<NoC_dynamic_power_div<<", "<<PE_dynamic_power_div<<", "<<CLK_dynamic_power_div<<endl;

				float total_energy = total_dynamic_power * exe_time / 1000 + dram_energy; // mW * nS / K = pJ/K = nJs
				float GLB_energy = GLB_dynamic_power * exe_time / 1000;
				float LB_energy = LB_dynamic_power * exe_time / 1000;
				float NoC_energy = NoC_dynamic_power * exe_time / 1000;
				float PE_energy = PE_dynamic_power * exe_time / 1000;
				float CLK_energy = CLK_dynamic_power * exe_time / 1000;

				energy_data_file<<total_energy<<", "<<dram_energy<<", "<<GLB_energy<<", "<<LB_energy<<", "<<NoC_energy<<", "<<PE_energy<<", "<<CLK_energy<<", ";

				float dram_energy_div = dram_energy / total_energy;
				float GLB_energy_div = GLB_energy / total_energy;
				float LB_energy_div = LB_energy / total_energy;
				float NoC_energy_div = NoC_energy / total_energy;
				float PE_energy_div = PE_energy / total_energy;
				float CLK_energy_div = CLK_energy / total_energy;

				energy_data_file<<dram_energy_div<<", "<<GLB_energy_div<<", "<<LB_energy_div<<", "<<NoC_energy_div<<", "<<PE_energy_div<<", "<<CLK_energy_div<<endl;




				float total_leakage_power = GLB_leakage_power + LB_leakage_power + NoC_leakage_power + PE_leakage_power + CLK_leakage_power;
				float PE_leakage_power_div = PE_leakage_power / total_leakage_power;
				float LB_leakage_power_div = LB_leakage_power / total_leakage_power;
				float GLB_leakage_power_div = GLB_leakage_power / total_leakage_power;
				float NoC_leakage_power_div = NoC_leakage_power / total_leakage_power;
				float CLK_leakage_power_div = CLK_leakage_power / total_leakage_power;
				leakage_power_division_data_file<<GLB_leakage_power_div<<", "<<LB_leakage_power_div<<", "<<NoC_leakage_power_div<<", "<<PE_leakage_power_div<<", "<<CLK_leakage_power_div<<endl;


				cout<<"total_area = "<<total_area<<endl;
				cout<<"total_latency = "<<total_latency<<endl;
				cout<<"total_dynamic_power = "<<total_dynamic_power<<endl;
				cout<<"total_leakage_power = "<<total_leakage_power<<endl;

				out_data_file<<total_area<<", "<<total_latency<<", "<<total_dynamic_power<<", "<<total_leakage_power<<endl;









			//}
		//}
	//}

	dram_data_file.close();
	energy_data_file.close();
	out_data_file.close();

	
	GLB_data_file.close();
	LB_data_file.close();
	NoC_data_file.close();
	PE_data_file.close();

	PE_sum_data_file.close();

	PE_area_division_data_file.close();
	PE_power_division_data_file.close();
	PE_latency_division_data_file.close();

	CLK_data_file.close();

	area_division_data_file.close();
	latency_division_data_file.close();
	dynamic_power_division_data_file.close();
	leakage_power_division_data_file.close();	

	return 0;

}