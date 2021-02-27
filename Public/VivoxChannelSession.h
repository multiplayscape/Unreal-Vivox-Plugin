/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"
#include "Delegates/IDelegateInstance.h"

#include "VivoxCore.h"

#include "VivoxStatus.h"
#include "VivoxChannelId.h"
#include "VivoxConnectionState.h"
#include "VivoxChannelTextMessage.h"
#include "VivoxTranscribedMessage.h"

#include "VivoxChannelSession.generated.h"

class UVivoxLoginSession;

UENUM(BlueprintType)
enum class EVivoxTransmitPolicy : uint8 {
    No, // Do not transmit
    Yes // Transmit
};

UCLASS(BlueprintType)
class VIVOX_API UVivoxChannelSession : public UObject {
public:
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAudioStateChanged, const UVivoxChannelConnectionState *, vivoxChannelConnectionState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextStateChanged, const UVivoxChannelConnectionState *, vivoxChannelConnectionState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterParticipantAdded, const UVivoxParticipant *, vivoxParticipant);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeParticipantRemoved, const UVivoxParticipant *, vivoxParticipant);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterParticipantUpdated, const UVivoxParticipant *, vivoxParticipant);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextMessageReceived, const UVivoxChannelTextMessage *, vivoxChannelTextMessage);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTranscribedMessageReceived, const UVivoxTranscribedMessage*, vivoxTranscribedMessage);

