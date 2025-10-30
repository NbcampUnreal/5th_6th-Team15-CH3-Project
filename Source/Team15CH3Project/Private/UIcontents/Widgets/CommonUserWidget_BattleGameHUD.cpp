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
}