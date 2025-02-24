// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemActor.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"

AConsumableItemActor::AConsumableItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConsumableItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	Super::OnPickup(PlayerCharacter);
}

void AConsumableItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	Super::OnDrop(PlayerCharacter);
}

void AConsumableItemActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Use: PlayerCharacter is null"));
		return;
	}

	// �Һ� ������ ȿ�� ����
	ApplyConsumableEffect(PlayerCharacter);

	// ������ ���� ����
	if (--Quantity <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s has been consumed completely! Removing from inventory."), *ItemData.TextData.Name.ToString());
		Destroy(); // �κ��丮���� ����
	}
}

void AConsumableItemActor::ApplyConsumableEffect_Implementation(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyConsumableEffect: PlayerCharacter is null"));
		return;
	}

	// ��� ȿ�� ����
	UE_LOG(LogTemp, Log, TEXT("Applying effect of %s"), *GetName());

	// TODO: ü�� ȸ��, ���¹̳� ȸ�� ���� ȿ�� ����
	// ����: PlayerCharacter->IncreaseHealth(ItemData.NumericData.Value);

	// ���� ȿ���� �ִ� ���, Ÿ�̸Ӹ� ����Ͽ� ȿ�� ����
	if (ItemData.NumericData.EffectDuration > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("%s effect will persist for %f seconds"), *GetName(), ItemData.NumericData.EffectDuration);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Effect, this, &AConsumableItemActor::RemoveEffect, ItemData.NumericData.EffectDuration, false);
	}
}

void AConsumableItemActor::RemoveEffect()
{
	// TODO: ȿ�� ���� ���� ����
	UE_LOG(LogTemp, Log, TEXT("Effect of %s has ended"), *GetName());
}
