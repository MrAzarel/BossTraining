// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"
#include "BT_Boss.generated.h"

class UBT_BossHealthComponent;
class UTextRenderComponent;
class UBT_NeuralNetwork;
class UBT_WeaponComponent;

UCLASS()
class BOSSTRAINING_API ABT_Boss : public ACharacter
{
	GENERATED_BODY()

public:
	ABT_Boss();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBT_BossHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBT_WeaponComponent* WeaponComponent;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	UBT_NeuralNetwork* NeuralNetwork;

	TArray<float> Inputs;
	TArray<float> Outputs;

	void FillInputs();
	void OnHealthChanged(float Health);
	void OnDeath();

	float LoacationX = GetActorLocation().X;
	float LoacationY = GetActorLocation().Y;

	float FirstAttack = 0.f;
	float SecondAttack = 0.f;
	float ThirdAttack = 0.f;
};
