// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConsumableDataStructs.generated.h"

/* 소비 아이템 데이터 구조체 */
USTRUCT(BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	int32 Quantity; // 기본 소비 아이템 개수 (붕대는 5개, 드링크는 1개 등등..)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	float RestoreAmount;  // 체력, 스태미너 등 회복량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	float Duration;  // 지속 시간 (버프 아이템용)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	bool bIsInstant;  // 즉시 효과 여부

};
