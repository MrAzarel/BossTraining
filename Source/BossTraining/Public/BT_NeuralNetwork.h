// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Components/ActorComponent.h"
#include "BT_NeuralNetwork.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSTRAINING_API UBT_NeuralNetwork : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBT_NeuralNetwork();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Train(const TArray<float>& Inputs);
	TArray<float> Predict(const TArray<float>& Inputs);

private:
	
    void InitializeWeights();
    float GetRandomWeight();
    float Sigmoid(float x);
    float SigmoidDerivative(float x);

    static constexpr int InputNodes = 8; // x и y владельца, x и y игрока, расстояние до игрока, здоровье владельца, здоровье игрока, последнее действие
    static constexpr int HiddenNodes1 = 12;
    static constexpr int HiddenNodes2 = 10;
    static constexpr int OutputNodes = 5; // три атаки, отсутствие атаки, движение по X и Y
    float bias = 0.5f;

    float LearningRate;
    TArray<TArray<float>> WeightsInputHidden1;
    TArray<TArray<float>> WeightsHidden1Hidden2;
    TArray<TArray<float>> WeightsHiddenOutput;
    TArray<float> HiddenLayer1Outputs;
    TArray<float> HiddenLayer2Outputs;
    TArray<float> FinalOutputs;
};
