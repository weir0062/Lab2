// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PController.generated.h"

/**
 * 
 */
UCLASS()
class LAB2_API APController : public APlayerController
{
	GENERATED_BODY()

public:
		virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMyUserWidget* GameInfoWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UMyUserWidget> WidgetBP;
	
};
