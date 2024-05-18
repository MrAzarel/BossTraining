// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BT_WeaponComponent.generated.h"

class ABT_Sword;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSTRAINING_API UBT_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBT_WeaponComponent();

	void Hit();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABT_Sword> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachPointName = "WeaponSocket";

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABT_Sword* CurrentWeapon = nullptr;

	void SpawnWeapon();
	
	
};
