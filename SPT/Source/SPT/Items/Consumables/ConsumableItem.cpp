// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"
#include "SPTPlayerCharacter.h"

AConsumableItem::AConsumableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// 기본 ?�태???�드 ?�태
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

	// ?�레?�어 ?�니메이???�행
	if (UAnimMontage*  UseAnimation = ItemData->GetItemData().AnimationData.UseAnimation)
	{
		PlayerCharacter->PlayAnimMontage(UseAnimation);
	}

	// ?�정 ?�간 ???�동 ??��
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FinishUse();
		});
}

void AConsumableItem::FinishUse()
{
	Destroy();
}
