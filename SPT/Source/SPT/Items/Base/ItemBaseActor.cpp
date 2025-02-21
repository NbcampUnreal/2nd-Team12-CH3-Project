// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBaseActor.h"
#include "SPTPlayerCharacter.h"

AItemBaseActor::AItemBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Skeletal Mesh Component 생성 (무기)
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	Quantity = 1;
	ItemState = EItemState::EIS_World; // 기본적으로 월드에 존재
}

void AItemBaseActor::UpdateItemState(EItemState NewState)
{
	ItemState = NewState;
	UE_LOG(LogTemp, Log, TEXT("ItemBaseActor:: Item state updated: %d"), static_cast<int32>(ItemState));
}

// 아이템 복제
AItemBaseActor* AItemBaseActor::CreateItemCopy() const
{
	AItemBaseActor* NewItem = NewObject<AItemBaseActor>();
	if (NewItem)
	{
		NewItem->SetItemData(ItemData);
		NewItem->Quantity = Quantity;
		NewItem->UpdateItemState(ItemState);
	}

	return NewItem;
}

// 아이템 사용
void AItemBaseActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemBaseActor:: Item Used: %s"), *ItemData.TextData.Name.ToString());


	// 소비 아이템에서 추가
}

// 아이템 데이터 설정
void AItemBaseActor::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
	Quantity = 1;

	// 아이템 유형에 따라 적절한 메시 설정
	if (ItemData.ItemType == EItemType::EIT_Weapon && ItemData.AssetData.SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(ItemData.AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetVisibility(true);
		StaticMeshComponent->SetVisibility(false);
	}
	else if (ItemData.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(ItemData.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
		SkeletalMeshComponent->SetVisibility(false);
	}
}

UStaticMeshComponent* AItemBaseActor::GetMeshComponent() const
{
	return StaticMeshComponent;
}

FItemData AItemBaseActor::GetItemData() const
{
	return ItemData;
}

bool AItemBaseActor::IsWeapon() const
{
	return ItemData.ItemType == EItemType::EIT_Weapon;
}
