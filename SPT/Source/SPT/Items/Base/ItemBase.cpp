// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Skeletal Mesh Component 생성 (무기)
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// 아이템 데이터 오브젝트 초기화
	ItemData = CreateDefaultSubobject<UItemDataObject>(TEXT("ItemData"));
}

void AItemBase::InitializeItem(FName ItemRowName)
{
	if (ItemData)
	{
		// ItemData에서 데이터 초기화
		ItemData->InitializeFromDataTable(ItemRowName);

		const FItemData& LoadedItemData = ItemData->GetItemData();

		// 메시 설정 (블루프린트에서 설정된 메시가 있으면 적용)
		if (LoadedItemData.AssetData.StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(LoadedItemData.AssetData.StaticMesh);
			StaticMeshComponent->SetVisibility(true);
			SkeletalMeshComponent->SetVisibility(false);
		}
		else if (LoadedItemData.AssetData.SkeletalMesh)
		{
			SkeletalMeshComponent->SetSkeletalMesh(LoadedItemData.AssetData.SkeletalMesh);
			SkeletalMeshComponent->SetVisibility(true);
			StaticMeshComponent->SetVisibility(false);
		}
	}
}

void AItemBase::PrimaryAction(ASPTPlayerCharacter* PlayerCharacter)
{
	// 기본적으로 PrimaryAction은 하위 클래스에서 오버라이드하여 사용
	UE_LOG(LogTemp, Warning, TEXT("%s: PrimaryAction()가 구현되지 않음!"), *ItemData->GetItemData().TextData.ItemName.ToString());
}

EItemState AItemBase::GetItemState() const
{
	return ItemState;
}

void AItemBase::SetItemState(EItemState NewItemState)
{
	if (NewItemState != EItemState::EIS_MAX)
	{
		ItemState = NewItemState;
	}
}

UItemDataObject* AItemBase::GetItemData() const
{
	return ItemData;
}

void AItemBase::SetItemData(UItemDataObject* NewItemData)
{
	if (NewItemData)
	{
		ItemData = NewItemData;
	}
}
