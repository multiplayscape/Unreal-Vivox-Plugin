/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"

#include "VivoxCore.h"
#include "VivoxTextMessage.h"

#include "VivoxChannelTextMessage.generated.h"

class UVivoxChannelSession;

UCLASS(BlueprintType)
class VIVOX_API UVivoxChannelTextMessage : public UVivoxTextMessage
{
public:
    GENERATED_BODY()

public:
    static UVivoxChannelTextMessage *CreateVivoxChannelTextMessage(IChannelTextMessage *VivoxChannelTextMessage);

	virtual void BeginDestroy() override;

    /**
     * \brief The ChannelSession in which this text message appeared.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    UVivoxChannelSession * ChannelSession() const;

    IChannelTextMessage *VivoxChannelTextMessage;
};

