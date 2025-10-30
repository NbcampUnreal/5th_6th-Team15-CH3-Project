#include "AI_Monster/AI_Monsters.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_Monster/AI_MonsterController.h"
#include "AI_Monster/Ranged_MonsterController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "PlayerMade/PlayerCharacter.h"
#include "GameFramework/DamageType.h"
#include "Components/CapsuleComponent.h" 



AAI_Monsters::AAI_Monsters()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAI_MonsterController::StaticClass();
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true; //키면 캐릭터가 움직이는 방향으로 바라보고 끄면 미끌어 지듯이 바라본다
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //돌아 보는 회전 속도 높으면 갑자기 확 돌아본다
	Movement->MaxWalkSpeed = WalkSpeed;
	
	MaxHP = 300.0f;
	CurrentHP = MaxHP;

	AttackRange = 175.f;
	AttackCooldown = 1.5f;
	AttackDamage = 15.f;
	LastAttackTime = -1000.0f;
}


void AAI_Monsters::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Character has been spawned."));

	SetMovementSpeed(WalkSpeed);

	if (AAI_MonsterController* MeleeCtrl = Cast<AAI_MonsterController>(GetController()))
	{
		MeleeCtrl->StartChaseLoop();
		return;
	}

	if (ARanged_MonsterController* RangedCtrl = Cast<ARanged_MonsterController>(GetController()))
	{
		RangedCtrl->StartChaseLoop();
		return;
	}
}

void AAI_Monsters::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = NewSpeed;
	}
}

bool AAI_Monsters::CanAttack(APawn* Target) const
{
	if (!Target || IsDead()) return false;
	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	if (Now - LastAttackTime < AttackCooldown) return false;
	const float Dist = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
	return Dist <= AttackRange;
	//UE_LOG(LogTemp, Warning, TEXT("[Attack!!]"));
}

void AAI_Monsters::PerformAttack(APawn* Target)
{
	if (!Target || IsDead()) return;

	LastAttackTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

	UGameplayStatics::ApplyDamage(
		Target,
		AttackDamage,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	UE_LOG(LogTemp, Warning, TEXT("[Monster] Attack! Target=%s, Damage=%.1f, Dist=%.0f"), *Target->GetName(), AttackDamage, FVector::Dist2D(GetActorLocation(), Target->GetActorLocation()));


}

float AAI_Monsters::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsDead()) return 0.f;

	CurrentHP = FMath::Clamp(CurrentHP - Applied, 0.f, MaxHP);
	UE_LOG(LogTemp, Log, TEXT("[Monster] -%.1f HP -> %.1f / %.1f"), Applied, CurrentHP, MaxHP);

	if (IsDead())
	{
		// 캐릭터 스테이터스에 접근해서 경험치를 제공하는 기능 추가해야함
		HandleDeath();
		SetLifeSpan(3.0f);
	}
	return Applied;
}

void AAI_Monsters::HandleDeath()
{
	DetachFromControllerPendingDestroy();

	if (UCapsuleComponent* Cap = GetCapsuleComponent())
	{
		Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

