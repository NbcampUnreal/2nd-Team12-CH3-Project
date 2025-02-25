// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component ����
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Skeletal Mesh Component ���� (����)
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// ������ ������ ������Ʈ �ʱ�ȭ
	ItemData = CreateDefaultSubobject<UItemDataObject>(TEXT("ItemData"));
}

void AItemBase::InitializeItem(FName ItemRowName)
{
	if (ItemData)
	{
		// ItemData���� ������ �ʱ�ȭ
		ItemData->InitializeFromDataTable(ItemRowName);

		const FItemData& LoadedItemData = ItemData->GetItemData();

		// �޽� ���� (�������Ʈ���� ������ �޽ð� ������ ����)
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
	// �⺻������ PrimaryAction�� ���� Ŭ�������� �������̵��Ͽ� ���
	UE_LOG(LogTemp, Warning, TEXT("%s: PrimaryAction()�� �������� ����!"), *ItemData->GetItemData().TextData.ItemName.ToString());
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
