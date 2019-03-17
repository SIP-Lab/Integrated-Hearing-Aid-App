
#include "VADRandomForest.h"
#include "VADTrainData.h"

VADRandomForests* initVADRandomForest(int decisionBufferLength)
{
	VADRandomForests* newRandomForest;

	newRandomForest = (VADRandomForests*)malloc(sizeof(VADRandomForests));
	newRandomForest->numTrees = numTrees;
	newRandomForest->numClasses = numClasses;
	newRandomForest->vadClassDecision = -1;
	newRandomForest->vadTreeVotes = (int*)malloc(numTrees*sizeof(int));
	//newRandomForest->classDecisionCount = (int*)malloc(nClasses*sizeof(int));


	return newRandomForest;
}

void evalVADTrees(VADRandomForests* VADRandomForest, float* inputFeatureList)
{
	VADRandomForests* rf = VADRandomForest;
	int i;
	int current_node;
	int cvar;
	//int tree_output[nTrees];
	int classVotes[numClasses];
	int max;
	//int mismatch_count=0;
    //float newNormalizedClassDecision;

	// Initialize vote counts to zero
	for(i=0;i<numClasses;i++)
	{
		classVotes[i]=0;
	}

    for(i = 0; i<rf->numTrees ;i++)
    {
        current_node = 0;
        while (childnode[i][current_node]!=0)
        {
            cvar = nodeCutVar[i][current_node];
            //if (inputFeatureList[i + (cvar-1)*M] < nodeCutValue[i][current_node])
            if( (inputFeatureList[cvar-1]) < nodeCutValue[i][current_node])
            	current_node = childnode[i][current_node]-1;
            else current_node = childnode[i][current_node];
        }
        rf->vadTreeVotes[i] = nodelabel[i][current_node]; // for debug
        classVotes[(nodelabel[i][current_node])-1]++;
    }

    // Check which class received the most votes
    max=classVotes[0];
    rf->vadClassDecision = 1;
    //rf->classDecisionCount[0]=classVotes[0];
    for(i=1;i<rf->numClasses;i++)
    {
    	//rf->classDecisionCount[i]=classVotes[i];
    	if(classVotes[i]>max)
    	{
    		max=classVotes[i];
    		rf->vadClassDecision=i+1;
    	}
    }

//	// Multiply the class decision by 1/N
//	newNormalizedClassDecision = rf->Normalize * (float)rf->classDecision;
//	// Remove the oldest decision from the average
//	rf->floatAverageClassDecision -= rf->classDecisionBuffer[rf->oldestClassDecision];
//	// Add the newest decision to the average
//	rf->floatAverageClassDecision += newNormalizedClassDecision;
//	// Store the newest cbn to the periodicity buffer
//	rf->classDecisionBuffer[rf->oldestClassDecision]=newNormalizedClassDecision;
//
//	// Round the floating point average class to an integer class
//	rf->averageClassDecision = (int) (rf->floatAverageClassDecision + 0.5);
//	// Update the pointer in the decision buffer
//	if (rf->oldestClassDecision < rf->bufferLength-1)
//		rf->oldestClassDecision++;
//	else
//		rf->oldestClassDecision=0;
}

void destroyVADRandomForest(VADRandomForests** rf)
{
	if((*rf)->vadTreeVotes != NULL){
		free((*rf)->vadTreeVotes);
		(*rf)->vadTreeVotes = NULL;
	}
//	if((*rf)->classDecisionCount != NULL){
//		free((*rf)->classDecisionCount);
//		(*rf)->classDecisionCount = NULL;
//	}
	if(*rf != NULL){
		free(*rf);
		*rf=NULL;
	}
}
