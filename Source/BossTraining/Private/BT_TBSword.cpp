// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_TBSword.h"
#include "BT_Boss.h"

ABT_TBSword::ABT_TBSword() 
{
	OnActorBeginOverlap.AddDynamic(this, &ABT_TBSword::Attack);

	isAttacking = false;
}

void ABT_TBSword::BeginPlay() 
{
	Super::BeginPlay();
}

void ABT_TBSword::Attack(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		ABT_Boss* boss = Cast<ABT_Boss>(otherActor);

		
	}
}