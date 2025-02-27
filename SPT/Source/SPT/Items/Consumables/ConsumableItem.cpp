// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"
#include "SPTPlayerCharacter.h"

AConsumableItem::AConsumableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// �⺻ ���´� ���� ����
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

	// �÷��̾� �ִϸ��̼� ����
	if (UAnimMontage*  UseAnimation = ItemData->GetItemData().AnimationData.UseAnimation)
	{
		PlayerCharacter->PlayAnimMontage(UseAnimation);
	}

	// ���� �ð� �� �ڵ� ����
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FinishUse();
		});
}

void AConsumableItem::FinishUse()
{
	Destroy();
}
