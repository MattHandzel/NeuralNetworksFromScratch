#include "NeuralLayer.h"
#include <iostream>

NeuralLayer::NeuralLayer(int size, WeightGenerationType weightGenerationType = WeightGenerationType::RANDOM) : m_layerSize(size),  m_weightGenerationType(weightGenerationType)
{
    // Creates neurons based on the size of the layer
    for (int i = 0; i < size; i++)
    {
        this->m_neurons.push_back(CreateNeuron());
    }
}

Neuron NeuralLayer::CreateNeuron()
{
    // Initial bias is so that in the future we can make the initial bias 0, random, etc.
    double initialBias = 0;
    Neuron n = Neuron(initialBias);
    n.SetActivationFunction(m_activationFunction);

    return n;
}

void NeuralLayer::ProcessLayer()
{
    for (int i = 0; i < m_neurons.size(); i++)
    {
        m_neurons[i].ProcessData();
        // std::cout << "Neuron " << i << " has a value now of " << m_neurons[i].m_value << " and" << m_neurons[i].GetConnections().size() << " connections\n";
    }
}


void NeuralLayer::SetInput(const std::vector<double>& input)
{
    // If the size of the input is not the same as the layer size then it will throw an exception
    if (input.size() != m_layerSize)
    {
        throw std::invalid_argument("Input (size: " + std::to_string(input.size()) + " )" + "must be equal in size to layer size (size: " + std::to_string(m_layerSize) + ") ");
    }

    // Set the value of the neurons in the layer to the inputs
    for (int i = 0; i < m_layerSize; i++)
    {
        // m_neurons[i].ProcessInput(input[i]);
        m_neurons[i].SetValue(input[i]);
    }
}

std::vector<Neuron> &NeuralLayer::GetNeurons()
{
    return m_neurons;
}

std::vector<double> NeuralLayer::GetValues(){
    std::vector<double> values;
    for(int i = 0; i < m_neurons.size(); i++){
        values.push_back(m_neurons.at(i).m_value);
    }
    return values;
}


void NeuralLayer::ConnectLayer(NeuralLayer& other, const WeightGenerationType& weightGenerationType){
    this->ConnectLayer(other, generateWeights(GetLayerSize(), other.GetLayerSize(), m_weightGenerationType));
}
void NeuralLayer::ConnectLayer(NeuralLayer& other){
    this->ConnectLayer(other, m_weightGenerationType);
}

void NeuralLayer::ConnectLayer(NeuralLayer& other, const std::vector<std::vector<double>>& weights)
{
    assert(m_activationFunctionSet);
    this->m_previous = &other;

    // Change the weight value here, default to 1, you might wanna make it random, i dunno how this stuff works
    // double weightValue = generateRandomNumber(1, -1);
    // double weightValue = 0.2;
    //  For each neuron in this layer, connect it to the neuron in the other layer
    for (int i = 0; i < m_layerSize; i++)
    {
        for (int b = 0; b < other.GetLayerSize(); b++)
        {
            std::cout << b << std::endl;
            m_neurons.at(i).AddConnection(&(other.m_neurons.at(b)), weights.at(i).at(b)); // generateRandomNumber(1, -1)
        }
    }
}

void NeuralLayer::SetWeights(const std::vector<std::vector<double>>& weights){
    if(weights.size() != m_layerSize || weights.at(0).size() != m_previous->GetLayerSize()){
        throw std::invalid_argument("Weight dimensions are " + std::to_string(weights.size()) + "x" + std::to_string(weights.at(0).size()) + " when they should be" + std::to_string(m_layerSize) + "x" + std::to_string(m_previous->GetLayerSize()));
    }
    for (int i = 0; i < m_layerSize; i++)
    {
        for (int b = 0; b < m_previous->GetLayerSize(); b++)
        {
            m_neurons.at(i).SetWeight(b, weights.at(i).at(b)); // generateRandomNumber(1, -1)
        }
    }
}

void NeuralLayer::SetBiases(const std::vector<double>& baises){
    for(int i = 0; i < m_layerSize; i++){
        m_neurons.at(i).SetBias(baises.at(i));
    }
}

void NeuralLayer::SetActivationFunction(ActivationFunction activationFunction){
    m_activationFunctionSet = true;
    m_activationFunction = activationFunction;
    for(int i = 0; i < m_neurons.size(); i++){
        m_neurons.at(i).SetActivationFunction(m_activationFunction);
    }
}

int NeuralLayer::GetLayerSize()
{
    return m_layerSize;
}

std::string NeuralLayer::ConnectionsToString()
{
    // Concatenates the connection message from all of the neurons into one string
    std::string message = "";
    for (Neuron n : m_neurons)
    {
        message += n.ConnectionsToString();
    }
    return message;
}

void NeuralLayer::operator()(NeuralLayer& otherLayer)
{
    this->ConnectLayer(otherLayer);
}

void NeuralLayer::operator()(const std::vector<double>& input)
{
    // If there is another layer before this layer, then set the input of that layer to the passed inputs
    if (m_previous != NULL)
    {
        m_previous->SetInput(input);
        return;
    }

    // If that is not true, set the values of the neurons to the input
    this->SetInput(input);
}

std::string NeuralLayer::ValuesToString()
{
    std::string message = "";
    int i = 0;
    // for each neuron, get the value and concatenate it into one string
    for (Neuron n : m_neurons)
    {
        message += "NEURON:\n\tVALUE:\t" + std::to_string(n.m_value) + "\n";
    }
    return message;
}

NeuralLayer& NeuralLayer::GetPrevious()
{
    return *m_previous;
}

std::vector<double> NeuralLayer::FindGradient(){
    
    return std::vector<double>();
}

// Find the total connections a layer has
int sumConnections(NeuralLayer& layer)
{
    int sum = 0;
    for (int i = 0; i < layer.GetLayerSize(); i++)
    {
        sum += layer.GetNeurons()[i].m_neural_connections.size();
    }
    return sum;
}

std::vector<std::vector<double>> generateWeights(NeuralLayer& layer_1, NeuralLayer& layer_2, WeightGenerationType& type, double weight = 0){
    return generateWeights(layer_1.GetLayerSize(), layer_2.GetLayerSize(), type, weight);
}