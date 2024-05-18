// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BT_HealthsComponent.h"
#include "Components/TextRenderComponent.h"
#include "BT_WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BTPlayerLog, All, All);

ABT_Player::ABT_Player()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UBT_HealthsComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

	WeaponComponent = CreateDefaultSubobject<UBT_WeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ABT_Player::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ABT_Player::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABT_Player::OnHealthChanged);

	IsAttacking = false;
}

void ABT_Player::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

}

// Called every frame
void ABT_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABT_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABT_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABT_Player::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ABT_Player::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ABT_Player::AddControllerYawInput);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABT_Player::Jump);
	//PlayerInputComponent->BindAction("Hit", IE_Pressed, WeaponComponent, &UBT_WeaponComponent::Hit);
	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &ABT_Player::StartAttack);

}

void ABT_Player::MoveForward(float Amount) 
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ABT_Player::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}

void ABT_Player::OnDeath() 
{
	UE_LOG(BTPlayerLog, Display, TEXT("Player %s is dead"), *GetName());

	PlayAnimMontage(DeathAnimMontage);

	//GetCharacterMovement()->DisableMovement();

	SetLifeSpan(1.0f);
}

//Trying to make animation
void ABT_Player::StartAttack() 
{
	if (!AttackAnimation) return;

	if (!IsAttacking)
	{
		PlayAnimMontage(AttackAnimation);

		IsAttacking = true;
	}
}

void ABT_Player::LineTrace() 
{
	WeaponComponent->Hit();
}