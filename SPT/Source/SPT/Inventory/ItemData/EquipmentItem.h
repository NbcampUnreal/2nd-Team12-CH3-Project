// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */
UCLASS()
class SPT_API UEquipmentItem : public UInventoryItem
{
	GENERATED_BODY()
	
public:
    // 장비의 추가 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    int32 AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    int32 DefensePower;

    // 장비가 장착될 수 있는 슬롯 (예: 머리, 몸통, 무기 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FName EquipmentSlot;
};
