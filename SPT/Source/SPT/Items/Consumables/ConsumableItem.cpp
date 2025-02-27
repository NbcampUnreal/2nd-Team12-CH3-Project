// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"
#include "SPTPlayerCharacter.h"

AConsumableItem::AConsumableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// 기본 상태는 월드 상태
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

	// 플레이어 애니메이션 실행
	if (UAnimMontage*  UseAnimation = ItemData->GetItemData().AnimationData.UseAnimation)
	{
		PlayerCharacter->PlayAnimMontage(UseAnimation);
	}

	// 일정 시간 후 자동 삭제
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FinishUse();
		});
}

void AConsumableItem::FinishUse()
{
	Destroy();
}
