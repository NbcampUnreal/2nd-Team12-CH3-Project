// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Worlds/WorldItemBase.h"
#include "WorldWeapon.generated.h"

class AWeaponBase;
class AFirearmWeapon;
class AMeleeWeapon;
class AThrowableWeapon;

UCLASS()
class SPT_API AWorldWeapon : public AWorldItemBase
{
	GENERATED_BODY()

public:
	AWorldWeapon();

	/* 아이템을 줍는 기능 */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 메시 업데이트 */
	void UpdateMesh();

protected:
	/* SkeletalMeshComponent (총기, 투척 무기 전용) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMeshComponent;

	/* 무기 데이터 테이블 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* WeaponDataTable;

	/* 에디터에서 데이터 테이블 업데이트 */
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

};
