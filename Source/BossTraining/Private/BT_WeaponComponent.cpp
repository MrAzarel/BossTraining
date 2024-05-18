// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_WeaponComponent.h"
#include "BT_Sword.h"
#include "GameFramework/Character.h"

UBT_WeaponComponent::UBT_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}


void UBT_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void UBT_WeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	CurrentWeapon = GetWorld()->SpawnActor<ABT_Sword>(WeaponClass);
	if (!CurrentWeapon) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
	CurrentWeapon->SetOwner(Character);
}

void UBT_WeaponComponent::Hit() 
{
	if (!CurrentWeapon) return;
	CurrentWeapon->Hit();
}
