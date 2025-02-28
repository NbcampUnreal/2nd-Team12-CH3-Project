// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Worlds/WorldItemBase.h"
#include "WorldConsumable.generated.h"

UCLASS()
class SPT_API AWorldConsumable : public AWorldItemBase
{
	GENERATED_BODY()

public:

	AWorldConsumable();

	virtual void BeginPlay() override;

	/* 아이템 줍는 기능 */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;
	
	/* 소비 아이템 데이터 테이블 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* ConsumableDataTable;

	/* 에디터에서 데이터 테이블 업데이트 */
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
