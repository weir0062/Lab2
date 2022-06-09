// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Lab2Character.h"

bool UMyUserWidget::Initialize()
{
    bool result = Super::Initialize();
    return result;


    UTextBlock* WidgetText = Cast<UTextBlock>(GetWidgetFromName("TextBlock69"));

    if (WidgetText)
    {
        WidgetText->SetText(FText::FromString("Poop"));
    }
}
void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
DisplayNumGrenades();
}

void UMyUserWidget::DisplayNumGrenades()
{
    AController* pController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APawn* playerPawn = pController->GetPawn();
    ALab2Character* player = Cast<ALab2Character>(playerPawn);

    UTextBlock* WidgetText = Cast<UTextBlock>(GetWidgetFromName("TextBlock69"));
    FString value = FString::Printf(TEXT("Grenades : %d"), player->NumGrenades);


        if (WidgetText)
        {
            WidgetText->SetText(FText::FromString(value));
        }

}
