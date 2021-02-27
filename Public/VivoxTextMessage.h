/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"

#include "VivoxCore.h"
#include "VivoxAccountId.h"

#include "VivoxTextMessage.generated.h"

UCLASS(BlueprintType)
class VIVOX_API UVivoxTextMessage : public UObject
{
public:
	GENERATED_BODY()

public:
	static UVivoxTextMessage *CreateVivoxTextMessage(ITextMessage *VivoxTextMessage);

	virtual void BeginDestroy() override;

	/**
	 * \brief The time that the message was received in UTC
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FDateTime ReceivedTime() const;

	/**
	 * \brief The message
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FString Message() const;

	/**
	 * \brief The sender
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FVivoxAccountId Sender() const;

	/**
	 * \brief The language of the Message. This conforms to RFC5646 (https://tools.ietf.org/html/rfc5646)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FString Language() const;

	/**
	 * \brief An optional name space for application specific data
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FString ApplicationStanzaNamespace() const;

	/**
	 * \brief Optional application specific data
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FString ApplicationStanzaBody() const;

	ITextMessage *VivoxTextMessage;
};

