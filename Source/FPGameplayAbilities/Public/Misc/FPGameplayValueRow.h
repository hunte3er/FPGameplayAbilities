﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPGameplayValueRow.generated.h"

class UGameplayAbility;
struct FGameplayEffectSpecHandle;
struct FGameplayTagContainer;
class UAbilitySystemComponent;
struct FGameplayTag;

UCLASS(BlueprintType, Abstract)
class FPGAMEPLAYABILITIES_API UFPValueCalculation : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual float Calculate(UAbilitySystemComponent* ASC, float BaseValue, const FGameplayTagContainer& Tags) { return BaseValue; }
};

UCLASS(BlueprintType, Abstract)
class FPGAMEPLAYABILITIES_API UFPValueDisplay : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FString GetDisplayString(float Value) { return FString::SanitizeFloat(Value, 0); }
};

UCLASS(BlueprintType)
class FPGAMEPLAYABILITIES_API UFPValueSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UFPValueCalculation> ValueCalculation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseCalculationOnlyForDisplayValue = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UFPValueDisplay> ValueDisplayMethod;
};

USTRUCT(BlueprintType)
struct FPGAMEPLAYABILITIES_API FFPGameplayValueRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Value = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFPValueSettings* Settings;

	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// TSubclassOf<UFPValueCalculation> ValueCalculation;
	//
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// TSubclassOf<UFPValueDisplay> ValueDisplayMethod;
	//
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// bool bValueCalculationOnlyForDisplayValue = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayTag> Tags;
};

UCLASS(BlueprintType, DefaultToInstanced)
class FPGAMEPLAYABILITIES_API UFPGameplayValueHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool GetBaseValueFromTable(UDataTable* DataTable, FGameplayTag Tag, float& Value);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool GetTransformedValueFromTable(UDataTable* DataTable, UAbilitySystemComponent* ASC, FGameplayTag Tag, FGameplayTagContainer EffectTags, float& OutTransformedValue);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool GetDisplayValueFromTable(UDataTable* DataTable, FGameplayTag Tag, FString& OutString);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool GetTransformedDisplayValueFromTable(UDataTable* DataTable, FGameplayTag Tag, UAbilitySystemComponent* ASC, FGameplayTagContainer EffectTags, FString& OutString);

	UFUNCTION(BlueprintCallable)
	static void ApplyGameValueTableToSpec(UAbilitySystemComponent* ASC, FGameplayEffectSpecHandle SpecHandle, UDataTable* DataTable);

	UFUNCTION(BlueprintCallable)
	static FGameplayTagContainer GatherTagsFromGameplayAbility(UGameplayAbility* GameplayAbility, FGameplayTag GameValueTag, UDataTable* DataTable);

	UFUNCTION(BlueprintCallable)
	static FString ApplyValueDisplayMethod(TSubclassOf<UFPValueDisplay> ValueDisplayMethod, float Value);

	UFUNCTION(BlueprintCallable)
	static bool DoesGameValueTableHaveTag(FGameplayTag GameValueTag, UDataTable* DataTable);
};