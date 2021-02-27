/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"

#include "VivoxCore.h"
#include "ITranscribedMessage.h"

#include "VivoxTranscribedMessage.generated.h"

class UVivoxChannelSession;

UCLASS(BlueprintType)
class VIVOX_API UVivoxTranscribedMessage : public UObject
{
public:
    GENERATED_BODY()

public:
    static UVivoxTranscribedMessage *CreateVivoxTranscribedMessage(ITranscribedMessage *VivoxTranscribedMessage);

	virtual void BeginDestroy() override;

	/**
	 * \brief The time that the transcription was received in UTC
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FDateTime ReceivedTime() const;

	/**
	 * \brief The transcribed text
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	const FString& Text() const;

	/**
	 * \brief The speaker
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	FVivoxAccountId Speaker() const;

	/**
	 * \brief The language of the transcribed Text. This conforms to BCP-47 (https://tools.ietf.org/html/bcp47)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	const FString& Language() const;
	/**
	 * \brief The ChannelSession in which this message was originally spoken
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	UVivoxChannelSession *ChannelSession() const;

	ITranscribedMessage *VivoxTranscribedMessage;
};

