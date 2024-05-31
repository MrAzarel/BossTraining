// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Boss.h"
#include "BT_BossHealthComponent.h"
#include "BT_NeuralNetwork.h"
#include "BT_HealthsComponent.h"
#include "BT_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(BTBosssLog, All, All);

// Sets default values
ABT_Boss::ABT_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UBT_BossHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

	NeuralNetwork = CreateDefaultSubobject<UBT_NeuralNetwork>("NeuralNetwork");

	WeaponComponent = CreateDefaultSubobject<UBT_WeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ABT_Boss::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ABT_Boss::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABT_Boss::OnHealthChanged);
}

void ABT_Boss::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

}

void ABT_Boss::OnDeath()
{
	UE_LOG(BTBosssLog, Display, TEXT("Player %s is dead"), *GetName());

	PlayAnimMontage(DeathAnimMontage);

	//GetCharacterMovement()->DisableMovement();

	SetLifeSpan(1.0f);
}

void ABT_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FillInputs();
	NeuralNetwork->Train(Inputs);

	Outputs = NeuralNetwork->Predict(Inputs);
}

void ABT_Boss::FillInputs() 
{
	LoacationX = GetActorLocation().X;
	LoacationY = GetActorLocation().Y;

	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) 
	{
		PlayerLoacationX = Player->GetActorLocation().X;
		PlayerLoacationY = Player->GetActorLocation().Y;
		PlayerHealth = Player.GetComponents()->HealthComponent->GetHealth();
	}

	Inputs.Add(LoacationX);
	Inputs.Add(LoacationY);
	Inputs.Add(PlayerLoacationX);
	Inputs.Add(PlayerLoacationY);
	Inputs.Add(FMath::Sqrt(FMath::Square(PlayerLoacationX) + FMath::Square(PlayerLoacationY)));
	Inputs.Add(HealthComponent->GetHealth());
	Inputs.Add(PlayerHealth);
	Inputs.Add(LastAction);
}