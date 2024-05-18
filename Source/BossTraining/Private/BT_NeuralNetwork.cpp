// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_NeuralNetwork.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

// Sets default values for this component's properties
UBT_NeuralNetwork::UBT_NeuralNetwork()
{
	PrimaryComponentTick.bCanEverTick = true;

    LearningRate = 0.1f;
    InitializeWeights();
}

// Called when the game starts
void UBT_NeuralNetwork::BeginPlay()
{
    Super::BeginPlay();

}


// Called every frame
void UBT_NeuralNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TArray<float> Inputs = { /* Заполните соответствующими значениями */ };
    Train(Inputs);
    TArray<float> Outputs = Predict(Inputs);
}

void UBT_NeuralNetwork::InitializeWeights()
{
    WeightsInputHidden.SetNum(InputNodes);
    for (int i = 0; i < InputNodes; ++i)
    {
        WeightsInputHidden[i].SetNum(HiddenNodes);
        for (int j = 0; j < HiddenNodes; j++)
        {
            WeightsInputHidden[i][j] = GetRandomWeight();
        }
    }

    WeightsHiddenOutput.SetNum(HiddenNodes);
    for (int i = 0; i < HiddenNodes; ++i)
    {
        WeightsHiddenOutput[i].SetNum(OutputNodes);
        for (int j = 0; j < OutputNodes; j++)
        {
            WeightsHiddenOutput[i][j] = GetRandomWeight();
        }
    }

    HiddenLayerOutputs.SetNum(HiddenNodes);
    FinalOutputs.SetNum(OutputNodes);
}

float UBT_NeuralNetwork::GetRandomWeight()
{
    return FMath::FRand() - 0.5f;
}

float UBT_NeuralNetwork::Sigmoid(float x)
{
    return 1.0f / (1.0f + FMath::Exp(-x));
}

float UBT_NeuralNetwork::SigmoidDerivative(float x)
{
    return x * (1.0f - x);
}

void UBT_NeuralNetwork::Train(const TArray<float>& Inputs)
{
    // Прямое распространение
    for (int i = 0; i < HiddenNodes; i++)
    {
        float Sum = 0.0f;
        for (int j = 0; j < InputNodes; j++)
        {
            Sum += Inputs[j] * WeightsInputHidden[j][i];
        }
        HiddenLayerOutputs[i] = Sigmoid(Sum);
    }

    for (int i = 0; i < OutputNodes; i++)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes; j++)
        {
            Sum += HiddenLayerOutputs[j] * WeightsHiddenOutput[j][i];
        }
        FinalOutputs[i] = Sigmoid(Sum);
    }

    // Ошибки на выходном слое
    TArray<float> OutputErrors;
    OutputErrors.SetNum(OutputNodes);
    for (int i = 0; i < OutputNodes; i++)
    {
        OutputErrors[i] = FinalOutputs[i] * (1 - FinalOutputs[i]) * (FinalOutputs[i] - Inputs[i % InputNodes]);
    }

    // Ошибки на скрытом слое
    TArray<float> HiddenErrors;
    HiddenErrors.SetNum(HiddenNodes);
    for (int i = 0; i < HiddenNodes; i++)
    {
        float Sum = 0.0f;
        for (int j = 0; j < OutputNodes; j++)
        {
            Sum += OutputErrors[j] * WeightsHiddenOutput[i][j];
        }
        HiddenErrors[i] = HiddenLayerOutputs[i] * (1 - HiddenLayerOutputs[i]) * Sum;
    }

    // Обновление весов
    for (int i = 0; i < HiddenNodes; i++)
    {
        for (int j = 0; j < OutputNodes; j++)
        {
            WeightsHiddenOutput[i][j] -= LearningRate * OutputErrors[j] * HiddenLayerOutputs[i];
        }
    }

    for (int i = 0; i < InputNodes; i++)
    {
        for (int j = 0; j < HiddenNodes; j++)
        {
            WeightsInputHidden[i][j] -= LearningRate * HiddenErrors[j] * Inputs[i];
        }
    }
}

TArray<float> UBT_NeuralNetwork::Predict(const TArray<float>& Inputs)
{
    // Прямое распространение
    for (int i = 0; i < HiddenNodes; i++)
    {
        float Sum = 0.0f;
        for (int j = 0; j < InputNodes; j++)
        {
            Sum += Inputs[j] * WeightsInputHidden[j][i];
        }
        HiddenLayerOutputs[i] = Sigmoid(Sum);
    }

    for (int i = 0; i < OutputNodes; i++)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes; j++)
        {
            Sum += HiddenLayerOutputs[j] * WeightsHiddenOutput[j][i];
        }
        FinalOutputs[i] = Sigmoid(Sum);
    }

    return FinalOutputs;
}
