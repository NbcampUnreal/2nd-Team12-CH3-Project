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

	// 소비 아이템 효과 적용
	ApplyConsumableEffect(PlayerCharacter);

	// 아이템 개수 감소
	if (--Quantity <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s has been consumed completely! Removing from inventory."), *ItemData.TextData.Name.ToString());
		Destroy(); // 인벤토리에서 삭제
	}
}

void AConsumableItemActor::ApplyConsumableEffect_Implementation(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyConsumableEffect: PlayerCharacter is null"));
		return;
	}

	// 즉시 효과 적용
	UE_LOG(LogTemp, Log, TEXT("Applying effect of %s"), *GetName());

	// TODO: 체력 회복, 스태미너 회복 등의 효과 적용
	// 예제: PlayerCharacter->IncreaseHealth(ItemData.NumericData.Value);

	// 지속 효과가 있는 경우, 타이머를 사용하여 효과 유지
	if (ItemData.NumericData.EffectDuration > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("%s effect will persist for %f seconds"), *GetName(), ItemData.NumericData.EffectDuration);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Effect, this, &AConsumableItemActor::RemoveEffect, ItemData.NumericData.EffectDuration, false);
	}
}

void AConsumableItemActor::RemoveEffect()
{
	// TODO: 효과 제거 로직 구현
	UE_LOG(LogTemp, Log, TEXT("Effect of %s has ended"), *GetName());
}
