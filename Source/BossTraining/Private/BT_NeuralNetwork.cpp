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

    TArray<float> Inputs = { /* ��������� ���������������� ���������� */ };
    Train(Inputs);
    TArray<float> Outputs = Predict(Inputs);
}

void UBT_NeuralNetwork::InitializeWeights()
{
    // ������������� ����� ��� ������� �������� ����
    WeightsInputHidden1.SetNum(InputNodes);
    for (int i = 0; i < InputNodes; ++i)
    {
        WeightsInputHidden1[i].SetNum(HiddenNodes1);
        for (int j = 0; j < HiddenNodes1; ++j)
        {
            WeightsInputHidden1[i][j] = GetRandomWeight();
        }
    }

    // ������������� ����� ��� ������� �������� ����
    WeightsHidden1Hidden2.SetNum(HiddenNodes1);
    for (int i = 0; i < HiddenNodes1; ++i)
    {
        WeightsHidden1Hidden2[i].SetNum(HiddenNodes2);
        for (int j = 0; j < HiddenNodes2; ++j)
        {
            WeightsHidden1Hidden2[i][j] = GetRandomWeight();
        }
    }

    // ������������� ����� ��� ��������� ����
    WeightsHiddenOutput.SetNum(HiddenNodes2);
    for (int i = 0; i < HiddenNodes2; ++i)
    {
        WeightsHiddenOutput[i].SetNum(OutputNodes);
        for (int j = 0; j < OutputNodes; ++j)
        {
            WeightsHiddenOutput[i][j] = GetRandomWeight();
        }
    }

    HiddenLayer1Outputs.SetNum(HiddenNodes1);
    HiddenLayer2Outputs.SetNum(HiddenNodes2);
    FinalOutputs.SetNum(OutputNodes);
}

float UBT_NeuralNetwork::GetRandomWeight()
{
    return FMath::FRand() - bias;
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
    // ������ ���������������
    for (int i = 0; i < HiddenNodes1; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < InputNodes; ++j)
        {
            Sum += Inputs[j] * WeightsInputHidden1[j][i];
        }
        HiddenLayer1Outputs[i] = Sigmoid(Sum);
    }

    for (int i = 0; i < HiddenNodes2; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes1; ++j)
        {
            Sum += HiddenLayer1Outputs[j] * WeightsHidden1Hidden2[j][i];
        }
        HiddenLayer2Outputs[i] = Sigmoid(Sum);
    }

    for (int i = 0; i < OutputNodes; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes2; ++j)
        {
            Sum += HiddenLayer2Outputs[j] * WeightsHiddenOutput[j][i];
        }
        FinalOutputs[i] = Sigmoid(Sum);
    }

    // ������ �� �������� ����
    TArray<float> OutputErrors;
    OutputErrors.SetNum(OutputNodes);
    for (int i = 0; i < OutputNodes; ++i)
    {
        OutputErrors[i] = FinalOutputs[i] * (1.0f - FinalOutputs[i]) * (FinalOutputs[i] - Inputs[i % InputNodes]);
    }

    // ������ �� ������ ������� ����
    TArray<float> HiddenErrors2;
    HiddenErrors2.SetNum(HiddenNodes2);
    for (int i = 0; i < HiddenNodes2; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < OutputNodes; ++j)
        {
            Sum += OutputErrors[j] * WeightsHiddenOutput[i][j];
        }
        HiddenErrors2[i] = HiddenLayer2Outputs[i] * (1.0f - HiddenLayer2Outputs[i]) * Sum;
    }

    // ������ �� ������ ������� ����
    TArray<float> HiddenErrors1;
    HiddenErrors1.SetNum(HiddenNodes1);
    for (int i = 0; i < HiddenNodes1; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes2; ++j)
        {
            Sum += HiddenErrors2[j] * WeightsHidden1Hidden2[i][j];
        }
        HiddenErrors1[i] = HiddenLayer1Outputs[i] * (1.0f - HiddenLayer1Outputs[i]) * Sum;
    }

    // ���������� �����
    for (int i = 0; i < HiddenNodes2; ++i)
    {
        for (int j = 0; j < OutputNodes; ++j)
        {
            WeightsHiddenOutput[i][j] -= LearningRate * OutputErrors[j] * HiddenLayer2Outputs[i];
        }
    }

    for (int i = 0; i < HiddenNodes1; ++i)
    {
        for (int j = 0; j < HiddenNodes2; ++j)
        {
            WeightsHidden1Hidden2[i][j] -= LearningRate * HiddenErrors2[j] * HiddenLayer1Outputs[i];
        }
    }

    for (int i = 0; i < InputNodes; ++i)
    {
        for (int j = 0; j < HiddenNodes1; ++j)
        {
            WeightsInputHidden1[i][j] -= LearningRate * HiddenErrors1[j] * Inputs[i];
        }
    }
}

TArray<float> UBT_NeuralNetwork::Predict(const TArray<float>& Inputs)
{
    // ������ ���������������
    for (int i = 0; i < HiddenNodes1; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < InputNodes; ++j)
        {
            Sum += Inputs[j] * WeightsInputHidden1[j][i];
        }
        HiddenLayer1Outputs[i] = Sigmoid(Sum);
    }

    for (int i = 0; i < HiddenNodes2; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes1; ++j)
        {
            Sum += HiddenLayer1Outputs[j] * WeightsHidden1Hidden2[j][i];
        }
        HiddenLayer2Outputs[i] = Sigmoid(Sum);
    }

    for (int i = 0; i < OutputNodes; ++i)
    {
        float Sum = 0.0f;
        for (int j = 0; j < HiddenNodes2; ++j)
        {
            Sum += HiddenLayer2Outputs[j] * WeightsHiddenOutput[j][i];
        }
        FinalOutputs[i] = Sigmoid(Sum);
    }

    return FinalOutputs;
}