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
	/*
	if (!PlayerCharacter || !PlayerCharacter->InventoryComponent)
	{
		return;
	}
	*/

	// 인벤토리에 아이템 추가
	// PlayerCharacter->InventoryComponent->AddItem(GetItemData());

	UE_LOG(LogTemp, Log, TEXT("ConsumableItemActor: %s picked up %s"), *PlayerCharacter->GetName(), *GetName());

	// 월드에서 제거 (이제 인벤토리에서 관리됨)
	Destroy();
}

void AConsumableItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 아이템을 버릴 때 월드에 다시 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.Instigator = PlayerCharacter;

	AConsumableItemActor* DroppedItem = GetWorld()->SpawnActor<AConsumableItemActor>(
		GetClass(),
		PlayerCharacter->GetActorLocation() + FVector(50, 0, 0),
		FRotator::ZeroRotator,
		SpawnParams);

	if (DroppedItem)
	{
		DroppedItem->SetItemData(GetItemData());
		UE_LOG(LogTemp, Log, TEXT("%s has dropped %s"), *PlayerCharacter->GetName(), *DroppedItem->GetName());
	}
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

	// 애니메이션 실행 (애니메이션은 액터 삭제 후에도 유지됨)
	if (ItemData.AnimationData.UseAnimation && PlayerCharacter->GetMesh() && PlayerCharacter->GetMesh()->GetAnimInstance())
	{
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(ItemData.AnimationData.UseAnimation);
	}

	// 아이템 즉시 삭제 (애니메이션과 효과는 독립적으로 실행)
	Destroy();
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
}
