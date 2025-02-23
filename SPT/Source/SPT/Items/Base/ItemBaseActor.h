// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPT/Items/Data/ItemDataStructs.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "ItemBaseActor.generated.h"

class ASPTPlayerCharacter;


/* ������ ���� Enum*/
UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_World		UMETA(DisplayName = "World"),    // �������� ���忡 ������ ����
	EIS_Inventory   UMETA(DisplayName = "Inventory"),   // �������� �κ��丮�� ������ �������� ���� ����
	EIS_Equipped	UMETA(DisplayName = "Equipped"),  // �������� ������ ����
	EIS_QuickSlot	UMETA(DisplayName = "QuickSlot"),
	EIS_MAX			UMETA(Hidden)
};

UCLASS()
class SPT_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/* �⺻ ������ */
	AItemBaseActor();

	/* ������ ������ ���� */
	void SetItemData(const FItemData& NewItemData);

	/* ���¿� ���� �޽��� ���� */
	void UpdateMeshForState(EItemState NewState);

	/* ������ ���� ������Ʈ */
	void UpdateItemState(EItemState NewState);

	/* ������ ���� ��� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	AItemBaseActor* CreateItemCopy() const;

	/* ������ ��� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* Getter & Setter */
	UStaticMeshComponent* GetMeshComponent() const;
	FItemData GetItemData() const;
	bool IsWeapon() const;

	// ������ ���� (����, �κ��丮, ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EItemState ItemState;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ShowOnlyInnerProperties))
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

};
