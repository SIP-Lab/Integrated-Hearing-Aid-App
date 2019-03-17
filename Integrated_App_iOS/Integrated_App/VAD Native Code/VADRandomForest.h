
#ifndef VADRANDOMFOREST_H_
#define VADRANDOMFOREST_H_
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>

typedef struct VADRandomForests {
	int     numTrees;				 // Number of trees
	int     numClasses;            // Number of noise classes
	int     vadClassDecision;       // Classifier output result
	int*  	vadTreeVotes; // class vote from each tree for debug
} VADRandomForests;

VADRandomForests* initVADRandomForest(int decisionBufferLength);
void evalVADTrees(VADRandomForests* RandomForest, float* inputFeatureList);
void destroyVADRandomForest(VADRandomForests** rf);

#endif /* RANDOMFOREST_H_ */
