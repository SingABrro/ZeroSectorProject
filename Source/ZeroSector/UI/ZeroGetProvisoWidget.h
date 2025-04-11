// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroGetProvisoWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroGetProvisoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(Category = "UI")
    void ShowWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ProvisoNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ProvisoDescriptionText;

	// �ܼ� ������ ������ �����ϴ� �Լ�
	void SetProvisoInfo(const FString& ProvisoName, const FString& ProvisoDescription);

	UFUNCTION(BlueprintCallable)
	void SetProvisoImage(UTexture2D* InImage);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ProvisoImage;


private:
    FTimerHandle WidgetTimerHandle;
};

