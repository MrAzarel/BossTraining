// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Sword.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "BT_TBSword.h"
#include "GameFramework/Character.h"
#include "BT_Player.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBTPWeapon, All, All);

// Sets default values
ABT_Sword::ABT_Sword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	SwordBox = CreateDefaultSubobject<ABT_TBSword>("SwordBox");
	SwordBox->AttachToActor(this, AttachmentTransformRules, "Trigger");
}

// Called when the game starts or when spawned
void ABT_Sword::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);

}

void ABT_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SwordBox && SwordBox->isAttacking)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character->GetMesh()->IsPlaying() == false)
		{
			SwordBox->isAttacking = false;
		}
	}
}

void ABT_Sword::Hit() 
{
	UE_LOG(LogBTPWeapon, Display, TEXT("Hit"));

	if (SwordBox)
	{
		SwordBox->isAttacking = true;
	}
	MakeHit();
}


void ABT_Sword::MakeHit() 
{
	if (!GetWorld()) return;

	const auto Controller = GetPlayerController();
	if (!Controller) return;

	const FTransform StartSocketTransform = WeaponMesh->GetSocketTransform(StartSocketName);
	const FTransform EndSocketTransform = WeaponMesh->GetSocketTransform(EndSocketName);

	const FVector TraceStart = StartSocketTransform.GetLocation();
	const FVector TraceEnd = EndSocketTransform.GetLocation();
 
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FHitResult HitResult;

	//ECC_Visability change to boss collision
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		//ActorHit->Destroy();
	}
}

void ABT_Sword::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

APlayerController* ABT_Sword::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}