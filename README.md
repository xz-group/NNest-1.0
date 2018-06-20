# NNest-1.0

NNest: Early-Stage Design Space Exploration Tool for Neural Network Inference Accelerators

NNest explores NN accelerator's design space in two kind of dataflow: IWR and PR. 
main_IWR.cpp: the main function for IWR dataflow
main_PR.cpp: the main function for PR dataflow
main.cc: the main function containing both IWR and PR, and labeled by dataflow parameters

You can change the Makefile accordingly to compile the different main file

The XXX_model.h defines the different blocks including Global buffer, local buffer, BBus, and MACs units.

Define the certain NN model and constraint in user_input.txt in certain format:
1st row: NN models, currently only support Alexnet and VGG

2nd row: performance constraint (ms)
3rd row: area constraint (um)
4th row: energy constraint (uJ)
5th row: input bitwidth
6th row: weight bitwidth
7th row: psum bitwidth


