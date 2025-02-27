// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Interfaces/InteractableInterface.h"
#include "WorldItemBase.generated.h"

UCLASS()
class SPT_API AWorldItemBase : public AItemBase, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	/* 기본 생성자 */
	AWorldItemBase();

	virtual void BeginPlay() override;

	/* 아이템 줍기 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter);

	/* 아이템 드롭 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);

	/* 상호작용 관련 함수*/
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 메시 업데이트 */
	void UpdateMesh();

	/* 에디터에서 아이템 데이터 테이블 업데이트 */
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void OnConstruction(const FTransform& Transform);
#endif

	/* 아이템 데이터 테이블 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* ItemDataTable;

	/* 아이템 ID */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	FName ItemID;

protected:

	/* 드롭 위치 계산 */
	FVector GetDropLocation(
		ASPTPlayerCharacter* PlayerCharacter,
		FVector& OutDropLocation,
		FRotator& OutDropRotation);

};
