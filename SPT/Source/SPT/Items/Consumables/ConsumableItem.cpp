// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"
#include "SPTPlayerCharacter.h"

AConsumableItem::AConsumableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// ê¸°ë³¸ ?íƒœ???”ë“œ ?íƒœ
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

	// ?Œë ˆ?´ì–´ ? ë‹ˆë©”ì´???¤í–‰
	if (UAnimMontage*  UseAnimation = ItemData->GetItemData().AnimationData.UseAnimation)
	{
		PlayerCharacter->PlayAnimMontage(UseAnimation);
	}

	// ?¼ì • ?œê°„ ???ë™ ?? œ
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FinishUse();
		});
}

void AConsumableItem::FinishUse()
{
	Destroy();
}
