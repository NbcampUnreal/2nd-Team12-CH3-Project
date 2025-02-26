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
	/* 기본 생성자 */
	AConsumableItem();

	/* 좌클릭 시 호출되는 함수 */
	virtual void PrimaryAction(class ASPTPlayerCharacter* PlayerCharacter) override;

	/* 사용 액션 시작 */
	void StartUse(ASPTPlayerCharacter* PlayerCharacter);

	/* 사용 액션 완료 후 제거 */
	void FinishUse();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Consumable")
	float DestroyDelay;

};