public:
    static UVivoxChannelSession *CreateVivoxChannelSession(IChannelSession *VivoxVoiceChannelSession);

    /**
     * \brief This event is fired when the audio state changes
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FAudioStateChanged AudioStateChanged;

    /**
     * \brief This event is fired when the text state changes
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FTextStateChanged TextStateChanged;

    /**
     * \brief This event is raised after a participant has been added to the Participants() collection
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FAfterParticipantAdded AfterParticipantAdded;

    /**
     * \brief This event is raised before a participant will be removed from the Participants() collection
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FBeforeParticipantRemoved BeforeParticipantRemoved;

    /**
     * \brief This event is raised after a property of the participant has changed
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FAfterParticipantUpdated AfterParticipantUpdated;

    /**
     * \brief An event indicating that another player has sent the channel a text message.
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FTextMessageReceived TextMessageReceived;

    /**
     * \brief An event indicating that another player has sent the channel a text message.
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
	FTranscribedMessageReceived TranscribedMessageReceived;

    FDelegateHandle AudioStateChangedHandle;
    FDelegateHandle TextStateChangedHandle;
    FDelegateHandle AfterParticipantAddedHandle;
    FDelegateHandle BeforeParticipantRemovedHandle;
    FDelegateHandle AfterParticipantUpdatedHandle;
    FDelegateHandle TextMessageReceivedHandle;
    FDelegateHandle TranscribedMessageReceivedHandle;

	virtual void BeginDestroy() override;

    /**
     * \brief The login session that owns this channel session
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    UVivoxLoginSession * Parent();

    /**
     * \brief The state of the audio portion of this channel session.
     * \remarks Changes to this value may occur at anytime due to network or moderator events.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    EVivoxConnectionState AudioState() const;

    /**
     * \brief The state of the text portion of this channel session
     * \remarks Changes to this value may occur at anytime due to network or moderator events.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    EVivoxConnectionState TextState() const;

    /**
     * \brief The list of participants in this channel, including the current user.
     * \remarks Changes to this list may occur at anytime. Key equals the participant's AccountId::Name().
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    TMap<FString, UVivoxParticipant *> Participants() const;

    /**
     * \brief Whether this user is typing.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool Typing() const;

    /**
     * \brief Set whether this user is typing. Setting or clearing this will cause IParticipantProperties.IsTyping to change for other users in the channel.
     */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    void SetTyping(bool value);

	/**
	 * \brief Indicates that microphone audio and injected audio will transmit to this session.
	 * @see ILoginSession::SetTransmissionMode()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	bool IsTransmitting() const;

    /**
     * \brief The channel id of this session
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    FVivoxChannelId Channel() const ;

	/**
	 * Perform the initial connection to the channel
	 *
	 * \param connectAudio true to connect audio
	 * \param connectText true to connect text
	 * \param switchTransmission transmit into only this channel once connected. This overrides and changes the TransmissionMode set in ILoginSession.
	 * \param accessToken the access token granting the user access to the channel
	 * \param theDelegate a delegate to call when this operation completes
	 * \return VxErrorInvalidArgument if:
	 * - accessToken is empty
	 * - connectAudio and connectText are both false
	 * - Channel().IsValid() is false
	 * \remarks Developers of games that do not have secure communications requirements can use GetConnectToken() to generate the required access token.
	 */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
              Category = "vivox"
    )
    void BeginConnect(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            bool connectAudio,
            bool connectText,
			bool switchTransmission,
            const FString &accessToken,
            EVivoxStatus &status
    );

    /**
     * \brief Disconnect the user from this channel.
     *
     * \remarks The AudioState and TextState properties will not be set to ConnectionState.Disconnected until it is OK to re-join this channel.
     *  The Application must monitor property changes for these properties in order to determine when it's ok rejoin the channel. This object remains in the
     *  ILoginSession.ChannelSessions list. Use ILoginSession.DeleteChannelSession to remove it from the list.
     */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    void Disconnect();

	/**
	 * \brief Add or remove audio from the channel session
	 *
	 * \param value true to add audio, false to remove audio
	 * \param switchTransmission transmit into only this channel once audio is added. This overrides and changes the TransmissionMode set in ILoginSession.
	 * \param callback a delegate to call when this operation completes
	 * \remarks if transmission is set to this channel specifically, TransmissionMode will be changed to `None` when audio is removed.
	 * \return 0 for success
	 */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
              Category = "vivox"
    )
    void BeginSetAudioConnected(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            bool value,
			bool switchTransmission,
            EVivoxStatus &status
    );

    /**
    * \brief Add or remove text from the channel session
    *
    * \param value true to add text, false to remove text
    * \param callback a delegate to call when this operation completes
    * \return 0 for success
    */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
              Category = "vivox"
    )
    void BeginSetTextConnected(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            bool value,
            EVivoxStatus &status
    );

    /**
     * \brief Send a message to this channel
     *
     * \param language the language of the message e.g "en". This can be null to use the default language ("en" for most systems). This must conform to RFC5646 (https://tools.ietf.org/html/rfc5646)
     * \param message the body of the message
     * \param applicationStanzaNamespace an optional namespace element for additional application data
     * \param applicationStanzaBody the additional application data body
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 for success
     */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
              Category = "vivox"
    )
    void BeginSendText(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FString &language,
            const FString &message,
            const FString &applicationStanzaNamespace,
            const FString &applicationStanzaBody,
            EVivoxStatus &status
    );

    /**
    * \brief Set the 3D position of the speaker, and position and orientation of the listener.
    * \remarks For most applications, the speaker and listener positions are expected to be the same.
    *  The SDK does however support the decoupling of these positions (for instance, the virtual 'mouth'
    *  might be where the avatar is and the virtual 'ears' where the camera view is from).
    *  <br><b>Note:</b> Since this this the most frequently used VivoxCore method (when using Positional channels)
    *  you will not see any voice server response logged unless an error occurs (no reply is sent for success).
    *
    * \param speakerPosition the position of the virtual 'mouth.'
    * \param listenerPosition the position of the virtual 'ear.'
    * \param listenerForwardVector a unit vector, representing the forward (X) direction, or heading, of the listener.
    * \param listenerUpVector a unit vector, representing the up (Z) direction of the listener. Use FVector(0, 0, 1) for a 'global' up, in world space.
    *
    * \return 0 for success
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    EVivoxStatus Set3DPosition(const FVector &speakerPosition,
                               const FVector &listenerPosition,
                               const FVector &listenerForwardVector,
                               const FVector &listenerUpVector);

    /**
     * \brief Get a token that can be used to connect to this channel.
     *
     * \param tokenSigningKey the key corresponding to the issuer for this account that is used to sign the token.
     * \param tokenExpirationDuration the length of time the token is valid for.
     * \return A token that can be used to join this channel
     * \remarks To be used only by applications without secure communications requirements.
     */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    FString GetConnectToken(const FString &tokenSigningKey, FTimespan tokenExpirationDuration);

	/**
	 * \brief Indicates that this session is being transcibed.
	 * \remarks To set this value use BeginSetChannelTranscription().
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	bool IsSessionBeingTranscribed() const;

	/**
	* \brief Enable or disable transcription for this channel session.
	* \param value True to enable channel transcription, false to disable transcription.
	* \param accessToken The access token granting the user access to set transciption for the channel.
	* \param callback a delegate to call when this operation completes.
	* \return 0 for success
	*/
	UFUNCTION(BlueprintCallable,
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
		Category = "vivox"
	)
	void BeginSetChannelTranscription(
		UObject* WorldContextObject,
		struct FLatentActionInfo LatentInfo,
		bool value, 
		const FString& accessToken,
		EVivoxStatus & status
	);

	/**
	 * \brief Get a token that can be used to enable or disable transcription for this channel.
	 * \param tokenSigningKey The key corresponding to the issuer for this account that is used to sign the token.
	 * \param tokenExpirationDuration The length of time the token is valid for.
	 * \return A token that can be used to set transcriptions in this channel.
	 * \remarks To be used only by applications without secure communications requirements.
	 */
	UFUNCTION(BlueprintCallable, Category = "vivox")
	FString GetTranscriptionToken(const FString& tokenSigningKey, FTimespan tokenExpirationDuration);

    IChannelSession *VivoxVoiceChannelSession;
};