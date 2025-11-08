#include "AI_Monster/MonsterAIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Monster/AIMonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

AMonsterAIController::AMonsterAIController()
{

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);

		StartBehaviorTree();
	}
	else
	{

	}

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(
			this,
			&AMonsterAIController::OnPerceptionUpdated
		);
	}

}

//void AMonsterAIController::OnPossess(APawn* InPawn)
//{
//	Super::OnPossess(InPawn);
//
//	if (InPawn)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Controller is controlling %s."), *InPawn->GetName());
//	}
//}


void AMonsterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Actor != PlayerPawn)
	{
		return;
	}

	if (!BlackboardComp)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), true);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);
	}
	else
	{
		
	}
}

UBlackboardComponent* AMonsterAIController::GetBlackboardComp() const
{
	return BlackboardComp; 
}

void AMonsterAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
		UE_LOG(LogTemp, Warning, TEXT("[Sparta] Behavior Tree started"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Sparta] Behavior Tree Asset not set!"));
	}
}
