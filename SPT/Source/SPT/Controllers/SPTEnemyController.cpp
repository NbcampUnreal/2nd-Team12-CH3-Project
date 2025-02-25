#include "SPTEnemyController.h"
#include "SPTEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h" // RunBehaviorTree() ȣ�� �� UBehaviorTree* ���
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "NavigationSystem.h"
//#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h" 

ASPTEnemyController::ASPTEnemyController()
{
    BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ASPTEnemyController::BeginPlay()
{
    Super::BeginPlay();

     //������ �߰�: ���� ���� �� 1�� �ڿ� �ʱ�ȭ �õ�
    GetWorld()->GetTimerManager().SetTimer(
        BlackboardInitTimerHandle,
        this,
        &ASPTEnemyController::InitializeBlackboard,
        1.0f,
        false
    );
}

void ASPTEnemyController::InitializeBlackboard()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter)
    {
        BBC->SetValueAsObject(TEXT("AttackTarget"), PlayerCharacter);
        UE_LOG(LogTemp, Warning, TEXT("AttackTarget Set in Blackboard: %s"), *PlayerCharacter->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is NULL even after delay!"));
    }
}

void ASPTEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ASPTEnemyCharacter* EnemyCharacter = Cast<ASPTEnemyCharacter>(InPawn);
    if (EnemyCharacter && BehaviorTreeAsset)
    {
        if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BBC))
        {
            RunBehaviorTree(BehaviorTreeAsset);
            UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Started Successfully!"));
        }
    }
}