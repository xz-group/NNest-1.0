#include "util.h"

using namespace std;



int main(){

	float NN_model;
	AccConstraintStruct acc_constraint = {0,0,0};
	BitwidthStruct BW = {0, 0, 0};
	float cycle_time;
	get_user_input(&NN_model, &acc_constraint, &BW, &cycle_time);

	/*
	cout<<NN_model<<endl;
	cout<<acc_constraint.performance<<endl;
	cout<<acc_constraint.area<<endl;
	cout<<acc_constraint.energy<<endl;
	*/


	deque<SingleLayerStruct> nnsPMs;
	nnsPMs = load_NN_sturcture(NN_model);

	uaPMsStruct uaPMs = {BW, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	cout<<uaPMs.BW.I<<endl;

	float NumLayers = nnsPMs.size();

	while(!nnsPMs.empty()){
		SingleLayerStruct i_nnsPM = nnsPMs.front();
		nnsPMs.pop_front();

		cout<<i_nnsPM.layer_type<<", "<<i_nnsPM.M<<", "<<i_nnsPM.C<<", "<<i_nnsPM.R<<", "<<i_nnsPM.E<<endl;
		float DMS = 0; // 1 for IWR, 2 for PR

		for(DMS=1; DMS<=2; DMS++){
			//ideal condition, no fitting
			accPMsStruct accPMs = {i_nnsPM.C, i_nnsPM.F, i_nnsPM.R, i_nnsPM.S};

			// DMS = IWR
			if(DMS == 1){
				



			}

			// DMS = PR
			else if(DMS == 2){
				// 

			}

			
		}

	};







	return 0;

}