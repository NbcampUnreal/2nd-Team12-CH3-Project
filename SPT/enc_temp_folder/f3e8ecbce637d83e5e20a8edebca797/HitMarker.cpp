// Fill out your copyright notice in the Description page of Project Settings.


#include "HitMarker.h"

void UHitMarker::PlayAnimHitMarker()
{
    if (Anim_Hit)  // �ִϸ��̼��� �����ϸ� ����
    {
        UE_LOG(LogTemp, Log, TEXT("PlayAnimation"));
        PlayAnimation(Anim_Hit);
    }
}
