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

	// �κ��丮�� ������ �߰�
	// PlayerCharacter->InventoryComponent->AddItem(GetItemData());

	UE_LOG(LogTemp, Log, TEXT("ConsumableItemActor: %s picked up %s"), *PlayerCharacter->GetName(), *GetName());

	// ���忡�� ���� (���� �κ��丮���� ������)
	Destroy();
}

void AConsumableItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// �������� ���� �� ���忡 �ٽ� ����
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

	// �Һ� ������ ȿ�� ����
	ApplyConsumableEffect(PlayerCharacter);

	// �ִϸ��̼� ���� (�ִϸ��̼��� ���� ���� �Ŀ��� ������)
	if (ItemData.AnimationData.UseAnimation && PlayerCharacter->GetMesh() && PlayerCharacter->GetMesh()->GetAnimInstance())
	{
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(ItemData.AnimationData.UseAnimation);
	}

	// ������ ��� ���� (�ִϸ��̼ǰ� ȿ���� ���������� ����)
	Destroy();
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
}
