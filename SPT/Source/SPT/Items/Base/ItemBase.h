// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "ItemBase.generated.h"

class ASPTPlayerCharacter;

// ������ ���� Enum
UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_World		UMETA(DisplayName = "World"),    // �������� ���忡 ������ ����
	EIS_Inventory   UMETA(DisplayName = "Inventory"),   // �������� �κ��丮�� ������ �������� ���� ����
	EIS_QuickSlot	UMETA(DisplayName = "QuickSlot"),	// �����Կ� ������ ����
	EIS_Equipped	UMETA(DisplayName = "Equipped"),  // �������� �տ� ������ ����
	EIS_MAX			UMETA(Hidden)
};

UCLASS()
class SPT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	/* �⺻ ������ */
	AItemBase();

	/* ������ �ʱ�ȭ (������ ���̺��� �ε�) */
	void InitializeItem(FName ItemRowName);


	void InitializeItem(FName ItemRowName,
		UDataTable* ItemDataTable,
		UDataTable* WeaponTable,
		UDataTable* ConsumableTable);

	/* ���콺 ��Ŭ�� �� ������ �⺻ ���� */
	virtual void PrimaryAction(class ASPTPlayerCharacter* PlayerCharacter) PURE_VIRTUAL(AItemBase::PrimaryAction, );

	/* ���¿� ���� �޽� ������Ʈ */
	void UpdateMeshForState(EItemState NewState);

	/* ������ ���� */
	AItemBase* CreateItemCopy() const;

	/* Getter & Setter */
	EItemState GetItemState() const;
	void SetItemState(EItemState NewItemState);
	UItemDataObject* GetItemData() const;
	void SetItemData(UItemDataObject* NewItemData);

protected:

	/* ������ ���� (����, �κ��丮, ����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EItemState ItemState;

	/* ������ ������ (���� ���� ����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	UItemDataObject* ItemData;

	/* ������ �޽� (���忡�� �ð��� ǥ��) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	UStaticMeshComponent* StaticMeshComponent;

};
