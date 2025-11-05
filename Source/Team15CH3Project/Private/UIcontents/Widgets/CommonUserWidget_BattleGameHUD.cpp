#include "UIcontents/Widgets/CommonUserWidget_BattleGameHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UIcontents/Widgets/CommonUserWidget_Skill.h"

void UCommonUserWidget_BattleGameHUD::InitHUD()
{
	//캐릭터 가져오기 & 이걸 이젠 InitHUD를 HUD블루프린트로 연결 
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		MyPlayerChar = PlayerChar;

		if (UCharacterStatsComponent* StatsComp = PlayerChar->FindComponentByClass<UCharacterStatsComponent>())
		{
			LastKnownLevel = StatsComp->Level; //시작 시점 레벨 저장
			UE_LOG(LogTemp, Warning, TEXT("[HUD] InitHUD - Starting Level: %d"), LastKnownLevel);
		}
	}
}

void UCommonUserWidget_BattleGameHUD::ShowSkillSelectUI(UCharacterStatsComponent* StatsComp)
{

	// 위젯 생성
	UCommonUserWidget_Skill* SkillSelectUI = CreateWidget<UCommonUserWidget_Skill>(GetWorld(), SkillSelectWidgetClass);

	//정상 생성
	UE_LOG(LogTemp, Warning, TEXT("[HUD] SkillSelectUI successfully created!"));
	SkillSelectUI->AddToViewport(100);
	bIsSkillSelectUIShown = true;

	//게임 일시정지 및 입력 전환
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetPause(true);
		PC->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(SkillSelectUI->TakeWidget());
		PC->SetInputMode(InputMode);
	}
}


void UCommonUserWidget_BattleGameHUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (!MyPlayerChar) return;

	//UCharacterStatsComponent에서 스테이트 가져오기
	UCharacterStatsComponent* StatsComp = MyPlayerChar->FindComponentByClass<UCharacterStatsComponent>();
	if (!StatsComp) return;

	float CurrentHP = StatsComp->CurrentHP; 
	float MaxHP = StatsComp->MaxHP; 

	float CurrentMP = StatsComp->CurrentMP;
	float MaxMP = StatsComp->MaxMP;

	int32 Level = StatsComp->Level;

	int32 CurrentXP = StatsComp->Experience;
	int32 MaxXp = StatsComp->MaxExperience;

	if (Level != LastKnownLevel && Level > 0)
	{
		if (Level > LastKnownLevel)
		{
			UE_LOG(LogTemp, Warning, TEXT("Level Up! %d -> %d"), LastKnownLevel, Level);
			ShowSkillSelectUI(StatsComp);
		}

		LastKnownLevel = Level; // 감지 후에 갱신
	}

	if (XPBar != nullptr)
	{
		const float TargetXpPercent = (MaxXp > 0) ? (static_cast<float>(CurrentXP) / MaxXp) : 0.0f;

		DisplayedXpPercent = FMath::FInterpTo(
			DisplayedXpPercent,            
			TargetXpPercent,               
			DeltaTime,                     
			XpLerpSpeed                    
		);

		XPBar->SetPercent(DisplayedXpPercent);
	}

	if (XPText)
	{
		FString LevelString = FString::Printf(TEXT("Lv.%d"), Level);
		XPText->SetText(FText::FromString(LevelString));
	}

	//바인딩 값을 매 프레임마다 갱신
	const float TargetHPPercent = (MaxHP > 0.f) ? (CurrentHP / MaxHP) : 0.f;

	if (HealthBar != nullptr)
	{
		//FInterpTo를 사용하여 부드럽게 보간, DisplayedHealthPercent를 TargetPercent를 향해 천천히 이동시킵니다.
		DisplayedHealthPercent = FMath::FInterpTo(
			DisplayedHealthPercent,        // 현재 표시 값 (시작)
			TargetHPPercent,                 // 실제 HP 퍼센트 값 (목표)
			DeltaTime,                     // GetWorld()->GetDeltaSeconds()와 동일 (프레임 시간)
			HealthDecreaseSpeed            // 보간 속도 (헤더에서 정의된 변수)
		);

		//HP 바 위젯 업데이트, 보간된 DisplayedHealthPercent 값을 위젯에 적용합니다.
		HealthBar->SetPercent(DisplayedHealthPercent);
	}

	if (HPText)
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));

	const float TargetMPPercent = (MaxMP > 0.f) ? (CurrentMP / MaxMP) : 0.f;

	if (MPBar != nullptr)
	{
		DisplayedMPPercent = FMath::FInterpTo(
			DisplayedMPPercent,    
			TargetMPPercent,
			DeltaTime,                     
			MPDecreaseSpeed
		);

		MPBar->SetPercent(DisplayedMPPercent);
	}

	if (MPText)
		MPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMP, MaxMP)));

	if (CurrentHP > 0.0f)
	{
		//경과 시간 누적
		GameTimeElapsed += DeltaTime;

		//시간 업데이트
		if (CurrentTime)
		{
			int32 TotalSeconds = FMath::FloorToInt(GameTimeElapsed);
			int32 Minutes = TotalSeconds / 60;
			int32 Seconds = TotalSeconds % 60;

			//"00:00" 형태로 포맷팅합니다.
			FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
			CurrentTime->SetText(FText::FromString(TimeString));
		}
	}
	else//(CurrentHP <= 0.0f 일 때)
	{
		if (bIsDefeatUIShown)
		{
			return;
		}

		if (DefeatWidgetClass)
		{
			//순수 블루프린트 위젯이므로 UUserWidget으로 생성합니다.
			UUserWidget* DefeatUI = CreateWidget<UUserWidget>(GetWorld(), DefeatWidgetClass);

			if (DefeatUI)
			{
				//랜덤 문구 선택 및 함수 호출
				FString QuoteToDisplay = FString();
				if (DefeatQuotes.Num() > 0)
				{
					int32 RandomIndex = FMath::RandRange(0, DefeatQuotes.Num() - 1);
					QuoteToDisplay = DefeatQuotes[RandomIndex];
				}

				FString FunctionCallString = FString::Printf(TEXT("%s %s"), TEXT("SetDefeatQuoteBP"), *QuoteToDisplay);

				DefeatUI->CallFunctionByNameWithArguments(*FunctionCallString, *GLog, nullptr, true);

				DefeatUI->AddToViewport();
				bIsDefeatUIShown = true;
			}
		}
		return;
	}
}
