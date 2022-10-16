


#include "Character/XWeaponActor.h"
#include "Character/XCharacterBase.h"
#include "GameplayTagContainer.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"


AXWeaponActor::AXWeaponActor() 
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USphereComponent>("RootComp");
	RootComponent = RootComp;
	RootComp->SetCollisionProfileName("NoCollision");
	RootComp->SetSphereRadius(4.0f);
	

	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponComp->SetupAttachment(RootComponent);
	WeaponComp->SetCollisionProfileName("NoCollision");

	DefaultCollision = CreateDefaultSubobject<UCapsuleComponent>("DefaultCollision");
	DefaultCollision->SetupAttachment(RootComponent);
	DefaultCollision->SetCollisionProfileName("OverlapOnlyPawn");

}

void AXWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	OnBeginWeaponAttack.AddDynamic(this, &AXWeaponActor::BeginWeaponAttack);
	OnEndWeaponAttack.AddDynamic(this, &AXWeaponActor::EndWeaponAttack);
	OnActorBeginOverlap.AddDynamic(this, &AXWeaponActor::BeginOverlap);

	DefaultCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void AXWeaponActor::BeginWeaponAttack(FGameplayTag EventTag, float AttackDelayTime)
{
	HitActors.Empty();
	DefaultCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetIsAttacking(true);
}

void AXWeaponActor::EndWeaponAttack(FGameplayTag EventTag)
{
	HitActors.Empty();

	DefaultCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetIsAttacking(false);
}

void AXWeaponActor::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != WeaponUser && !HitActors.Contains(OtherActor) && WeaponUser->GetLocalRole() == ROLE_Authority )
	{
		HitActors.Add(OtherActor);

		FGameplayEventData EventData;
		EventData.Instigator = OtherActor;
		EventData.Target = OtherActor;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(WeaponUser, FGameplayTag::RequestGameplayTag(TEXT("Event.Montage.Hit")), EventData);
	}
}

void AXWeaponActor::SetIsAttacking(bool bNewAttackStatus)
{
	bIsAttacking = bNewAttackStatus;
}

AXCharacterBase* AXWeaponActor::GetWeaponUser()
{
	return WeaponUser;
}

void AXWeaponActor::SetWeaponUser(AXCharacterBase* User)
{
	WeaponUser = User;
}

bool AXWeaponActor::AttachToCharacter()
{
	check(WeaponUser);
	AXCharacterBase* Player = Cast<AXCharacterBase>(WeaponUser);
	if (Player)
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Weapon_R"));

		return true;
	}
	return false;
}

bool AXWeaponActor::GetIsAttacking()
{
	return bIsAttacking;
}
