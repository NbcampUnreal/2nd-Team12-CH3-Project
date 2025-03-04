// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"
#include "SPTPlayerCharacter.h"

AConsumableItem::AConsumableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// κΈ°λ³Έ ?ν???λ ?ν
	ItemState = EItemState::EIS_World;
}

void AConsumableItem::BeginPlay()
{
	Super::BeginPlay();
}

void AConsumableItem::PrimaryAction(ASPTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		StartUse(PlayerCharacter);
	}
}

void AConsumableItem::StartUse(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AConsumableUseActor::StartUse - PlayerCharacter is null!"));
		return;
	}

	// ?λ ?΄μ΄ ? λλ©μ΄???€ν
	if (UAnimMontage*  UseAnimation = ItemData->GetItemData().AnimationData.UseAnimation)
	{
		PlayerCharacter->PlayAnimMontage(UseAnimation);
	}

	// ?Όμ  ?κ° ???λ ?? 
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FinishUse();
		});
}

void AConsumableItem::FinishUse()
{
	Destroy();
}
