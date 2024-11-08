// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BT_BossHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSTRAINING_API UBT_BossHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBT_BossHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return Health <= 0.0f; };

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth = 1000.0f;

	virtual void BeginPlay() override;

private:	

	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);
};
