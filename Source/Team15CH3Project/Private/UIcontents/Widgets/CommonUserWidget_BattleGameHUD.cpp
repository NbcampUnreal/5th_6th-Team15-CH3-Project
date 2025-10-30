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
	// ĳ���� �������� & �̰� ���� InitHUD�� HUD�������Ʈ�� ���� 
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		MyPlayerChar = PlayerChar;
	}
}


void UCommonUserWidget_BattleGameHUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (!MyPlayerChar) return;


	//UCharacterStatsComponent���� ������Ʈ ��������
	UCharacterStatsComponent* StatsComp = MyPlayerChar->FindComponentByClass<UCharacterStatsComponent>();
	if (!StatsComp) return;

	float CurrentHP = StatsComp->CurrentHP; 
	float MaxHP = StatsComp->MaxHP; 

	float CurrentMP = StatsComp->CurrentMP;
	float MaxMP = StatsComp->MaxMP;

	//���ε� ���� �� �����Ӹ��� ����
	if (HealthBar)
		HealthBar->SetPercent(MaxHP > 0.f ? CurrentHP / MaxHP : 0.f);

	if (HPText)
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));

	if (MPBar)
		MPBar->SetPercent(MaxMP > 0.f ? CurrentMP / MaxMP : 0.f);

	if (MPText)
		MPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMP, MaxMP)));
}