// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemActor.h"

AConsumableItemActor::AConsumableItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConsumableItemActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("ConsumableItemActor: Cannot Use - Character is null!"));
        return;
    }

    // 손에 들고 있다가 사용하면 실행되는 로직
    UE_LOG(LogTemp, Log, TEXT("Using item: %s"), *GetName());

    // TODO: 체력 회복, 버프 적용 등의 로직 추가

    // 사용 후 아이템 제거
    Destroy();
}
