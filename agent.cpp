#include "agent.hpp"
#include <random>

using namespace std;

NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize) : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize) {
    // Inicia a matriz de pesos entre a camada de entrada e camada oculta com valores aleatórios
    for(int i=0; i<hiddenSize; i++) {
        for(int j=0; j<inputSize; j++) {
            weightsInputToHidden[i][j] = ((float) rand() / RAND_MAX) * 2 - 1;
        }
    }

    // Inicia a matriz de pesos entre a camada oculta e camada de saída com valores aleatórios
    for(int i=0; i<outputSize; i++) {
        for(int j=0; j<hiddenSize; j++) {
            weightsHiddenToOutput[i][j] = ((float) rand() / RAND_MAX) * 2 - 1;
        }
    }

    // Inicia os vieses da camada oculta com valores zerados
    biasesHidden.resize(hiddenSize, 0.0);
    // Inicia os vieses da camada de saída com valores zerados
    biasesOutput.resize(outputSize, 0.0);
}

vector<float> NeuralNetwork::forward(const vector<float>& inputs) {
    vector <float> hiddenLayer(hiddenSize, 0.0);

    for(int i=0; i<hiddenSize; i++) {
        for(int j=0; j<inputSize; j++) {
            hiddenLayer[i] += inputs[j] * weightsInputToHidden[i][j];
        }
        hiddenLayer[i] += biasesHidden[i];
    }

    for(float& value : hiddenLayer) {
        value = value < 0 ? 0 : value;
    }

    vector<float> outputLayer(outputSize, 0.0);

    for(int i=0; i<outputSize; i++) {
        for(int j=0; j<hiddenSize; j++) {
            outputLayer[i] += hiddenLayer[j] * weightsHiddenToOutput[i][j];
        }
        outputLayer[i] += biasesOutput[i];
    }
    return outputLayer;
}
