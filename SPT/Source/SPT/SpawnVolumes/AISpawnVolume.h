// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPT_API AAISpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawnVolume();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// 스폰 볼륨 내부에서 무작위 X,Y 좌표를 얻어오는 함수(Z축은 Volume의 최소값)
	FVector GetRandomXYPointInVolume() const;

	// AI를 스폰한다.
	ACharacter* SpawnAI();

protected:
	// 임시 멤버 변수
	// 추후 데이터 테이블로 스폰할 AI클래스들을 관리한다면 제거해야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ACharacter> SpawnAIClass;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	USceneComponent* Scene;
	// 스폰 영역을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	UBoxComponent* SpawningBox;
};
