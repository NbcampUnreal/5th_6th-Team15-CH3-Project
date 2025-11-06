#include "Skill/ActiveSkillItem.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Skill/SkillUseIndicatorComponent.h"
#include "skill/Actor/PowerSurgeBullet.h"
#include "Skill/Actor/ShootingLaser.h"
#include "Skill/Actor/NuclearAttack.h"
#include "Skill/Actor/NapalmAttack.h"

AActiveSkillItem::AActiveSkillItem()
{
	ActiveSkillData.StackCnt = 0;
	ActiveType = EActiveSkillItemType::None;
}

void AActiveSkillItem::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		ActiveSkillApply(Player);
	}
}

void AActiveSkillItem::ActiveSkillApply(class APlayerCharacter* Target)
{

	if (!Target) return;

	FActorSpawnParameters Params;
	Params.Owner = Target;
	Params.Instigator = Target;

	switch (ActiveType)
	{
	case EActiveSkillItemType::ShootingGun:
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveSkillApply: ShootingGun"));
		GetWorld()->SpawnActor<APowerSurgeBullet>(APowerSurgeBullet::StaticClass(), Target->GetActorLocation(), Target->GetActorRotation(), Params);
	}
		break;
	case EActiveSkillItemType::CircleNearbySword:

		break;
	case EActiveSkillItemType::ShootingLaser:
		GetWorld()->SpawnActor<AShootingLaser>(AShootingLaser::StaticClass(), Target->GetActorLocation(), Target->GetActorRotation(), Params);
		break;
	case EActiveSkillItemType::NapalmAttack:
		GetWorld()->SpawnActor<ANapalmAttack>(ANapalmAttack::StaticClass(), Target->GetActorLocation(), Target->GetActorRotation(), Params);
		break;
	case EActiveSkillItemType::NuclearAttack:
		GetWorld()->SpawnActor<ANuclearAttack>(ANuclearAttack::StaticClass(), Target->GetActorLocation(), Target->GetActorRotation(), Params);
		break;
	default:
		break;
	}
}
