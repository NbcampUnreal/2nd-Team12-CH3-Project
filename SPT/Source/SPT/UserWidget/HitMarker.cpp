// Fill out your copyright notice in the Description page of Project Settings.


#include "HitMarker.h"

void UHitMarker::PlayAnimHitMarker()
{
    if (Anim_Hit)  // 애니메이션이 존재하면 실행
    {
        PlayAnimation(Anim_Hit);
    }
}
