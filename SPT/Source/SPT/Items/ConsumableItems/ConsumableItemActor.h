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

	/* 아이템을 줍기 */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 아이템을 드롭 */
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 아이템 사용 (소모) */
	void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* 소비 아이템 효과 적용 - Blueprint에서 재정의 가능 */
	UFUNCTION(BlueprintNativeEvent, Category = "Consumable")
	void ApplyConsumableEffect(ASPTPlayerCharacter* PlayerCharacter);
	virtual void ApplyConsumableEffect_Implementation(ASPTPlayerCharacter* PlayerCharacter);

	/* 지속 효과 제거 */
	UFUNCTION()
	void RemoveEffect();

private:
	/* 지속 효과 제거 타이머 */
	FTimerHandle TimerHandle_Effect;

};
