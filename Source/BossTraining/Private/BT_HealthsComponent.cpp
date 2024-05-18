// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_HealthsComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UBT_HealthsComponent::UBT_HealthsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UBT_HealthsComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
	
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UBT_HealthsComponent::OnTakeAnyDamage);
	}
}

void UBT_HealthsComponent::OnTakeAnyDamage(
	AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || IsDead()) return;

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);


	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}
