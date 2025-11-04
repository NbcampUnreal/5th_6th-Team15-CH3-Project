#include "UIcontents/Widgets/CommonUserWidget_BattleGameHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UCommonUserWidget_BattleGameHUD::InitHUD()
{
	// 캐릭터 가져오기 & 이걸 이젠 InitHUD를 HUD블루프린트로 연결 
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		MyPlayerChar = PlayerChar;
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

	//바인딩 값을 매 프레임마다 갱신
	if (HealthBar)
		HealthBar->SetPercent(MaxHP > 0.f ? CurrentHP / MaxHP : 0.f);

	if (HPText)
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));

	if (MPBar)
		MPBar->SetPercent(MaxMP > 0.f ? CurrentMP / MaxMP : 0.f);

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