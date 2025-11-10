#include "Skill/SkillUseIndicatorComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "PlayerMade/Controller/PlayerMadeController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

USkillUseIndicatorComponent::USkillUseIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsVisible = false; // 테스트용으로 일단 바로 키게할꺼
}

void USkillUseIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IndicatorActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnedIndicatorActor = GetWorld()->SpawnActor<AActor>(
			IndicatorActorClass,
			GetOwner()->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (SpawnedIndicatorActor)
		{
			HideIndicator();
		}
	}
}

void USkillUseIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsVisible && SpawnedIndicatorActor)
	{
		UpdateIndicatorLocation();
	}
}

void USkillUseIndicatorComponent::ShowIndicator()
{
	if (SpawnedIndicatorActor)
	{
		SpawnedIndicatorActor->SetActorHiddenInGame(false);
		bIsVisible = true;
	}
}

void USkillUseIndicatorComponent::HideIndicator()
{
	if (SpawnedIndicatorActor)
	{
		SpawnedIndicatorActor->SetActorHiddenInGame(true);
		bIsVisible = false;
	}
}

void USkillUseIndicatorComponent::UpdateIndicatorLocation()
{
	if (!bIsVisible || !SpawnedIndicatorActor) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PlayerController) return;

	FVector MouseWorldOrigin, MouseWorldDirection;
	if (!PlayerController->DeprojectMousePositionToWorld(MouseWorldOrigin, MouseWorldDirection))
		return;

	float GroundZ = OwnerPawn->GetActorLocation().Z - 5.0f; // 상황에 맞게 조정

	const float EPS = KINDA_SMALL_NUMBER;
	FVector approxXYPoint; 
	bool bHavePlaneIntersection = false;

	if (FMath::Abs(MouseWorldDirection.Z) > EPS)
	{
		float t = (GroundZ - MouseWorldOrigin.Z) / MouseWorldDirection.Z;
		// t가 음수면 카메라 뒤로 가는 점
		if (t > 0.f)
		{
			FVector planePoint = MouseWorldOrigin + MouseWorldDirection * t;
			approxXYPoint = FVector(planePoint.X, planePoint.Y, GroundZ);
			bHavePlaneIntersection = true;
		}
	}

	if (!bHavePlaneIntersection)// 일정 거리만큼 전방을 사용해서 XY 얻기
	{
		const float FallbackDist = 3000.f;
		FVector fallbackPoint = MouseWorldOrigin + MouseWorldDirection * FallbackDist;
		approxXYPoint = FVector(fallbackPoint.X, fallbackPoint.Y, GroundZ);
	}

	FVector TraceStart = FVector(approxXYPoint.X, approxXYPoint.Y, GroundZ + 2000.f); 
	FVector TraceEnd = FVector(approxXYPoint.X, approxXYPoint.Y, GroundZ - 2000.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	if (SpawnedIndicatorActor)
		Params.AddIgnoredActor(SpawnedIndicatorActor);

	
	TArray<UPrimitiveComponent*> OwnerComps;// 또한 소유자와 스폰된 인디케이터의 모든 프리미티브 컴포넌트 제외 (자기충돌 완전 차단)
	GetOwner()->GetComponents<UPrimitiveComponent>(OwnerComps);
	for (UPrimitiveComponent* Comp : OwnerComps)
	{
		if (Comp) Params.AddIgnoredComponent(Comp);
	}

	if (SpawnedIndicatorActor)
	{
		TArray<UPrimitiveComponent*> IndComps;
		SpawnedIndicatorActor->GetComponents<UPrimitiveComponent>(IndComps);
		for (UPrimitiveComponent* Comp : IndComps)
			if (Comp) Params.AddIgnoredComponent(Comp);
	}

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		FVector NewLocation = Hit.Location;
		NewLocation.Z += 2.5f; // 살짝 띄움
		SpawnedIndicatorActor->SetActorLocation(NewLocation);
	}
	else // 아무 것도 맞지 않으면 fallback 위치
	{
		FVector FallbackLocation = FVector(approxXYPoint.X, approxXYPoint.Y, GroundZ + 200.f);
		SpawnedIndicatorActor->SetActorLocation(FallbackLocation);
	}
}