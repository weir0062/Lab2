// Fill out your copyright notice in the Description page of Project Settings.


#include "PController.h"
#include "MyUserWidget.h"
void APController::BeginPlay()
{
    Super::BeginPlay();


    if (WidgetBP)
    {
        GameInfoWidget = CreateWidget<UMyUserWidget>(this, WidgetBP);
        GameInfoWidget->AddToViewport();
    }
}
