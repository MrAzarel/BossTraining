// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BT_Sword.generated.h"

class ABT_TBSword;

UCLASS()
class BOSSTRAINING_API ABT_Sword : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABT_Sword();

	virtual void Hit();

	UPROPERTY(BlueprintReadWrite)
	bool IsAttacking;


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName StartSocketName = "StartSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName EndSocketName = "EndSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Animation")
	ABT_TBSword* SwordBox;
	
	FCollisionQueryParams TraceParams;

	virtual void BeginPlay() override;

	void MakeHit();
	APlayerController* GetPlayerController() const;

	void MakeDamage(const FHitResult& HitResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

};
