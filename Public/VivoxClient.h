/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"

#include "VivoxCore.h"

#include "VivoxStatus.h"
#include "VivoxAccountId.h"
#include "VivoxLoginSession.h"
#include "VivoxClientConfig.h"

#include "VivoxClient.generated.h"

UCLASS(BlueprintType)
class VIVOX_API UVivoxClient : public UObject
{
public:
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="vivox")
    static UVivoxClient* GetVivoxClient();

	virtual void BeginDestroy() override;

    /**
     * \brief Initializes this Client instance.
     * If the client is already initialized, it will do nothing but return VxErrorAlreadyInitialized.
     * \param logLevel -1 = no logging, 0 = errors only, 1 = warnings, 2 = info, 3 = debug, 4 = trace
     * \return VivoxErrorSuccess under most circumstances. Failures to initialize are rare.
     */
    UFUNCTION(BlueprintCallable, Category="vivox")
    EVivoxStatus Initialize(const FVivoxClientConfig &clientConfig);

    /**
     * \brief Uninitialize this Client instance. If this Client instance is not initialized, it will do nothing.
     */
    UFUNCTION(BlueprintCallable, Category="vivox")
    void Uninitialize();

    /**
    * \brief The internal version the low level vivoxsdk library
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="vivox")
    static FString GetSDKVersion();

	/**
	 * \brief The internal version of the low level vivoxsdk library with extended info
	 */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="vivox")
    static FString GetSDKVersionExtended();

    /**
     * \brief Gets the LoginSession object for the provided accountId, creating one if necessary
     * \param loginSessionId the AccountId for this login session
     * \return the login session for that accountId
     * \remarks If a new LoginSession is created, LoginSessions.AfterKeyAdded will be raised.</remarks>
     */
    UFUNCTION(BlueprintCallable, Category="vivox")
    UVivoxLoginSession* GetLoginSession(const FVivoxAccountId &accountId);

    /**
    * \brief All the Login Sessions associated with this Client instance.
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="vivox")
    TMap<FVivoxAccountId, UVivoxLoginSession * > LoginSessions();

    /**
     * \brief The Audio Input Devices associated with this Client instance
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="vivox")
    UVivoxAudioDevices *AudioInputDevices();

    /**
     * \brief The Audio Output Devices associated with this Client instance
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="vivox")
    UVivoxAudioDevices *AudioOutputDevices();

    IClient *VivoxVoiceClient;
};

