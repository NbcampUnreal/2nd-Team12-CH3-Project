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

	/* ������ �ݴ� ��� */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;
	
	/* �Һ� ������ ������ ���̺� */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* ConsumableDataTable;

	/* �����Ϳ��� ������ ���̺� ������Ʈ */
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
