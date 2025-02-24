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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float EffectDuration;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Transform")
	FVector EquipPositionOffset;  // ���� ��ġ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Transform")
	FRotator EquipRotationOffset;  // ���� ȸ�� ������
};

/* ������ �ִϸ��̼� ������ ����ü */
USTRUCT(BlueprintType)
struct FItemAnimationData
{
	GENERATED_USTRUCT_BODY()

	/* ������ ��� �ִϸ��̼� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	UAnimMontage* UseAnimation;

	/* �ִϸ��̼� ���� �� ������ ���� ���� �ð� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	float AnimationDelayAfterUse;

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

	/* �ִϸ��̼� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	FItemAnimationData AnimationData;

	/* ���� ������ (������ ��쿡�� �Ҵ�) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon", meta = (EditCondition = "ItemType == EIT_Weapon"))
	FWeaponItemData WeaponData;

};
