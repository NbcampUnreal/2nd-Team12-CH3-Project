// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.h"
#include "ItemDataStructs.generated.h"

/* 아이템 타입 Enum */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),			// 무기
	EIT_Consumable UMETA(DisplayName = "Consumable"),	// 소모품
	EIT_MAX UMETA(Hidden)
};

/* 아이템 문자 데이터 */
USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Name;	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText InteractionText;	// 상호작용 메시지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText UsageText;	// 사용 시 메시지
};

/* 아이템 수치 데이터 */
USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;	// 아이템 무게
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;	// 최대 스택 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;	// 스택 가능 여부
};

/* 아이템 에셋 데이터 */
USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;	// 아이템 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* StaticMesh;	// 월드에서 보여질 스태틱 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EIT_Weapon"))
	USkeletalMesh* SkeletalMesh;	// 장착 시 사용될 SkeletalMesh (무기)
};

/* 전체 아이템 데이터 */
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FName ItemID;	// 아이템 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	EItemType ItemType;	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemTextData TextData;	// 아이템 문자 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemNumericData NumericData;	// 아이템 수치 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemAssetData AssetData;	// 아이템 에셋 데이터

	/* 무기 데이터 (무기인 경우에만 할당) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon", meta = (EditCondition = "ItemType == EIT_Weapon"))
	FWeaponItemData WeaponData;
};
