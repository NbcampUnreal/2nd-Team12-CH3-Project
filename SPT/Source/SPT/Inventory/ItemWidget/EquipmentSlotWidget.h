// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UInventoryItem;


UCLASS()
class SPT_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;  // 장착된 아이템 아이콘

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;  // 장착된 아이템 이름

    UFUNCTION()
    void UpdateSlot(UInventoryItem* EquippedItem);  // 슬롯 업데이트
};
