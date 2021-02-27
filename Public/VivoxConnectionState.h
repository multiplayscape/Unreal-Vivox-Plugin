/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "VivoxConnectionState.generated.h"

/**
 * \brief Represents the state of any resource with connection semantics
 */
UENUM(BlueprintType)
enum class EVivoxConnectionState : uint8
{
    /**
     *\brief The resource is disconnected
     */
            Disconnected	UMETA(DisplayName = "disconnected"),
    /**
     * \brief The resource is in the process of connecting
     */
            Connecting		UMETA(DisplayName = "connecting"),
    /**
     * \brief The resource is connected
     */
            Connected		UMETA(DisplayName = "connected"),
    /**
     * \brief The resource is in the process of disconnecting
     */
            Disconnecting	UMETA(DisplayName = "disconnecting"),
};