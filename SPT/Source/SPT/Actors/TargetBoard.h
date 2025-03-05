#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetBoard.generated.h"


UCLASS()
class SPT_API ATargetBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetBoard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* MeshComp;

private:

    UFUNCTION()
    void OnHit(
        UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    UPROPERTY(EditAnywhere, Category = "Setup")
    class ARecordBoard* RecordBoard;
};
