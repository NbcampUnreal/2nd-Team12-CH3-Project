// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

class UItemDataObject;
class AItemBase;
class AWeaponBase;

UCLASS()
class SPT_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	// 아이템 데이터 타입을 가져와 저장
	UPROPERTY()
	UItemDataObject* ItemDataObject;



	// 무기 관련해서 임시로 만든 변수
	UPROPERTY()
	AItemBase* ItemBaseReference;



	// 아이템 데이터를 복사하여 인벤토리 데이터로 저장
	void SetItemData(UItemDataObject* NewItemDataObject);



	// 아이템 데이터를 호출하는 함수
	TSubclassOf<AItemBase> GetItemBaseClass() const;

	// 아이템이 장비일 경우 클래스를 호출하는 함수
	TSubclassOf<AWeaponBase> GetWeaponClass() const;

	void SetItemBase(AItemBase* NewItemBase);
	AItemBase* GetItemBase() const;


	FText GetItemName() const;

	UTexture2D* GetItemIcon() const;

	USkeletalMesh* GetSkeletalMesh() const;
	

	// 아이템의 타입을 판별
	bool IsWeapon() const;

	bool IsConsumable() const;

	int32 GetSlotType() const;



	// 임시용 인벤토리 내 아이템 사용함수
	void UseItem();
};
