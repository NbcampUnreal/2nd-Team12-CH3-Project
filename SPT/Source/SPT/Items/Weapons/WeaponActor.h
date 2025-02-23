// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "WeaponActor.generated.h"

UCLASS()
class SPT_API AWeaponActor : public AWorldItemActor
{
	GENERATED_BODY()

public:
	AWeaponActor();

	/* ���� ��� (���� ���� �Լ�) */
	virtual void Attack() PURE_VIRTUAL(AWeaponActor::Attack, );

	/* ������ �ݱ� */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;
	
	/* ������ ��� */
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);

	/* ���� ������ �������� */
	EWeaponType GetWeaponType() const;
	FWeaponItemData GetWeaponData() const;
};
