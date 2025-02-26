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

	/* �������� �ݴ� ��� */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;

	/* �޽� ������Ʈ */
	void UpdateMesh();

protected:
	/* SkeletalMeshComponent (�ѱ�, ��ô ���� ����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMeshComponent;

	/* ���� ������ ���̺� */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* WeaponDataTable;

	/* �����Ϳ��� ������ ���̺� ������Ʈ */
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

};
