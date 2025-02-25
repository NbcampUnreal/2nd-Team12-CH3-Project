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
    // ����� �߰� �Ӽ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    int32 AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    int32 DefensePower;

    // ��� ������ �� �ִ� ���� (��: �Ӹ�, ����, ���� ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FName EquipmentSlot;
};
