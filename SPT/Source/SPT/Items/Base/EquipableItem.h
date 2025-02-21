// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseActor.h"
#include "SPT/Interfaces/EquipableInterface.h"
#include "EquipableItem.generated.h"

UCLASS()
class SPT_API AEquipableItem : public AItemBaseActor, public IEquipableInterface
{
	GENERATED_BODY()
	
public:
    AEquipableItem();
     
    /* 아이템 장착 */
    virtual bool Equip(ASPTPlayerCharacter* PlayerCharacter) override;

    /* 아이템 해제 */
    virtual bool UnEquip(ASPTPlayerCharacter* PlayerCharacter)override;

    virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;
    
protected:
    /* 아이템 데이터에서 AttachSocketName을 가져와 설정 */
    void DetermineAttachSocket();

    /* 장착 시 연결할 소켓 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Equipable")
    FName AttachSocketName;
};
