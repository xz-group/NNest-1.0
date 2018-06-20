# NNest-1.0

NNest: Early-Stage Design Space Exploration Tool for Neural Network Inference Accelerators

NNest explores NN accelerator's design space in two kind of dataflow: IWR and PR.</br>
main_IWR.cpp: the main function for IWR dataflow</br>
main_PR.cpp: the main function for PR dataflow</br>

You can change the Makefile accordingly to compile the different main file.

The XXX_model.h defines the different blocks including Global buffer, local buffer, BBus, and MACs units.

Define the certain NN model and constraint in user_input.txt in certain format: </br>
1st row: NN models, currently only support Alexnet and VGG </br>
2nd row: performance constraint (ms) </br>
3rd row: area constraint (um) </br>
4th row: energy constraint (uJ) </br>
5th row: input bitwidth </br>
6th row: weight bitwidth </br>
7th row: psum bitwidth </br>


