// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.h"
#include "ItemDataStructs.generated.h"

/* ������ Ÿ�� Enum */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),			// ����
	EIT_Consumable UMETA(DisplayName = "Consumable"),	// �Ҹ�ǰ
	EIT_MAX UMETA(Hidden)
};

/* ������ ���� ������ */
USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Name;	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText InteractionText;	// ��ȣ�ۿ� �޽���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText UsageText;	// ��� �� �޽���
};

/* ������ ��ġ ������ */
USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;	// �ִ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;	// ���� ���� ����
};

/* ������ ���� ������ */
USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;	// ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* StaticMesh;	// ���忡�� ������ ����ƽ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EIT_Weapon"))
	USkeletalMesh* SkeletalMesh;	// ���� �� ���� SkeletalMesh (����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName AttachSocketName = NAME_None;	// ������ ���� �̸� (���� ������ �����۸� ���)
};

/* ��ü ������ ������ */
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FName ItemID;	// ������ ���� ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	EItemType ItemType;	// ������ Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemTextData TextData;	// ������ ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemNumericData NumericData;	// ������ ��ġ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemAssetData AssetData;	// ������ ���� ������

	/* ���� ������ (������ ��쿡�� �Ҵ�) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon", meta = (EditCondition = "ItemType == EIT_Weapon"))
	FWeaponItemData WeaponData;
};
