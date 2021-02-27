/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"
#include "VivoxCore.h"

#include "VivoxParticipantProperties.generated.h"

UCLASS(BlueprintType)
class VIVOX_API UVivoxParticipantProperties : public UObject
{
public:
    GENERATED_BODY()

public:
    static UVivoxParticipantProperties *CreateVivoxParticipantProperties(IParticipantProperties *VivoxParticipantProperties);

	virtual void BeginDestroy() override;

    /**
     * \brief True if this participant corresponds to the currently connected user.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool IsSelf() const;

    /**
    * \brief If true, the user is in audio
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool InAudio() const;

    /**
    * \brief If true, the user is in text
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool InText() const;

    /**
    * \brief If true, the user is speaking
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool SpeechDetected() const;

    /**
    * \brief The audio energy for this user. This can be used to build a VU meter for this user.
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    float AudioEnergy() const;

    /**
     * \brief The gain for this user, only for the currently logged in user.
     *  The valid range is between -50 and 50.
     *  Positive values increase volume, negative values decrease volume.
     *  Zero (default) leaves volume unchanged.
     */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    int LocalVolumeAdjustment() const;

    /**
     * \brief The gain for this user, only for the currently logged in user.
     *  The valid range is between -50 and 50.
     *  Positive values increase volume, negative values decrease volume.
     *  Zero (default) leaves volume unchanged.
     */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    EVivoxStatus SetLocalVolumeAdjustment(int value);

    /**
     * \brief Returns the muted status of the participant for the currently connected user only.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool LocalMute() const;

    /**
    * \brief Use this to silence or un-silence this participant for the currently connected user only.
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    void SetLocalMute(bool value);

    /**
     * \brief Indicates if the user is currently typing.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool IsTyping() const;

    /**
     * \brief Indicates if the user has been muted for all users.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool IsMutedForAll() const;

    /**
    * \brief Use this to silence or un-silence this participant for everyone in the channel.
    *
    * \param setMuted true to mute or false to unmute
    * \param accessToken the access token granting the user permission to mute this participant in the channel
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidArgument if accessToken is empty
    * \remarks Developers of games that do not have secure communications requirements can use GetMuteForAllToken() to generate the required access token.
    */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
              Category = "vivox"
    )
    void BeginSetIsMutedForAll(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            bool setMuted,
            const FString& accessToken,
            EVivoxStatus &status
    );

    /**
    * \brief Get a token that can be used to silence or un-silence a participant for everyone in a channel.
    *
    * \param tokenSigningKey the key corresponding to the issuer for this account that is used to sign the token.
    * \param tokenExpirationDuration the length of time the token is valid for.
    * \return A token that can be used to mute a participant in a channel.
    * \remarks To be used only by applications without secure communications requirements.
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    FString GetMuteForAllToken(const FString & tokenSigningKey, FTimespan tokenExpirationDuration) const;

    IParticipantProperties *VivoxParticipantProperties;
};