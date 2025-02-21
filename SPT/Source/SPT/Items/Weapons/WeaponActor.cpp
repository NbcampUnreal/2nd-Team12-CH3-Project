// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	AttachSocketName = "hand_r";
}

bool AWeaponActor::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return false;
	}

	if (PlayerCharacter->EquippedItem)
	{
		PlayerCharacter->UnEquipItem(); // 기존 장착 아이템 해제
	}

	PlayerCharacter->EquippedItem = this;
	AttachToComponent(
		PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		AttachSocketName);

	UpdateItemState(EItemState::EIS_Equipped);

	// TODO: 무기 장착 애니메이션 실행
	/*
	if (EquipAnimation && PlayerCharacter->GetMesh()->GetAnimInstance())
	{
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(EquipAnimation);
		UE_LOG(LogTemp, Log, TEXT("AWeaponActor: Playing equip animation for %s"), *GetName());
	}
	*/

	// TODO: 무기 전용 기능 (예: 탄약 설정, 조준 설정)
	// - 탄약 설정: Reload() 초기화
	// - 조준 상태 초기화
	// - UI 업데이트

	UE_LOG(LogTemp, Log, TEXT("AWeaponActor: %s is Equipped with additional weapon setup"), *GetName());

	return true;
}

bool AWeaponActor::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || PlayerCharacter->EquippedItem != this)
	{
		return false;
	}

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PlayerCharacter->EquippedItem = nullptr;
	UpdateItemState(EItemState::EIS_QuickSlot);

	return true;
}

void AWeaponActor::Drop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || PlayerCharacter->EquippedItem != this)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = nullptr;
	SpawnParams.Instigator = PlayerCharacter;

	AWorldItemActor* DroppedItem = PlayerCharacter->GetWorld()->SpawnActor<AWorldItemActor>(
		AWorldItemActor::StaticClass(),
		PlayerCharacter->GetActorLocation() + FVector(50, 0, 0),
		FRotator::ZeroRotator,
		SpawnParams);

	if (DroppedItem)
	{
		DroppedItem->InitializeItem(ItemData);
	}

	PlayerCharacter->UnEquipItem();
	Destroy();
}

EWeaponType AWeaponActor::GetWeaponType() const
{
	return WeaponData.WeaponType;
}

FWeaponItemData AWeaponActor::GetWeaponData() const
{
	return WeaponData;
}
