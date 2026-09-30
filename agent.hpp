#ifndef AGENT_HPP
#define AGENT_HPP

#include <vector>

using namespace std;

class NeuralNetwork {
    private:
        int inputSize;
        int hiddenSize;
        int outputSize;
        vector<vector<double>> weightsInputToHidden;
        vector<vector<double>> weightsHiddenToOutput;
        vector<double> biasesHidden;
        vector<double> biasesOutput;

    public:
        NeuralNetwork(int inputSize, int hiddenSize, int outputSize);
        vector<float> forward(const vector<float>& inputs);
};


#endif