// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/XHeroCharacter.h"
#include "Player/XPlayerState.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

AXHeroCharacter::AXHeroCharacter(const class FObjectInitializer& ObjectInitializer)
{

}

void AXHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AXHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AXHeroCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AXHeroCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AXHeroCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AXHeroCharacter::SprintStop);

	PlayerInputComponent->BindAction("MBL", IE_Pressed, this, &AXHeroCharacter::MBLAttack);
	PlayerInputComponent->BindAction("MBR", IE_Pressed, this, &AXHeroCharacter::MBRAttack);
	PlayerInputComponent->BindAction("ExtraSkill", IE_Pressed, this, &AXHeroCharacter::ExtraSkill);

}

void AXHeroCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AXHeroCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AXHeroCharacter::SprintStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Start!"));
}

void AXHeroCharacter::SprintStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Stop!"));
}

void AXHeroCharacter::MBLAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBL Attack!"));
}

void AXHeroCharacter::MBRAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBR Attack!"));
}

void AXHeroCharacter::ExtraSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Extra Skill!"));
}

void AXHeroCharacter::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Interact!"));
}

void AXHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	if (PS)
	{
		AbilitySystemComp = Cast<UXAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void AXHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	if (PS)
	{
		AbilitySystemComp = Cast<UXAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}
