// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConsumableDataStructs.generated.h"

/* �Һ� ������ ������ ����ü */
USTRUCT(BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	int32 Quantity; // �⺻ �Һ� ������ ���� (�ش�� 5��, �帵ũ�� 1�� ���..)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	float RestoreAmount;  // ü��, ���¹̳� �� ȸ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	float Duration;  // ���� �ð� (���� �����ۿ�)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	bool bIsInstant;  // ��� ȿ�� ����

};
