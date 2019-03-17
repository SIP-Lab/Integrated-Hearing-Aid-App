//
//  TensorFlowInference.m
//  Integrated_App
//
//  Created by Abhishek Sehgal on 7/20/18.
//  Copyright © 2018 UT Dallas. All rights reserved.
//

#import "TensorFlowInference.h"
#import <tensorflow/core/public/session.h>

@implementation TensorFlowInference {
    tensorflow::GraphDef graph;
    tensorflow::Session *session;
}

/*--------------------------Tensorflow Code-----------------------*/

// Input Tensor
tensorflow::Tensor x(tensorflow::DT_FLOAT,
                     tensorflow::TensorShape({40,40}));


std::vector<std::pair<std::string, tensorflow::Tensor>> inputs = {
    {"inputs/x-input", x},
};
std::vector<std::string> nodes = {
    {"model/Softmax"}
};
std::vector<tensorflow::Tensor> outputs;

/**
 Load the tensorflow graph into memory from the specified path
 
 @param path Path to the graph on the machine
 @return If the graph has been loaded or not
 */
- (BOOL)loadGraphFromPath:(NSString *)path {
    
    auto status = ReadBinaryProto(tensorflow::Env::Default(), path.fileSystemRepresentation, &graph);
    if (!status.ok()) {
        NSLog(@"Error reading graph: %s", status.error_message().c_str());
        return NO;
    }
    
    // This prints out the names of the nodes in the graph.
    auto nodeCount = graph.node_size();
    //NSLog(@"Node count: %d", nodeCount);
    for (auto i = 0; i < nodeCount; ++i) {
        auto node = graph.node(i);
        //NSLog(@"Node %d: %s '%s'", i, node.op().c_str(), node.name().c_str());
    }
    
    return YES;
}


/**
 Start the tensorflow session
 
 @return If the session has been started or not
 */
- (BOOL)createSession {
    tensorflow::SessionOptions options;
    auto status = tensorflow::NewSession(options, &session);
    if (!status.ok()) {
        NSLog(@"Error creating session %s",
              status.error_message().c_str());
        return NO;
    }
    
    status = session->Create(graph);
    if (!status.ok()) {
        NSLog(@"Error adding graph to session: %s",
              status.error_message().c_str());
        return NO;
    }
    return YES;
}

- (void)predict: (VADNoiseReductionCompression *) vadNoiseRedCom  settings: (Settings *) settings{
    
    auto input  = x.tensor<float, 2>();
    
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            input(i, j) = vadNoiseRedCom->melSpectrogram->melSpectrogramImage[i][j];
        }
    }
    
    auto status = session->Run(inputs, nodes, {}, &outputs);
    if(!status.ok()) {
        NSLog(@"Error running model: %s", status.error_message().c_str());
        return;
    }
    
    auto pred = outputs[0].tensor<float, 2>();
    settings->classLabel = round(pred(0,1)) + 1;
    NSLog(@"%f", pred(0,1));
}

@end
