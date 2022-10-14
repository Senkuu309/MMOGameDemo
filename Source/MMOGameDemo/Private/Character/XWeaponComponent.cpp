// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XWeaponComponent.h"
#include "Character/XCharacterBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"

UXWeaponComponent::UXWeaponComponent()
{

}

void UXWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UXWeaponComponent::WeaponBeginOverlap);
	OnComponentHit.AddDynamic(this, &UXWeaponComponent::WeaponHit);
	//OnComponentEndOverlap.AddDynamic(this, &UXWeaponComponent::WeaponEndOverlap);
}

void UXWeaponComponent::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* PlayerActor = GetOwner();

	AXCharacterBase* Player = Cast<AXCharacterBase>(PlayerActor);

	UE_LOG(LogTemp, Warning, TEXT("Weapon owner is %s"), *GetNameSafe(Player));

	if (Player)
	{
		if (OtherActor && PlayerActor->GetClass() != OtherActor->GetClass() && Player->GetIsAttacking())
		{
			if (!HitActors.Contains(OtherActor))
			{
				HitActors.Add(OtherActor);

				FGameplayEventData EventData;
				EventData.Instigator = Player;
				EventData.Target = OtherActor;

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerActor, FGameplayTag::RequestGameplayTag(FName("Event.Montage.Hit")), EventData);
			}
		}
	}
}

void UXWeaponComponent::WeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* PlayerActor = GetOwner();

	AXCharacterBase* Player = Cast<AXCharacterBase>(PlayerActor);

	UE_LOG(LogTemp, Warning, TEXT("Weapon owner is %s"), *GetNameSafe(Player));
}
