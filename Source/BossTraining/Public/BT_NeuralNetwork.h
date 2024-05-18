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

	static constexpr int InputNodes = 6;
	static constexpr int HiddenNodes = 10;
	static constexpr int OutputNodes = 5;

	float LearningRate;
	TArray<TArray<float>> WeightsInputHidden;
	TArray<TArray<float>> WeightsHiddenOutput;
	TArray<float> HiddenLayerOutputs;
	TArray<float> FinalOutputs;
};
