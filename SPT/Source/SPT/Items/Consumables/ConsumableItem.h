// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/ItemBase.h"
#include "ConsumableItem.generated.h"

UCLASS()
class SPT_API AConsumableItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	/* �⺻ ������ */
	AConsumableItem();

	/* ��Ŭ�� �� ȣ��Ǵ� �Լ� */
	virtual void PrimaryAction(class ASPTPlayerCharacter* PlayerCharacter) override;

	/* ��� �׼� ���� */
	void StartUse(ASPTPlayerCharacter* PlayerCharacter);

	/* ��� �׼� �Ϸ� �� ���� */
	void FinishUse();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Consumable")
	float DestroyDelay;

};
