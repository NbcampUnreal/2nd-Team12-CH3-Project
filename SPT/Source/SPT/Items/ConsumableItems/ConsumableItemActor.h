// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"
#include "ConsumableItemActor.generated.h"

class ASPTPlayerCharacter;

UCLASS()
class SPT_API AConsumableItemActor : public AWorldItemActor
{
	GENERATED_BODY()
	
public:
	AConsumableItemActor();

	/* �������� �ݱ� */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;

	/* �������� ��� */
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ������ ��� (�Ҹ�) */
	void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* �Һ� ������ ȿ�� ���� - Blueprint���� ������ ���� */
	UFUNCTION(BlueprintNativeEvent, Category = "Consumable")
	void ApplyConsumableEffect(ASPTPlayerCharacter* PlayerCharacter);
	virtual void ApplyConsumableEffect_Implementation(ASPTPlayerCharacter* PlayerCharacter);

	/* ���� ȿ�� ���� */
	UFUNCTION()
	void RemoveEffect();

private:
	/* ���� ȿ�� ���� Ÿ�̸� */
	FTimerHandle TimerHandle_Effect;

};
