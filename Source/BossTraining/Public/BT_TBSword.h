// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BT_TBSword.generated.h"


UCLASS()
class BOSSTRAINING_API ABT_TBSword : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	ABT_TBSword();

	UFUNCTION()
	void Attack(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY()
	bool isAttacking;

};
