// ItemDataStructs.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
// #include "WeaponDataStructs.h"
// #include "ConsumableDataStructs.h"
// #include "AttachmentDataStructs.h"
#include "ItemDataStructs.generated.h"

class AItemBase;

/* ������ Ÿ�� Enum */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon		UMETA(DisplayName = "Weapon"),			// ����
	EIT_Consumable	UMETA(DisplayName = "Consumable"),		// �Һ� ������
	EIT_Attachment	UMETA(DisplayName = "Attachment"),		// ������
	EIT_MAX			UMETA(Hidden)
};

/* ������ ���� ������ */
USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	/* ������ �̸� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	/* ��ȣ�ۿ� �� �޽��� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText InteractionText;

	/* ������ ��� �� �޽��� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText UsageText;
};

/* ������ ��ġ ������ */
USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	/* ������ ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;

	/* �ִ� ���� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;

	/* ���� ���� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;
};

/* ������ ���� ������ */
USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	/* ������ ������ (�κ��丮�� ǥ���� ������ - ������ ����) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;

	/* ���忡�� ������ ����ƽ �޽� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* StaticMesh;

	/* ���� �� ���� ���� ���̷�Ż �޽� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EIT_Weapon"))
	USkeletalMesh* SkeletalMesh;

	/* �������� ������ ���� �̸� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName AttachSocketName = NAME_None;
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

	/* ������ ���� ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FName ItemID;

	/* ������ Ÿ�� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	EItemType ItemType;

	/* ������ ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemTextData TextData;

	/* ������ ��ġ ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemNumericData NumericData;

	/* ������ ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemAssetData AssetData;

	/* ������ �ִϸ��̼� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	FItemAnimationData AnimationData;

	/* 
	 * �켱 �� �������� �����ʹ� �� ������ ���̽����� �����ϴ� �������� �����Ͽ����ϴ�.
	 * �� ������ ������ ������ �ʿ��ϴٰ� �Ǵܵ� ��� �Ʒ��� �ּ��� �����ϰ� ����� �� �ֽ��ϴ�.
	**/

	/* ���� ������ (������ ��쿡�� �Ҵ�) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon", meta = (EditCondition = "ItemType == EIT_Weapon"))
	// FWeaponItemData WeaponData;

	/* �Һ� ������ ������(�Һ� �������� ��쿡�� �Ҵ�) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Consumable", meta = (EditCondition = "ItemType == EIT_Consumable"))
	// FConsumableItemData ConsumableData;

	/* ���� ������ ������(���� �������� ��쿡�� �Ҵ�) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Attachment", meta = (EditCondition = "ItemType == EIT_Attachment"))
	// FAttachmentItemData AttachmentData;



	// �������� ������ �� �ʿ�(DropItem�� ����� ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	TSubclassOf<AItemBase> ItemActorClass;
};
