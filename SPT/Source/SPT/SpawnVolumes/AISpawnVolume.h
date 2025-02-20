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
	// ���� ���� ���ο��� ������ X,Y ��ǥ�� ������ �Լ�(Z���� Volume�� �ּҰ�)
	FVector GetRandomXYPointInVolume() const;

	// AI�� �����Ѵ�.
	ACharacter* SpawnAI();

protected:
	// �ӽ� ��� ����
	// ���� ������ ���̺�� ������ AIŬ�������� �����Ѵٸ� �����ؾ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ACharacter> SpawnAIClass;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	USceneComponent* Scene;
	// ���� ������ ����� �ڽ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	UBoxComponent* SpawningBox;
};
