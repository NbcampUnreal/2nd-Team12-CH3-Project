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

    // TakeDamage 오버라이드 (FHitResult 가져오기)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

private:


    UPROPERTY(EditAnywhere, Category = "Setup")
    class ARecordBoard* RecordBoard;
};
