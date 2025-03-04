// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

class UItemDataObject;
class AItemBase;

UCLASS()
class SPT_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	// ������ ������ Ÿ���� ������ ����
	UPROPERTY()
	UItemDataObject* ItemDataObject;


	// ������ �����͸� �����Ͽ� �κ��丮 �����ͷ� ����
	void SetItemData(UItemDataObject* NewItemDataObject);



	// ������ �����͸� ȣ���ϴ� �Լ�
	TSubclassOf<AItemBase> GetItemBaseClass() const;

	FText GetItemName() const;

	UTexture2D* GetItemIcon() const;

	USkeletalMesh* GetSkeletalMesh() const;
	

	// �������� Ÿ���� �Ǻ�
	bool IsWeapon() const;

	bool IsConsumable() const;

	int32 GetSlotType() const;



	// �ӽÿ� �κ��丮 �� ������ ����Լ�
	void UseItem();
};
