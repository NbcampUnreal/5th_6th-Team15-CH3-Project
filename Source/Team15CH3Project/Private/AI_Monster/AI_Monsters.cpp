#include "AI_Monster/AI_Monsters.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_Monster/AI_MonsterController.h"
#include "AI_Monster/Ranged_MonsterController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "PlayerMade/PlayerCharacter.h"
#include "GameFramework/DamageType.h"
#include "Skill/SkillInventoryComponent.h"
#include "Components/CapsuleComponent.h" 
//추가
#include "Components/ProgressBar.h" 
#include "Blueprint/UserWidget.h" 
//끝


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

	RangeAttackRange = 850;
	AttackRange = 175.f;
	AttackCooldown = 1.5f;
	AttackDamage = 15.f;
	LastAttackTime = -1000.0f;
	//HP 추가
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());
	HealthBarComp->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f)); // Z축 위치는 예시입니다.
	HealthBarComp->SetDrawSize(FVector2D(150.0f, 20.0f));
	//HP 추가 끝
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
	//추가
	UpdateOverheadHP();
	//끝
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

bool AAI_Monsters::ReangCanAttak(APawn* Target) const
{
	if (!Target || IsDead()) return false;
	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	if (Now - LastAttackTime < AttackCooldown) return false;
	const float Dist = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
	return Dist <= RangeAttackRange;
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

void AAI_Monsters::BulletAttack(APawn* Target)
{
	if (!Target || IsDead()) return;

	if (Bullets)
	{
		FVector MuzzleLocation = GetMesh()->GetSocketLocation("FireMuzzle");

		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector TargetLoc = Target->GetActorLocation() + FVector(0, 0, 50.0f); // 약간 상체 쪽을 향하게
			FVector Direction = (TargetLoc - MuzzleLocation).GetSafeNormal();
			FRotator MuzzleRotation = Direction.Rotation();

			AMonsterBullet* bullet = world->SpawnActor<AMonsterBullet>(Bullets, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (bullet)
			{
				bullet->FireInDirection(Direction);
			}
		}

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
}

float AAI_Monsters::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsDead()) return 0.f;

	CurrentHP = FMath::Clamp(CurrentHP - Applied, 0.f, MaxHP);
	UE_LOG(LogTemp, Log, TEXT("[Monster] -%.1f HP -> %.1f / %.1f"), Applied, CurrentHP, MaxHP);
	//추가
	UpdateOverheadHP();
	//끝

	if (IsDead())
	{

		/*if (UWorld* World = GetWorld())
		{
			if (APlayerCharacter* PC = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0)))
			{
				PC->AddExp(ExpReward);
				UE_LOG(LogTemp, Warning, TEXT("[Monster] Player0에게 경험치 %d 지급"), ExpReward);
			}
		}*/

		HandleDeath();
		SetLifeSpan(3.0f);

		///// 캐릭터 흡혈 패시브 스킬
		if (DamageCauser)
		{
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(DamageCauser))
			{
				if (USkillInventoryComponent* SkillInv = Player->FindComponentByClass<USkillInventoryComponent>())
				{
					if (SkillInv->HasPassiveSkill(EPassiveItemType::BloodAbsorbing))
					{
						if (UCharacterStatsComponent* Stats = Player->FindComponentByClass<UCharacterStatsComponent>())
						{
							const float HealAmount = Stats->MaxHP * 0.05f;
							Stats->CurrentHP = FMath::Clamp(Stats->CurrentHP + HealAmount, 0.f, Stats->MaxHP);
						}
					}
				}
			}
		}
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
//추가 
void AAI_Monsters::UpdateOverheadHP()
{
	// 위젯 컴포넌트 유효성 검사
	if (!HealthBarComp) return;

	// 위젯 인스턴스를 가져올 수 있는지 확인 (GetUserWidgetObject는 위젯이 런타임에 생성될 때만 유효합니다)
	if (UUserWidget* WidgetInstance = HealthBarComp->GetUserWidgetObject())
	{
		// 이름이 "HealthBar"인 UProgressBar 찾기
		if (UProgressBar* HPBar = Cast<UProgressBar>(WidgetInstance->GetWidgetFromName(TEXT("Monster_HP"))))
		{
			// HP 비율 계산
			const float HPPercent = (MaxHP > 0.f) ? CurrentHP / MaxHP : 0.f;
			HPBar->SetPercent(HPPercent);

			// HP가 낮을 때 색상 변경 필요없을 듯 
			if (HPPercent < 0.3f)
			{
				HPBar->SetFillColorAndOpacity(FLinearColor::Red);
			}
		}
	}
}
//끝