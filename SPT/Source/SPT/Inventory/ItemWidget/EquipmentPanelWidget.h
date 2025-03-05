// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentPanelWidget.generated.h"

class UEquipmentSlotWidget;
class AEquipmentSlotInventory;

UCLASS()
class SPT_API UEquipmentPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 실제 구현되어있지 않은 아이템(임의로 제가 만든 부분입니다.)
    //UPROPERTY(meta = (BindWidget))
    //UEquipmentSlotWidget* HeadSlot;

    //UPROPERTY(meta = (BindWidget))
    //UEquipmentSlotWidget* BodySlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlotWidget* WeaponSlot;

    void UpdateEquipmentSlots(AEquipmentSlotInventory* EquipmentSlotInventory);
};
